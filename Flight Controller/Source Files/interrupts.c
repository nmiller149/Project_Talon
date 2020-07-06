/* =========================================
Last edited 04/03/2020
 * "interrupts.c"
 *
 * Author:  Nathan Miller
 *
 * Version: 1.0
 *
 * Description:
 * SOURCE FILE - Contains interrupt handler for
 *
 * Reasons for Revision:
 *    -  
 *    - 
 *    -
 *
**/
 /* ======================================== */

#include "interrupts.h"


/********************************************************************
                    Interrupt Initialization
    
    Used in: Main.c
    Description: For starting isr hardware components and initializing 
    global variables / flags.
********************************************************************/
uint8 interrupts_init(void)
{
    refreshTimerFlag = 0;
    gpsRxFlag = 0;
    isr_Refresh_StartEx(refreshHandler);
    isr_GPS_StartEx(GPS_Handler);
    
    return 1;
}


/********************************************************************
                    Refresh Handler
    
    Used in: Main.c
    Description: Triggered by refresh timer to check time criticall 
    sensor data.
********************************************************************/
CY_ISR(refreshHandler)
{
    /* Clear pending Interrupt */
    isr_Refresh_ClearPending();
    refreshTimerFlag = 1;       
}


/********************************************************************
                    GPS Handler
    
    Used in: Main.c
    Description: Triggered by new GPS NMEA string in FIFO to refresh
    GPS structures.
********************************************************************/
CY_ISR(GPS_Handler)
{
    isr_GPS_ClearPending();
    gpsRxFlag = 1;
}

/* [] END OF FILE */
