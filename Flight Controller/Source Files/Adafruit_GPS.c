/* =========================================
Last edited 04/06/2020
 * "Adafruit_BMP280.h"
 *
 * Author:  Nathan Miller
 *
 * Version: 2.1
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
 *    -  04/06/2020 - Fixed bugs in Checksum and refreshdata through extensive testing. Added atoi_deci and clean_gps_string as private functions
 *
**/
 /* ======================================== */

#include "Adafruit_GPS.h"

uint8 NewGPSData = 0;
uint8 clean_gps_string(char * ptr);
int atoi_deci(char * ptr);

/*========================================================================================
                       PUBLIC FUNCTIONS
==========================================================================================*/

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
        if(buffer[bufIndex] == '\n' && (increment_enable == 1))
        {
            increment_enable = 0;
            bufIndex = 0;
            strcpy(GPS_String_Pointer, buffer);
            memset(buffer,0,100);
            return 1;
        }
        else if (increment_enable == 1)
            bufIndex++;
		else
			bufIndex = 0;
			
        if (buffer[bufIndex] == '$')
		{
            increment_enable = 1;
			buffer[0] = '$';
			bufIndex=1;
		}
    }
    return increment_enable-1;
}


/********************************************************************
                    GPS_RefreshData()
    
    Takes a NMEA string starting with the '$' + NMEA code and parses it to find
    the Value of each component of the cooresponding NMEA string. Each
    component is a global variable declared in Adafruit_GPS.h. Returns
	1 if succesful, 0 for empty string or bad checksum.
********************************************************************/
uint8 GPS_RefreshData(char *GPS_String)
{    
    if ( GPS_Checksum(GPS_String) == 1)
    {
		char _GPS_String [100];
		strcpy(_GPS_String, GPS_String);
		
		if (clean_gps_string(_GPS_String) == 1)
		{
			char *ptr;
			ptr = strtok(_GPS_String, ",");
			++ptr; //remove '$'
			
			if ( strcmp(ptr, "GPGGA") ==0 )
			{
 				GGA.UTC  = atoi(strtok(NULL,","));
				//GGA.UTC += atoi_deci(ptr);
				GGA.Hour  = GGA.UTC / 10000;
				GGA.Minute= GGA.UTC/100 - GGA.Hour*100;
				GGA.Second= GGA.UTC - GGA.Hour*10000 - GGA.Minute*100;
				
				ptr = strtok(NULL,",");
				GPS_DMM_t dmm = atoi(ptr)*10000;
				dmm += atoi_deci(ptr);
				GGA.Latitude = dmm;//(dmm/10000)*10000;
				GGA.Latitude += 0;//(dmm - GGA.Latitude)/60;
				ptr = strtok(NULL,",");
				if (*ptr == 'S')
					GGA.Latitude *= -1;
				
				ptr = strtok(NULL,",");
				dmm = atoi(ptr)*10000;
				dmm += atoi_deci(ptr);
				GGA.Longitude = (dmm/1000000)*1000000;
				GGA.Longitude += (dmm - GGA.Longitude)/60;
				ptr = strtok(NULL,",");
				if (*ptr == 'W')
					GGA.Longitude *= -1;
					
				GGA.Fix_Quality = atoi(strtok(NULL,","));
				GGA.nSatellites = atoi(strtok(NULL,","));
				
				ptr = strtok(NULL,",");
				GGA.Horizontal_Dilution = atoi(ptr)*100;
				GGA.Horizontal_Dilution += atoi_deci(ptr);
				
				ptr = strtok(NULL,",");
				GGA.Altitude = atoi(ptr)*10;
				GGA.Altitude += atoi_deci(ptr);
				ptr = strtok(NULL,",."); //Should == 'M'
				if (*ptr == 'M') 
					GGA.Altitude = (GGA.Altitude * 328) /100; //Feet conversion
				
				ptr = strtok(NULL,",");
				GGA.Geoidal_Separation = atoi(ptr)*10;
				GGA.Geoidal_Separation +=(abs(GGA.Geoidal_Separation)/GGA.Geoidal_Separation)*atoi_deci(ptr); //check for negative to add/sub decimal
				ptr = strtok(NULL,",."); //Should == 'M'
				if (*ptr != 'M') 
					GGA.Geoidal_Separation = (GGA.Geoidal_Separation * 328) /100; //Feet converstion
				
				//Typically empty from here down.
				ptr = strtok(NULL,",*");
				GGA.Last_Update = atoi(ptr)*10;
				GGA.Last_Update += atoi_deci(ptr);
				
				GGA.Ref_Station_ID = atoi(strtok(NULL,",.*\r\n")); 
				return 1;
			} //END If GPGGA
			else if( strcmp(ptr, "GPRMC") ==0 )
			{
				RMC.UTC = atoi(strtok(NULL,","));
				//RMC.UTC += atoi_deci(ptr);
				RMC.Hour  = RMC.UTC / 10000;
				RMC.Minute= RMC.UTC/100 - RMC.Hour*100;
				RMC.Second= RMC.UTC - RMC.Hour*10000 - RMC.Minute*100;
				
				RMC.Status = *strtok(NULL,","); // =A; What is this?
				
				ptr = strtok(NULL,",");
				GPS_DMM_t dmm = atoi(ptr)*10000;
				dmm += atoi_deci(ptr);
				RMC.Latitude = (dmm/10000)*10000;
				RMC.Latitude += (dmm - RMC.Latitude)/60;
				ptr = strtok(NULL,",");
				if (*ptr == 'S')
					RMC.Latitude *= -1;
				
				ptr = strtok(NULL,",");
				dmm = atoi(ptr)*10000;
				dmm += atoi_deci(ptr);
				RMC.Longitude = (dmm/1000000)*1000000;
				RMC.Longitude += (dmm - RMC.Longitude)/60;
				ptr = strtok(NULL,",");
				if (*ptr == 'W')
					RMC.Longitude *= -1;
				
				ptr = strtok(NULL,",");
				RMC.Speed = atoi(ptr) *100;
				RMC.Speed +=  atoi_deci(ptr);
				
				ptr = strtok(NULL,",");
				RMC.Direction = atoi(ptr)*100;
				RMC.Direction += atoi_deci(ptr);
				
				RMC.Date = atoi(strtok(NULL,",")); //Extract day month year as todo?
				RMC.Day = RMC.Date /10000;
				RMC.Month = RMC.Date /100 - RMC.Day * 100;
				RMC.Year = RMC.Date - RMC.Day*10000 - RMC.Month*100;
				
				//Below is normally null //Adafruit GPS produces extra comma and char 'A' at end before checksum
				ptr = strtok(NULL,",");
				RMC.Magnetic_Variation += atoi(ptr) *10;
				RMC.Magnetic_Variation = atoi_deci(ptr);
				ptr = strtok(NULL,",*\r\n");
				if (*ptr == 'W')
					RMC.Magnetic_Variation*=-1;
				return 1;        
			} //End If GPRMC
		} //End If clean_gps_string
    } //End If GPS_Checsum
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
        if (GPS_String[i] == 0 )
            return -1;
        else if (GPS_String[i] == '$')
        {
            ++i;
            while(GPS_String[i] != '*' && i <255)
            {
                if (GPS_String[i] == 0) 
                    return -1;
                xor ^= GPS_String[i];
                ++i;
            }
            char * ptr = &GPS_String[i+3];
			return (xor == strtol(&GPS_String[i+1], &ptr , 16));
        }
    }
    return -1;
}


