/* ========================================
Last edited 07/6/2020
 * File Name: "main.c" 
 * Project  : Talon_01
 * Branch   : study-Bandwidth_Utilization
 *
 * Author:  Nathan Miller
 *
 * Version: 2.2.0
 *
 * Description: 
 *       Branched off of Master v2.0.3. To determine how much bandwidth is consumed by different methods and interfaces.
 *       V2.2.0 focuses on IMU I2C bandwidth
 *    
 * Reasons for Revision:
 *    - xx/xx/xx 
 *    -
**/
/* ======================================== */



#include <project.h>

#include "Adafruit_BNO055.h"
#include "Adafruit_GPS.h"
#include "Adafruit_BMP280.h"
#include "interrupts.h"
#include "SD_Config.h"
#include "Flight_Controller.h"

#include "hc12_Config.h"
#include "hc12_Comm.h"


#define DEFAULT_PRIORITY                    (3u)
#define HIGHER_PRIORITY                     (2u)

//---------- USER INPUTS --------------
#define SENSOR_UPDATE_RATE          30 //Hz
#define REFRESH_DIVIDER             8  //30Hz ÷ 8 = 3.5Hz
#define FC_AUTO_NEUTRALIZE_TIME     500 //ms
#define ALT_INIT_FILTER_SIZE        100 //samples
//-------------------------------------

extern _Bool refreshTimerFlag;
extern _Bool gpsRxFlag;
_Bool fcAutoNeutralizeOption; //Make external when remote control is added to FC

typedef enum FC_Option_List
{
    OFF,
    ON
}Option_Val;

//List of variable types to keep track of different manipulations.
typedef enum Talon_Variable_Type
{
    TYPENAME_GPS_UTC,
    TYPENAME_GPS_DATE,
    TYPENAME_GPS_DMS,
    TYPENAME_GPS_DMM,
    TYPENAME_GPS_DDD,
    TYPENAME_GPS_1D,
	TYPENAME_GPS_2D,
    TYPENAME_BMP280_U32,
    TYPENAME_BMP280_S32,
    TYPENAME_BNO055_U16,
    TYPENAME_BNO055_S16,
    TYPENAME_BNO055_T16
}Var_Type;

uint16 getDec(uint32 value,Var_Type type);
int32 getInt(uint32 value,Var_Type type);
uint32 avg(uint32 *array, uint32 arraysize);

void HC_Initialize(uint8 commChannel)
{
    HC_FindBaud();
    HC_Baud(9600);
    HC_Channel(commChannel);
    HC_Function(3);
    HC_Power(8); 
}

void FC_Initialize(void)
{
    executeFlightCommand(FC_PITCH, 50);
    executeFlightCommand(FC_ROLL, 50);
    executeFlightCommand(FC_YAW, 25);
    CyDelay(25);
    executeFlightCommand(FC_PITCH, 0);  //Pitch Down
    executeFlightCommand(FC_ROLL, 0);  //Roll Left
    CyDelay(500);
    executeFlightCommand(FC_PITCH, 100); //Pitch Up
    executeFlightCommand(FC_ROLL, 100); //Roll Right
    CyDelay(500);
    executeFlightCommand(FC_PITCH, 50);
    executeFlightCommand(FC_ROLL, 50);
    CyDelay(500);
    executeFlightCommand(FC_YAW, 0);  //Yaw Left
    CyDelay(500);
    executeFlightCommand(FC_YAW, 50);  //Yaw Right
    CyDelay(500);
    executeFlightCommand(FC_PITCH, 50);
    executeFlightCommand(FC_ROLL, 50);
    executeFlightCommand(FC_YAW, 25); 
}

BMP280_U32_t Altitude_Initialize(void)
{
    BMP280_U32_t altTemp=0, initTemp=0;
    for (uint8 i=0; i<ALT_INIT_FILTER_SIZE; i++)
    {
        altTemp += *readAltitude(0);
    }
    initTemp = altTemp / ALT_INIT_FILTER_SIZE;
    
    return initTemp;
}


