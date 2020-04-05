/* =========================================
Last edited 03/15/2020
 * "Adafruit_BMP280.h"
 *
 * Author:  Nathan Miller
 *
 * Version: 2.0
 *
 * Description:
 * HEADER FILE - Contains all files, functions, and variables used for the Bosch MTk33x9 GPS chipset implemented on the Adafruit Ultimate GPS Module
 *
 * To Do's: 
 * - Change UART to "addressed byte for byte" and make interrupt driven.
 *
 *
 * Reasons for Revision:
 *    -  07/06/2019 - Added more functionality to extract data from GPS NMEA strings and cleaned up library 
 *    -  04/04/2020 - Added UTC hour, minute and seconds. Finshed  RMC. Created Checksum calculator. New GetStringSimp method.
 *    -
 *
**/
 /* ======================================== */

#include "Adafruit_GPS.h"

uint8 NewGPSData = 0;

/*=========================================================================
                       PUBLIC FUNCTIONS
=========================================================================*/

/********************************************************************
                    GPS_Start()
 
********************************************************************/
void GPS_Start(){
    
        UART_GPS_Start();
        Clock_UART_GPS_SetDividerValue(BCLK__BUS_CLK__HZ/(9600*8));
        Counter_PPS_Start();     
        CyDelay(100);    
                
        UART_GPS_PutString(PMTK_SET_BAUD_57600);
        CyDelay(100);
        Clock_UART_GPS_SetDividerValue(BCLK__BUS_CLK__HZ/(57600*8));
        UART_GPS_ClearRxBuffer();
        CyDelay(100);
        UART_GPS_PutString(PMTK_SET_NMEA_OUTPUT_RMCGGA);
        //UART_GPS_PutString(PMTK_SET_NMEA_OUTPUT_RMCONLY);
        CyDelay(250);
}


/********************************************************************
                    GPS_GetString()
    
    Uses a static circular buffer that is parsed to produce string from
    $ to new line feed. Used in the cases where serial data is collected
    or checked sporadically and multiple strings could be contained in
    same buffer together. NEEDS TESTED. 
********************************************************************/
char *GPS_GetString()
{
    //Sets content from GPS NMEA string between $ and * not including these 
    if (UART_GPS_GetRxBufferSize() >= 50)
    {
        static char cbuffer[256]= {_NULL}, newString[100] = {_NULL};
        static uint8 bufIndex = 0;
        static uint16 i = 0,  end =0; //for parsing only. i is last place of succesful string
        
        //Read UART buffer into circular software buffer
        while(UART_GPS_GetRxBufferSize() > 0)
        {
            cbuffer[bufIndex++] = UART_GPS_GetChar();
        }
        
        // Parse through sw buffer to find full string;
        char *ptrStart = _NULL, *ptrEnd = _NULL;
        if (bufIndex < i)
        {
            end = bufIndex + 256;  //So that parse loop will loop around ciruclar buffer
        }
        else
            end = bufIndex;
        
        for(; i<end; i++)
        {
            
            uint8 p = i; //Should cut off 256 and loop to 0 automatically.
         
            if (cbuffer[p] == '$')
                ptrStart = &cbuffer[p+1];
            else if (cbuffer[p] == '\n')
                ptrEnd = &cbuffer[p];
        }
                
        //Copy Succesful string segment to newString
        if (ptrStart != _NULL && ptrEnd != _NULL)
        {
            strncpy(newString,ptrStart,ptrEnd-ptrStart); //Should cutoff '/n' from string
            return newString;
        }
    } //End If UART>50
    return NULL;  
}

/********************************************************************
                    GPS_GetStringSimp()
    
    Fills a user specified buffer with a string from $ to new line feed.
    Used in the cases where serial data is collected and checked at a 
    higher speed than incoming data and using interrupts. 
    
    If returns -1 then no data was found or error. Returns 0 if data was
    cutoff and needs ran again. Returns 1 if full NMEA sentence is found. NEEDS TESTED. 
********************************************************************/
int8 GPS_GetStringSimp(char *GPS_String_Pointer)
{
    static char buffer[100]= {_NULL};
    static uint8 bufIndex = 0, increment_enable = 0;
    
    //Read UART buffer into circular software buffer
    while(UART_GPS_GetRxBufferSize() > 0)
    {
        buffer[bufIndex] = UART_GPS_GetChar();
        if(buffer[bufIndex] == '\n')
        {
            increment_enable = 0;
            bufIndex = 0;
            strcpy(buffer,GPS_String_Pointer);
            //memset(buffer,0,100);
            return 1;
        }
        else if (increment_enable == 1)
            bufIndex++;
        else if (buffer[bufIndex] == '$')
            increment_enable = 1;
    }
    if (increment_enable == 1)
        bufIndex=0;
    
    return increment_enable-1;
}