/*========================================================================================
                       PRIVATE FUNCTIONS
==========================================================================================*/


/********************************************************************
                    int atoi_deci(uint8 * ptr)
 
    Expects an ascii string of numbers containing a decimal. Returns
	the decimal value. 
********************************************************************/
int atoi_deci(char * ptr)
{
	//for(uint8 i=0; i < (sizeof(ptr)/sizeof(uint8)); i++)
	uint8 size = strlen(ptr);
	
	for(uint8 i = 0; i < size; ++i)
	{
		if (ptr[i]=='.')
		{
			return atoi(&ptr[i+1]);
		}
	}
	return 0;
}


/********************************************************************
                    uint8 clean_gps_string(uint8 * ptr)
 
    Parses through GPS ASCII sentence and fills each null token with
	"null" phrase. If more than 3 nulls occur and original sentence
	is less than 45 chars long, then function returns 0; Otherwise
	returns 1.
	 * Current functionallity only applicable for GGA and RMC strings
	 * in Adafruit GPS module
********************************************************************/
uint8 clean_gps_string(char * ptr)
{
	char fill[5] = "0";
	uint8 lenString = strlen(ptr);
	uint8 lenFill = strlen(fill);
	uint8 numFills = 0;
	
	for(uint8 i=1; i < lenString; ++i)
    {
		if (ptr[i]== ',' && ptr[i-1] == ',')
		{
			memmove(ptr+i+lenFill, ptr+i, lenString-i+lenFill-1);
			memcpy(&ptr[i], fill, lenFill);
			lenString+= lenFill;
			i += lenFill;	
			++numFills;
		}
	}

	if((numFills > 3) && (lenString < 45))
		return 0;
	
	return 1;
}

