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
        Clock_UART_GPS_SetDividerValue(BCLK__BUS_CLK__HZ/(9600*8));
        Counter_PPS_Start();     
        CyDelay(250);    
        UART_GPS_PutString(PMTK_SET_NMEA_OUTPUT_RMCGGA);
        //UART_GPS_PutString(PMTK_SET_NMEA_OUTPUT_RMCONLY);
        
        CyDelay(100);        
        UART_GPS_PutString(PMTK_SET_BAUD_57600);
        CyDelay(100);
        Clock_UART_GPS_SetDividerValue(BCLK__BUS_CLK__HZ/(57600*8));
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

char *GPS_GetString()
{
    //Sets content from GPS NMEA string between $ and * not including these 
    if (UART_GPS_GetRxBufferSize() >= 50)
    {
        static char buffer[256]= {_NULL}, newString[100] = {_NULL};
        static uint8 bufIndex = 0;
        static uint16 i = 0,  end =0; //for parsing only. i is last place of succesful string
        
        //Read UART buffer into circular software buffer
        while(UART_GPS_GetRxBufferSize() > 0)
        {
            buffer[bufIndex] = UART_GPS_GetChar();
            bufIndex++;
        }
        
        // Parse through sw buffer to find full string;
        char *ptrStart = _NULL, *ptrEnd = _NULL;
        if (bufIndex < i)
        {
            end = bufIndex + 256;  //So that parse loop will loop around ciruclar buffer
        }
        else
            end = bufIndex;
        
        for(i=i; i<end; i++)
        {
            uint8 p = i; //Should cut off 256 and loop to 0 automatically.
         
            if (buffer[p] == '$')
                ptrStart = &buffer[p+1];
            else if (buffer[p] == '*')
                ptrEnd = &buffer[p];
        }
                
        //Copy Succesful string segment to newString
        if (ptrStart != _NULL && ptrEnd != _NULL)
        {
            strncpy(newString,ptrStart,ptrEnd-ptrStart); //Should cutoff '*' from string
            return newString;
        }
    } //End If UART>50
    return NULL;  
}

uint8 GPS_RefreshData(char *GPS_String)
{
    char *ptr;
    ptr = strtok(GPS_String, ",");
    
    if ( strcmp(ptr, "GPGGA") ==0 )
    {
        //GGA.UTC = strtol(strtok(NULL,",."),&ptr,10); //needs <stdlib.h>
        GGA.UTC = atoi(strtok(NULL,",."))*100;
        GGA.UTC += atoi(strtok(NULL,",."));
        GGA.Latitude = atoi(strtok(NULL,",."))*100;
        GGA.Latitude += atoi(strtok(NULL,",."));
        
        ptr = strtok(NULL,",.");
        if (*ptr == 'S')
            GGA.Latitude *= -1;
        GGA.Longitude = atoi(strtok(NULL,",."))*100;
        GGA.Longitude += atoi(strtok(NULL,",."));
        
        ptr = strtok(NULL,",.");
        if (*ptr == 'W')
            GGA.Longitude *= -1;
        GGA.Fix_Quality = atoi(strtok(NULL,",."));
        GGA.nSatellites = atoi(strtok(NULL,",."));
        
        ptr = strtok(NULL,",.");
        GGA.Horizontal_Dilution = atoi(ptr)*10;
        if (ptr[-1] == '.') 
            GGA.Horizontal_Dilution += atoi(strtok(NULL,",."));
        
        ptr = strtok(NULL,",.");
        GGA.Altitude = atoi(ptr)*10;
        if (ptr[-1] == '.') 
            GGA.Altitude += atoi(strtok(NULL,",."));
        ptr = strtok(NULL,",."); //Should == 'M'
        if (*ptr == 'M') 
            GGA.Altitude = (GGA.Altitude * 338) /100; //Feet converstion
        
        ptr = strtok(NULL,",.");
        GGA.Geoidal_Separation = atoi(ptr)*10;
        if (ptr[-1] == '.') 
            GGA.Geoidal_Separation += atoi(strtok(NULL,",."));
        ptr = strtok(NULL,",."); //Should == 'M'
        if (*ptr == 'M') 
            GGA.Geoidal_Separation = (GGA.Geoidal_Separation * 338) /100; //Feet converstion
        
        ptr = strtok(NULL,",.");
        GGA.Last_Update = atoi(ptr)*10;
        if (ptr[-1] == '.') 
            GGA.Last_Update += atoi(strtok(NULL,",."));
        
        GGA.Ref_Station_ID = atoi(strtok(NULL,",."));         
        return 1;
    } 
    else if( strcmp(ptr, "GPRMC") ==0 )
    {
        
        return 1;
    }
    return 0;
}


/* [] END OF FILE */
      //$GPGGA,hhmmss.ss,ddmm.mm,a,dddmm.mm,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh
      //NMEA_Code, UTC, Latitude, NS, Longitude, EW, Quality, Satellites, Horizontal_Dilution, Altitude, Geoidal_Separation, Last_Update, Ref_Station_ID
//    GPS_UTC_t UTC; 
//    GPS_COORD_t Latitude, Longitude;
//    uint8 Fix_Quality;//enum 0 ->8 (we want 1);
//    uint8 nSatellites;
//    GPS_1D_t Horizontal_Dilution, Altitude, Geoidal_Separation;
//    uint16 Last_Update, Ref_Station_ID; //No Last_Update, No Ref_Station_ID (JUNK);