/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "Adafruit_GPS.h"

uint8 NewGPSData = 0;

/*----------------------------- GPS FUNCTIONS  --------------------------------------- */

void GPS_Start(){
    
        UART_GPS_Start();
        CyDelay(250);
        
        UART_GPS_PutString(PMTK_SET_NMEA_OUTPUT_RMCGGA);
        //UART_GPS_PutString(PMTK_SET_NMEA_OUTPUT_RMCONLY);
        
        CyDelay(200);        
        UART_GPS_PutString(PMTK_SET_BAUD_57600);
        CyDelay(200);
        Clock_UART_GPS_SetDivider(52);
        UART_GPS_ClearRxBuffer();
}


void GPS_Refresh(){
    static uint8 temp = 0;    
    
//    UART_GPS_RX_STS_FIFO_NOTEMPTY //If set, indicates the FIFO has data available.
//    UART_GPS_RX_STS_PAR_ERROR  //If set, indicates a parity error was detected.
//    UART_GPS_RX_STS_STOP_ERROR //If set, indicates a framing error was detected. The framing error is caused when the UART hardware sees the logic 0 where the stop bit should be (logic 1).
//    UART_GPS_RX_STS_BREAK      //If set, indicates a break was detected.
//    UART_GPS_RX_STS_OVERRUN    //If set, indicates the FIFO buffer was overrun.
//    UART_GPS_RX_STS_ADDR_MATCH //Indicates that the received byte matches one of the two addresses available for hardware address detection. It is not implemented if Address Mode is set to None. In Half Duplex mode, only Address #1 is implemented for this detection.
//    UART_GPS_RX_STS_MRKSPC     //Status of the mark/space parity bit. This bit indicates whether a mark or space was seen in the parity bit location of the transfer. It is not implemented if Address Mode is set to None.
//    UART_GPS_RX_STS_SOFT_BUFF_OVER   //If set, indicates the RX buffer was overrun.
    U8 i = 0;
    if( UART_GPS_GetRxBufferSize() >= 70)
    {
        
        while( UART_GPS_GetRxBufferSize() )
        {
            temp = UART_GPS_GetChar();
            if (temp== '\n')
            {
                break;
            }
            
            
            if(NewGPSData ==0){
                GPS_RMC_Data[i] = temp;
            }
            if(NewGPSData ==2){
                GPS_GGA_Data[i] = temp;
            }
            i++;
        }
        
        //NewGPSData works as a state machine. Set to 0 initially, then 1 after RMC data is written,
        //2 when  rmc data is sent, three when GGA data is written, and back to zero when GGA is sent
        if(NewGPSData == 0)
        {
            //Nullify the unused elements in the RMC array
            for (i = i;i<100; ++i)
            {
                GPS_RMC_Data[i] = '\0';  
            }
            //Prepares the state machine to write the RMC data
            NewGPSData = 1; 
        }
        
        else
        {
            //Nullify the unused elements in the GGA array
            for (i = i;i<100; ++i)
            {
                GPS_GGA_Data[i] = '\0';  
            }
            //Prepares state machine to Write GGA data
            NewGPSData = 3;  
        }
        

        //Any element that is not used in the array is assigned to null
        
        
    }
    /*
    U8 status = UART_GPS_ReadRxStatus();
    if ( (status & UART_GPS_RX_STS_FIFO_NOTEMPTY) == UART_GPS_RX_STS_FIFO_NOTEMPTY   ||
         (status & UART_GPS_RX_STS_SOFT_BUFF_OVER) == UART_GPS_RX_STS_SOFT_BUFF_OVER)
    {
        
        
        //while (UART_GPS_GetChar() != 'R'){}
        
        U8 i;
        for (i = 0; i<225; i++)
        {
            //temp = UART_GPS_GetByte();
            temp = UART_GPS_GetChar();
    */
            
            /*
            if (temp >= 255)
            {
                i--;
                continue;
            }*/
            
            /*
            if (temp == '\r' || temp == '\n' || temp== '$')
            {
                break;
            }*/
     
         /*   
            GPS_Data[i] = temp;
            
        }
        
        //Any element that is not used in the array is assigned to null
        for (i = i;i<250; i++){
            GPS_Data[i] = '\0';  
        }
        
        UART_GPS_ClearRxBuffer();
        NewGPSData = 1;
       
    }*/
 
    return;    
}



/* [] END OF FILE */