/********************************************************************
                    GPS_RefreshData()
    
    Takes a NMEA string starting with the NMEA code and parses it to find
    the Value of each component of the cooresponding NMEA string. Each
    component is a global variable declared in Adafruit_GPS.h
********************************************************************/
uint8 GPS_RefreshData(char *GPS_String)
{    
    if ( GPS_Checksum(GPS_String) == 1)
    {
        char *ptr;
        ptr = strtok(GPS_String, ",");
        
        if ( strcmp(ptr, "GPGGA") ==0 )
        {
            //GGA.UTC = strtol(strtok(NULL,",."),&ptr,10); //needs <stdlib.h>
            GGA.UTC  = atoi(strtok(NULL,",."))*100;
            GGA.UTC += atoi(strtok(NULL,"."));
            GGA.Hour  = GGA.UTC / 1000000;
            GGA.Minute= GGA.UTC / 10000 - GGA.Hour*100;
            GGA.Second= GGA.UTC / 100 - GGA.Hour*10000 - GGA.Minute*100;
            
            GPS_DMM_t dmm = atoi(strtok(NULL,",."))*100;
            dmm += atoi(strtok(NULL,",."));
            GGA.Latitude = (dmm/10000)*10000;
            GGA.Latitude += (dmm - RMC.Latitude)/60;
            ptr = strtok(NULL,",.");
            if (*ptr == 'S')
                GGA.Latitude *= -1;
            
            dmm = atoi(strtok(NULL,",."))*100;
            dmm += atoi(strtok(NULL,",."));
            GGA.Longitude = (dmm/10000)*10000;
            GGA.Longitude += (dmm - RMC.Longitude)/60;
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
            
            GGA.Ref_Station_ID = atoi(strtok(NULL,",.\r\n")); 
            return 1;
        } 
        else if( strcmp(ptr, "GPRMC") ==0 )
        {
            RMC.UTC = atoi(strtok(NULL,",."))*100;
            RMC.UTC += atoi(strtok(NULL,"."));
            RMC.Hour  = RMC.UTC / 1000000;
            RMC.Minute= RMC.UTC / 10000 - RMC.Hour*100;
            RMC.Second= RMC.UTC / 100 - RMC.Hour*10000 - RMC.Minute*100;
            
            RMC.Status = *strtok(NULL,",.");
            
            GPS_DMM_t dmm = atoi(strtok(NULL,",."))*100;
            dmm += atoi(strtok(NULL,",."));
            RMC.Latitude = (dmm/10000)*10000;
            RMC.Latitude += (dmm - RMC.Latitude)/60;
            ptr = strtok(NULL,",.");
            if (*ptr == 'S')
                RMC.Latitude *= -1;
            else if (*ptr != 'N')
            {
                //ERROR
            }
            
            dmm = atoi(strtok(NULL,",."))*100;
            dmm += atoi(strtok(NULL,",."));
            RMC.Longitude = (dmm/10000)*10000;
            RMC.Longitude += (dmm - RMC.Longitude)/60;
            ptr = strtok(NULL,",.");
            if (*ptr == 'W')
                RMC.Longitude *= -1;
            else if (*ptr != 'E')
            {
                //ERROR
            }
            
            RMC.Speed = atoi(strtok(NULL,",.")) *10;
            RMC.Speed += atoi(strtok(NULL,",."));
            
            RMC.Direction += atoi(strtok(NULL,",."))*10;
            RMC.Direction = atoi(strtok(NULL,",."));
            
            RMC.Date = atoi(strtok(NULL,",."));
            
            RMC.Magnetic_Variation += atoi(strtok(NULL,",.")) *10;
            RMC.Magnetic_Variation = atoi(strtok(NULL,",."));
            ptr = strtok(NULL,",.\r\n");
            if (*ptr == 'W')
                RMC.Magnetic_Variation*=-1;
            return 1;        
        }
    }
    return 0;
}

/********************************************************************
                    GPS_Checksum()
 
    Looks for '$' in string and xor's each character between that and
    '*'. After astrisk, the next token of chars is taken and converted to
    an integer to check against. Returns 1 for good checksum, 0 for bad 
    checksum, and -1 if error occurs.
********************************************************************/
int8 GPS_Checksum(char *GPS_String)
{
    uint8 i=0, xor=0;
    
    for(i=0; i<255; ++i)
    {
        if (GPS_String[i] != 0 )
            return -1;
        else if (GPS_String[i] == '$')
        {
            ++i;
            while(GPS_String[i] != '*' && i <255)
            {
                if (GPS_String[i] != 0) 
                    return -1;
                xor ^= GPS_String[i];
                ++i;
            }
            
            return (xor == atoi(strtok(&GPS_String[++i],",")));
        }
    }
    return -1;
}
