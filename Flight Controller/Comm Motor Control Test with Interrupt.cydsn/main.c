/* ========================================
 *  ÿã
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/ 
#include "project.h"
#include "Flight_Controller.h"
#include "hc12_Config.h"
#include "hc12_Comm.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    FC_Start();
    

    //uint32 c =100;
    uint8 oBV=1, nBV=1, sweepToggle=0, sweepVal=0;
    uint32 timeVal=0;
    for(;;)
    {
        //tempVar = UART_HC12_GetRxBufferSize();

        if (rxCommandFlag == 1)
        {
            //uint8 * command = getFlightCommand4(); //Updates FC_RxSting
            executeFlightCommand(FC_RxString[0],FC_RxString[1]);
            //executeFlightCommand(*command,*command+1);      
        }
        if(sweepToggle){
            if(timeVal++ > 10000)
            {
                Motor_Speed_WriteCompare(100+(sweepVal++)); 
                if (sweepVal>=100) sweepVal = 0;
                timeVal = 0;
            }
        }       
        nBV = button_Read();
        if(oBV < nBV) sweepToggle^=1;
        oBV=nBV;
        
    }    
}


/* [] END OF FILE */