int main(void)
{ 
    //=========================== Initialization =====================================/
    
    // ===================== Defaults ==================/
    fcAutoNeutralizeOption = ON;
    uint8 commChannel = 30;
    // =================================================/
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    Timer_1_Start(); //Redundant
    Timer_2_Start();
    Timer_2_WritePeriod(10000/SENSOR_UPDATE_RATE); // 10,000kHz / 30 Hz = Period
    Timer_SW_Start(); //DEBUG
    
	interrupts_init();
    HC_Start();
    HC_Comm_Init();
    HC_Initialize(commChannel);
    FC_Start();
    //FC_Initialize();
	
    beginBMP280();
    IMU_Start();
    GPS_Start();
    EEPROM_Start();      
    Start_SD_Card(); 
    BMP280_S32_t InitAltitude = Altitude_Initialize();
    
    //Print Headers to SD and through HC12
    char rfHeaderString[256] =
    "PPS_count,PITCH,ROLL,YAW,XACCEL,YACCEL,ZACCEL,IMUTEMP,Altitude,RelativeAltitude,Pressure,BMP280_Temperature,gpsArray,GGA.UTC,GGA.Latitude,GGA.Longitude,GGA.Altitude,RMC.Speed,RMC.Direction,GGA.nSatellites,GGA.Fix_Quality,RMC.Status\r\n"; 
    char sdHeaderString[325] = 
    "PPS_count,PITCH,ROLL,YAW,XACCEL,YACCEL,ZACCEL,IMUTEMP,Altitude,RelativeAltitude,Pressure,BMP280_Temperature,gpsArray,GGA.UTC,GGA.Latitude,GGA.Longitude,GGA.Altitude,RMC.Speed,RMC.Direction,GGA.nSatellites,GGA.Fix_Quality,RMC.Status,FC._Roll,FC._Pitch,FC._Yaw,FC._Flap,FC._Throttle,FC._TrimRoll,FC._TrimPitch,FC._TrimYaw\r\n";
    UART_HC12_PutString(rfHeaderString);
    if(pFile)
    {
        FS_Write(pFile, sdHeaderString, strlen(sdHeaderString));
    }

    //===========================================================================================/

    uint8 refreshCounter = 0;
    uint PPS_count = 0;
    uint16 rxCommandStartTime = 0;
    char dataArray[512];
	char GPS_String[100];
    char timingArray[100]; //DEBUG
    uint16 time0, time1, timespan, downtime; //DEBUG
    uint32 timespan_us[256], downtime_us, timespan_avg_us; //DEBUG
    uint8 filt_index=255; //DEBUG;
    
    BMP280_U32_t Altitude, Pressure; //x.xx *100
    BMP280_S32_t Temperature, RelativeAltitude; //x.xx *100

    for(;;)
    {   
        if (rxCommandFlag == 1)
        {
            LED_Write(1); //DEBUG;
            rxCommandStartTime = Timer_1_ReadCounter();
            rxCommandFlag=0;
            executeFlightCommand(FC_RxString[0],FC_RxString[1]);
            LED_Write(0); //DEBUG
        }
        //Auto neutralize ailerons and elevators if timer expires.
        else if( ((uint16)(rxCommandStartTime - Timer_1_ReadCounter()) >= (FC_AUTO_NEUTRALIZE_TIME * 10)) && fcAutoNeutralizeOption)
        {
            rxCommandStartTime = Timer_1_ReadCounter();
            executeFlightCommand(FC_PITCH, 50);
            executeFlightCommand(FC_ROLL, 50);
            executeFlightCommand(FC_YAW, 25);
        }
        
        //Refresh GPS_Data
        if (gpsRxFlag == 1)
        {
            if (GPS_GetStringSimp(GPS_String) == 1)
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
            
            time0 = Timer_SW_ReadCounter(); //DEBUG
            downtime = (time1 - time0)-25/24; downtime_us = (downtime *24*125) /3000; //DEBUG
            time0 = Timer_SW_ReadCounter(); //DEBUG
            IMU_EulersRefresh();
            IMU_AccelRefresh();
            IMU_GetTemp();
            time1 = Timer_SW_ReadCounter(); //DEBUG
            timespan = (time0 - time1)-25/24; timespan_us[++filt_index] = (timespan *24*125) /3000; //DEBUG
            timespan_avg_us = avg(timespan_us,256);

            
            PPS_count = Counter_PPS_ReadCounter();
        }// End if refreshTimer
        
        if (refreshCounter >= REFRESH_DIVIDER)
        {
            refreshCounter = 0;
            
            //Write the comma seperated vector to save and send   
            sprintf(timingArray,"IMU: %li,%li,%li \tIMU_Read_Time: %lu us\tDownTime: %lu us \t\tIMU_AVG_Read = %lu us\r\n",
                getInt(PITCH,TYPENAME_BNO055_S16), getInt(ROLL,TYPENAME_BNO055_S16), getInt(YAW,TYPENAME_BNO055_U16),
                timespan_us[filt_index], downtime_us, timespan_avg_us);
                
            //Write the comma seperated vector to save and send   
            sprintf(dataArray,"%u,%li,%li,%li,%li,%li,%li,%li,%lu.%.2u,%li.%.2u,%lu.%.2u,%li.%.2u,",
                PPS_count,
                getInt(PITCH,TYPENAME_BNO055_S16),
                getInt(ROLL,TYPENAME_BNO055_S16),
                getInt(YAW,TYPENAME_BNO055_U16),
                getInt(XACCEL,TYPENAME_BNO055_S16),
                getInt(YACCEL,TYPENAME_BNO055_S16),
                getInt(ZACCEL,TYPENAME_BNO055_S16),
                getInt(IMUTEMP,TYPENAME_BNO055_T16),
                getInt(Altitude,TYPENAME_BMP280_U32),getDec(Altitude, TYPENAME_BMP280_U32),
                getInt(RelativeAltitude,TYPENAME_BMP280_S32),getDec(RelativeAltitude,TYPENAME_BMP280_S32),
                getInt(Pressure,TYPENAME_BMP280_U32),getDec(Pressure,TYPENAME_BMP280_U32),
                getInt(Temperature,TYPENAME_BMP280_S32),getDec(Temperature,TYPENAME_BMP280_S32));
            
            
            //Concatinate the gps data with the orientation data array
            static char gpsArray[128]="0,0,0,0,0,0,0,0,0"; //initiallize array so that number of tokens is consistant?
            static char  fcArray[128]="0,0,0,0,0,0,0,";
            if (GPS_RefreshData(GPS_GetString()))
            { 
                //sprintf(tempArray,"%lu.%u,%li",GGA.UTC/100,decimal(GGA.UTC,1),GGA.Latitude/100);  
                sprintf(gpsArray,"%.4lu.%.2u,%li.%.4u,%li.%.4u,%lu.%u,%lu.%u,%lu.%u,%u,%u,%c",
                    getInt(GGA.UTC,TYPENAME_GPS_UTC),getDec(GGA.UTC,TYPENAME_GPS_UTC),
                    getInt(GGA.Latitude,TYPENAME_GPS_DDD),getDec(GGA.Latitude,TYPENAME_GPS_DDD), 
                    getInt(GGA.Longitude,TYPENAME_GPS_DDD),getDec(GGA.Longitude,TYPENAME_GPS_DDD),
                    getInt(GGA.Altitude,TYPENAME_GPS_1D),getDec(GGA.Altitude,TYPENAME_GPS_1D),
                    getInt(RMC.Speed,TYPENAME_GPS_1D),getDec(RMC.Speed,TYPENAME_GPS_1D),
                    getInt(RMC.Direction,TYPENAME_GPS_1D),getDec(RMC.Direction,TYPENAME_GPS_1D),
                    GGA.nSatellites,
                    GGA.Fix_Quality,
                    RMC.Status);         
            }
            else
                strcpy(gpsArray,"0,0,0,0,0,0,0,0,0");
            UART_HC12_PutString(timingArray); //DEBUG
            //send it!
            //UART_HC12_PutString(dataArray);
            //UART_HC12_PutString(gpsArray);
            UART_HC12_PutString("\r\n\0");
            
            //Add control Data
            sprintf(fcArray,"%u,%u,%i,%u,%u,%i,%i,%i", FC._Roll, FC._Pitch, FC._Yaw, FC._Flap, FC._Throttle, FC._TrimRoll, FC._TrimPitch, FC._TrimYaw);
            strcat(dataArray,gpsArray); //Needs array size checking
            strcat(dataArray, fcArray); //NEEDS ARRAY SIZE CHECKING
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
                    //SD_Error();
                    pFile = 0;
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
            decimal = 0;
            break;
        case TYPENAME_GPS_DATE :
            decimal = 0;
            break;
            
        case TYPENAME_GPS_DMS :
            decimal = 0; 
            break;
            
        case TYPENAME_GPS_DMM :
            value_s = abs((int32) value);
            decimal = (value_s-(value_s/10000)*10000); 
            break;
            
        case TYPENAME_GPS_DDD :
            value_s = abs((int32) value);
            decimal = (value_s-(value_s/10000)*10000);
            break;
            
        case TYPENAME_GPS_1D :
            value_s = abs((int32) value);
            decimal = (value_s-(value_s/10)*10);
            break;
		
		case TYPENAME_GPS_2D :
            value_s = abs((int32) value);
            decimal = (value_s-(value_s/100)*100);
            break;
            
        case TYPENAME_BMP280_U32 :
            decimal = (value-(value/100)*100);
            break;
            
        case TYPENAME_BMP280_S32 :
            value_s = abs((int32) value);
            decimal = ((int32) value_s - ( (int32)value_s/100 )*100);
            break;    
            
        case TYPENAME_BNO055_S16:
            decimal = (value & 0x000F);
            break;   
            
        case TYPENAME_BNO055_U16:
            decimal = (value & 0x000F);
            break;      
            
        case TYPENAME_BNO055_T16:
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
    uint32 integer;
    int32 value_s;
    
    switch(type) 
    {
        case TYPENAME_GPS_UTC:
            integer = value;
            break;
        case TYPENAME_GPS_DATE :
            integer = 0;
            break;
            
        case TYPENAME_GPS_DMS :
            integer = 0; 
            break;
            
        case TYPENAME_GPS_DMM :
            value_s = (int32) value;
            integer = (value_s/10000); 
            break;
            
        case TYPENAME_GPS_DDD :
            value_s = (int32) value;
            integer = (value_s/10000);
            break;
            
        case TYPENAME_GPS_1D :
            value_s = (int32) value;
            integer = (value_s/10);
            break;
		
		case TYPENAME_GPS_2D :
            value_s = (int32) value;
            integer = (value_s/100);
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
            
        case TYPENAME_BNO055_T16:
            integer = (value >> 1);
            break;       
            
        default:
            integer = 0;
            break;
    }
    
    return integer;
}

uint32 avg(uint32 *array, uint32 arraysize)
{
    uint32 sum = 0;
    for(uint32 i=0; i<arraysize; ++i)
    {
        sum += array[i];
    }
    
    return sum/arraysize;
}


		  