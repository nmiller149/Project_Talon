/* ========================================
Last edited 04/04/2020
 * File Name: "main.c" 
 * Project  : sensorDataFlow
 *
 * Author:  Nathan Miller
 *
 * Version: 2.1
 *
 * Description:
 *    Used to build the data aquisition tools for project Talon. Moves data to an from. 
 *    BNO055 9-DOF IMU ,        
 *    ??? Adafruit GPS ,
 *    BMP280 barometric pressure sensor ,
 *    EEPROM
 *    SD CARD
 *    AND eventually an airspeed sensor
 *
 * Reasons for Revision:
 *    - 01/12/2020 Counter_Duration is now Counter_PPS
 *    - 03/30/2020 Merged all of Talon_1 -> main.c that was applicable into this. Updated BNO055 function calls. Updated AGM to IMU in BNO055 function calls and variables
 *    - 04/04/2020 Added getDec and getInt functions. Also introduced new GPS handling and interrupts.
 *
 *
**//* ======================================== */

#include <project.h>

#include "Adafruit_BNO055.h"
#include "Adafruit_GPS.h"
#include "Adafruit_BMP280.h"
#include "interrupts.h"
#include "SD_Config.h"
//#include "Flight_Controller.h"

#include "hc12_Config.h"
#include "hc12_Comm.h"


#define DEFAULT_PRIORITY                    (3u)
#define HIGHER_PRIORITY                     (2u)

//---------- USER INPUTS --------------
#define SENSOR_UPDATE_RATE    30 //Hz
#define REFRESH_DIVIDER       8  //30Hz ? 4 = 7Hz
uint8 commChannel = 30;
//-------------------------------------

extern _Bool refreshTimerFlag;
extern _Bool gpsRxFlag;

//List of variable types to keep track of different manipulations.
typedef enum Talon_Variable_Type
{
    TYPENAME_GPS_UTC,
    TYPENAME_GPS_DATE,
    TYPENAME_GPS_DMS,
    TYPENAME_GPS_DMM,
    TYPENAME_GPS_DDD,
    TYPENAME_GPS_1D,
    TYPENAME_BMP280_U32,
    TYPENAME_BMP280_S32,
    TYPENAME_BNO055_U16,
    TYPENAME_BNO055_S16,
    TYPENAME_BNO055_U8
}Var_Type;

uint16 getDec(uint32 value,Var_Type type);
int32 getInt(uint32 value,Var_Type type);

void HC_Initialize(void)
{
    HC_FindBaud();
    HC_Baud(9600);
    HC_Channel(commChannel);
    HC_Function(3);
    HC_Power(8); 
}

BMP280_U32_t Altitude_Initialize(void)
{
    BMP280_U32_t altTemp=0, initTemp=0;
    for (uint8 i=0; i<7; i++)
    {
        altTemp += *readAltitude(0);
    }
    initTemp = altTemp / 7;
    
    return initTemp;
}


