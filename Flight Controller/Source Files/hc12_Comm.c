/* ========================================
Last edited 01/12/2020
 * "hc12_Comm.c"
 *
 * Author:  Nathan Miller
 *
 * Version: 0.3
 *
 * Description:
 * SOURCE FILE - Contains all files for communicating between two HC12 devices. This version of the HC12 Communication
 *               layer is focuesd on combining all communication of and to the HC12 from all other files. 
 *
 *       TODOS:   Version 0.3 will also contain a routine for sending data directly upon receiving the data to prevent 
 *                packets from clashing (receive and send at the same time). This will require a master (either plane or 
 *                more likely the base station) to produce each message at a specific rate (5-10Hz) and the slave to send
 *                back at that rate while keeping a rhythm incase a message is missed. Master might continuously resend 
 *                last message or could send an acknowledge/synce message to fill required rate.
 *               
 * Reasons for Revision:
 *    - Previous Version 1.0 did not contain anything useful. This file is completely different minus the name.
 *    - [0.3] Created Initialization Function
 *    -
 *
**/
 /* ======================================== */


#include "hc12_Comm.h"


/*******************************************************************************
* Function Name: UARTisrHandler
********************************************************************************
* Summary:
*  The interrupt handler for GPIO interrupts.
*  Clears a pending Interrupt.
*  Clears a pin Interrupt.
*  Blinks the LED with the LED_Isr pin.
*  Calls nested interrupt.

*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void HC_Comm_Init(){    
    rxCommandFlag=0;                   //initialize hc12_Comm
    isr_HC12_Rx_StartEx(FCisrHandler); //initialize hc12_Comm
}

CY_ISR(FCisrHandler)
{

    /* Clear pending Interrupt */
    isr_HC12_Rx_ClearPending();
    
    static uint8 Index=0;
    
    while (UART_HC12_GetRxBufferSize() > 0)
    {
        char temp = UART_HC12_GetChar();
        
        if (Index== 0 && temp == HI8(COMMAND_START_BYTE))
        {
            LED_Write(1u);
            Index=1;
        }
        else if (Index==1 && temp == LO8(COMMAND_START_BYTE))
        {
            Index=2;
        }
        else if (Index ==2)
        {
            FC_RxString[0] = temp;
            Index=3;
        }
        else if (Index == 3)
        {
            if(FC_RxString[0] == HI8(COMMAND_START_BYTE) && temp == LO8(COMMAND_START_BYTE))
            {
                Index=2;
            }
            else
            {
                FC_RxString[1] = temp;
                rxCommandFlag = 1;
                Index=0;
                LED_Write(0u);
                break;
            }
        }
        else
        {
            Index = 0;
            LED_Write(0);
        }
    }

}
/*******************************************************************************/