int main(void)
{

    //=========================== Initialization =====================================
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    Timer_1_Start(); //Redundant
    //Timer_2_Start();
    //Timer_2_WritePeriod(10000/SENSOR_UPDATE_RATE); // 10,000kHz / 30 Hz = Period
    
    interrupts_init();
    HC_Start();
    HC_Comm_Init();
    HC_Initialize();
	//FC_Start();
    //FC_Initialize();
    beginBMP280();
    BMP280_S32_t InitAltitude = Altitude_Initialize();
    IMU_Start();
    GPS_Start();
    EEPROM_Start();      
    Start_SD_Card(); 
    //===========================================================================================
    
    //duration,X,Y,Z,Yaw,Roll,Pitch,TEMP,$GPS,,,,,,,,,,,, 

    uint8 refreshCounter = 0;
    uint duration = 0;
    char GPS_String[100];
    char dataArray[512];
    
    BMP280_U32_t Altitude, Pressure, RelativeAltitude;
    BMP280_S32_t Temperature;
    
    for(;;)
    {   
        if (rxCommandFlag == 1)
        {    
        }    
        
        //Refresh GPS_Data
        if (gpsRxFlag == 1)
        {
            int8 status = GPS_GetStringSimp(GPS_String);
            if (status==1)
            {
                gpsRxFlag=0;
                GPS_RefreshData(GPS_String);
            }
        }
        
        //Refresh Sensors and Update
        if (refreshTimerFlag ==1)
        {
            refreshTimerFlag = 0;
            refreshCounter++;
            
            BMP280_U32_t *altPtr = readAltitude(0);
            Altitude  = altPtr[0]; 
            RelativeAltitude = Altitude - InitAltitude;
            Pressure  = altPtr[1];
            Temperature = altPtr[2];
            Temperature = (Temperature *9)/5 + 3200; //Farenheit conversion
            
            IMU_EulersRefresh();
            IMU_AccelRefresh();
            IMU_GetTemp();
            //GPS_Refresh();
            
            duration = Counter_PPS_ReadCounter();
        }// End if refreshTimer
        
        if (refreshCounter >= REFRESH_DIVIDER)
        {
            refreshCounter = 0;
         
            //Write the comma seperated vector to save and send   
            sprintf(dataArray,"%u,%li,%li,%li,%li,%li,%li,%li,%lu.%u,%li.%u,%lu.%u,%li.%u,",duration,getInt(PITCH,TYPENAME_BNO055_S16),getInt(ROLL,TYPENAME_BNO055_S16),getInt(YAW,TYPENAME_BNO055_U16),getInt(XACCEL,TYPENAME_BNO055_S16),getInt(YACCEL,TYPENAME_BNO055_S16),getInt(ZACCEL,TYPENAME_BNO055_S16),getInt(IMUTEMP,TYPENAME_BNO055_U8),getInt(Altitude,TYPENAME_BMP280_U32),
                getDec(Altitude, TYPENAME_BMP280_U32),getInt(RelativeAltitude,TYPENAME_BMP280_U32),getDec(RelativeAltitude,TYPENAME_BMP280_U32),getInt(Pressure,TYPENAME_BMP280_U32),getDec(Pressure,TYPENAME_BMP280_U32),getInt(Temperature,TYPENAME_BMP280_S32),getDec(Temperature,TYPENAME_BMP280_S32));
            
            
            //Concatinate the gps data with the orientation data array
            static char gpsArray[128]="0,0,0,0,0,0,0,"; //initiallize array so that number of tokens is consistant?
            static char  fcArray[128]="0,0,0,0,0,0,0,";
            if (GPS_RefreshData(GPS_GetString()))
            { 
               //sprintf(tempArray,"%lu.%u,%li",GGA.UTC/100,decimal(GGA.UTC,1),GGA.Latitude/100);  
               sprintf(gpsArray,"%lu.%u,%li.%u,%li.%u,%lu.%u,%lu.%u,%lu.%u,%u,",getInt(GGA.UTC,TYPENAME_GPS_UTC),getDec(GGA.UTC,TYPENAME_GPS_UTC),getInt(GGA.Latitude,TYPENAME_GPS_DDD),getDec(GGA.Latitude,TYPENAME_GPS_DDD), getInt(GGA.Longitude,TYPENAME_GPS_DDD),getDec(GGA.Longitude,TYPENAME_GPS_DDD),getInt(GGA.Altitude,TYPENAME_GPS_1D),getDec(GGA.Altitude,TYPENAME_GPS_1D),getInt(RMC.Speed,TYPENAME_GPS_1D),getDec(RMC.Speed,TYPENAME_GPS_1D),getInt(RMC.Direction,TYPENAME_GPS_1D),getDec(RMC.Direction,TYPENAME_GPS_1D),GGA.nSatellites);         
            }
            
            //send it!
            UART_HC12_PutString(dataArray);
            UART_HC12_PutString(gpsArray);
            UART_HC12_PutString("\r\n\0");
            
            //Add control Data
            //sprintf(gpsArray2,"%u,%u,%i,%u,%u,%i,%i,%i",FC._Roll, FC._Pitch, FC._Yaw, FC._Flap, FC._Throttle, FC._TrimRoll, FC._TrimPitch, FC._TrimYaw);
            strcat(dataArray,gpsArray);
            strcat(dataArray, fcArray);
            strcat(dataArray,"\r\n\0\0");
            
            //Save it!
            if(pFile)//Checks if file was created
            {
                if(FS_Write(pFile, &dataArray, strlen(dataArray)) != 0) 
                {
                    /* Indicate that data was written to a file */
                }
                else
                {

                }
            } //End if pFile
        }//END if refreshCounter
    }//END for(;;)   
    
}/* [] END OF MAIN  */



//Get Mantissa
uint16 getDec(uint32 value, Var_Type type)
{    
    uint16 decimal;
    int32 value_s;
    
    switch(type) 
    {
        case TYPENAME_GPS_UTC:
            decimal = (value-(value/100)*100);
            break;
        case TYPENAME_GPS_DATE :
            decimal = 0;
            break;
            
        case TYPENAME_GPS_DMS :
            decimal = 0; 
            break;
            
        case TYPENAME_GPS_DMM :
            value_s = (int32) value;
            decimal = (value_s-(value_s/100)*100); 
            break;
            
        case TYPENAME_GPS_DDD :
            value_s = (int32) value;
            decimal = (value_s-(value_s/10000)*10000);
            break;
            
        case TYPENAME_GPS_1D :
            value_s = (int32) value;
            decimal = (value_s-(value_s/10)*10);
            break;
            
        case TYPENAME_BMP280_U32 :
            decimal = (value-(value/100)*100);
            break;
            
        case TYPENAME_BMP280_S32 :
            value_s = (int32) value;
            decimal = (value_s-(value_s/100)*100);
            break;    
            
        case TYPENAME_BNO055_S16:
            decimal = (value & 0x000F);
            break;   
            
        case TYPENAME_BNO055_U16:
            decimal = (value & 0x000F);
            break;      
            
        case TYPENAME_BNO055_U8:
            decimal = (value & 0x0001);
            break;       
            
        default:
            decimal = 12345;
            break;
    }
    
    return decimal;
}

//Get Characteristic
int32 getInt(uint32 value, Var_Type type)
{    
    uint16 integer;
    int32 value_s;
    
    switch(type) 
    {
        case TYPENAME_GPS_UTC:
            integer = (value/100);
            break;
        case TYPENAME_GPS_DATE :
            integer = 0;
            break;
            
        case TYPENAME_GPS_DMS :
            integer = 0; 
            break;
            
        case TYPENAME_GPS_DMM :
            value_s = (int32) value;
            integer = (value_s/100); 
            break;
            
        case TYPENAME_GPS_DDD :
            value_s = (int32) value;
            integer = (value_s/10000);
            break;
            
        case TYPENAME_GPS_1D :
            value_s = (int32) value;
            integer = (value_s/10);
            break;
            
        case TYPENAME_BMP280_U32 :
            integer = (value/100);
            break;
            
        case TYPENAME_BMP280_S32 :
            value_s = (int32) value;
            integer = (value_s/100);
            break;    
            
        case TYPENAME_BNO055_S16:
            value_s = (int32) value;
            integer = (value_s >> 4);
            break;   
            
        case TYPENAME_BNO055_U16:
            integer = (value >> 4);
            break;      
            
        case TYPENAME_BNO055_U8:
            integer = (value >> 1);
            break;       
            
        default:
            integer = 0;
            break;
    }
    
    return integer;
}
