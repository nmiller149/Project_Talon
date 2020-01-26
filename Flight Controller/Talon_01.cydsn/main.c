/* ========================================
Last edited 01/07/2020
 * File Name: "main.c" 
 * Project  : Talon_01
 *
 * Author:  Nathan Miller
 *
 * Version: 1.2.1
 *
 * Description:
 *    Integration Testing. Starting with integration of HC12 project and 
 *    Comm Motor Control Test to test wireless control. Then integration of
 *    sensorDataFlow to test all sensors are connected properly and data is
 *    received at home base properly. Next, sensorDataFlow should be developed
 *    the rest of the way to include other  sensors. Some optimization should
 *    occur to clean code up. A mostly working and integrated project will be
 *    produced as Talon_02 version 2.0.
 *    
 *
 * TODO: Break GPS into useful variables and send data back with only a few important
 *      features, or send back starting characters to decipher what kind of senetence is being sent
 *      (May need both, soon debugging will need simplified and debug/status sentence to know which 
 *      sensors are available and what state the plane is in will be needed)
 *    
 *
 * Reasons for Revision:
 *    - This File includes succesful integration of HC12 and CommMotorControlTest along
 *      with intitial integration of sensorDataFlow with a working pressure sensor. 
 *    - Add more Data to output string and changed GPS string functions
 *    - Changed FC_Initialization function to use enum names instead of numbers
 *
 *
**/
/* ======================================== */




#include <project.h>

#include "Adafruit_BNO055.h"
#include "Adafruit_GPS.h"
#include "Adafruit_BMP280.h"
#include "SD_Config.h"
#include "Flight_Controller.h"

#include "hc12_Config.h"
#include "hc12_Comm.h"


#define DEFAULT_PRIORITY                    (3u)
#define HIGHER_PRIORITY                     (2u)

//---------- USER INPUTS --------------
#define SENSOR_UPDATE_RATE    30 //Hz
#define REFRESH_DIVIDER       8  //30Hz ÷ 4 = 7Hz
uint8 commChannel = 30;
//-------------------------------------


void HC_Initialize(void)
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
int32 decimal(int32 value, uint8 type);

BMP280_U32_t Altitude_Initialize(void)
{
    BMP280_U32_t altTemp, initTemp;
    for (uint8 i=0; i<7; i++)
    {
        altTemp += *readAltitude(0);
    }
    initTemp = altTemp / 7;
    
    return initTemp;
}


CY_ISR_PROTO(refreshHandler);
_Bool refreshTimerFlag =0;


int main(void)
{
  
    //=========================== Initialization =====================================
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    Timer_1_Start(); //Redundant
    Timer_2_Start();
    Timer_2_WritePeriod(10000/SENSOR_UPDATE_RATE); // 10,000kHz / 30 Hz = Period
    isr_Refresh_StartEx(refreshHandler);
    
    HC_Start();
    HC_Comm_Init();
    HC_Initialize();
    FC_Start();
    FC_Initialize();
    
    beginBMP280();
    BMP280_S32_t InitAltitude = Altitude_Initialize();
    AGM_Start();
    GPS_Start();
    EEPROM_Start();      
    Start_SD_Card(); 
    //===========================================================================================

    uint8 refreshCounter = 0;
    uint duration = 0;
    char dataArray[512];
    
    BMP280_U32_t Altitude, Pressure, RelativeAltitude;
    BMP280_S32_t Temperature;
    for(;;)
    {   
        if (rxCommandFlag == 1)
        {
            rxCommandFlag=0;
            executeFlightCommand(FC_RxString[0],FC_RxString[1]);      
        }    
        
        //Refresh Sensors and Update
        if (refreshTimerFlag ==1)
        {
            refreshTimerFlag = 0;
            refreshCounter++;
            
            BMP280_U32_t *altPtr = readAltitude(0);
            Altitude  = altPtr[0]; RelativeAltitude = Altitude - InitAltitude;
            Pressure  = altPtr[1];
            Temperature = altPtr[2];
            Temperature = (Temperature *9)/5 + 3200; //Farenheit conversion
            
            AGM_EulersRefresh();
            AGM_AccelRefresh();
            AGM_GetTemp();
            //GPS_Refresh();
            
            duration = Counter_PPS_ReadCounter();
        }
        if (refreshCounter >= REFRESH_DIVIDER)
        {
            refreshCounter = 0;
         
            //Write the comma seperated vector to save and send   
            sprintf(dataArray,"%u,%i,%i,%i,%i,%i,%i,%i,%lu.%u,%li.%u,%lu.%u,%li.%u,",duration,PITCH>>4,ROLL>>4,YAW>>4,XACCEL,YACCEL,ZACCEL,AGMTEMP,Altitude/100,
                (uint8)(Altitude-(Altitude/100)*100),RelativeAltitude/100,(uint8)(RelativeAltitude-(RelativeAltitude/100)*100),Pressure/100,(uint8)(Pressure-(Pressure/100)*100),Temperature/100,(uint8)(Temperature-(Temperature/100)*100));
            
            
            //Concatinate the gps data with the orientation data array
            static char tempArray1[128]="0,0,0,0,0,0,0,";
            static char tempArray2[128]="0,0,0,0,0,0,0,";
            if (GPS_RefreshData(GPS_GetString()))
            { 
               //sprintf(tempArray,"%lu.%u,%li",GGA.UTC/100,decimal(GGA.UTC,1),GGA.Latitude/100);  
               sprintf(tempArray1,"%lu.%u,%li.%u,%li.%u,%lu.%u,%lu.%u,%lu.%u,%u,",GGA.UTC/100,(uint8)(GGA.UTC-GGA.UTC/100),GGA.Latitude/100,(uint8)(GGA.Latitude-GGA.Latitude/100), GGA.Longitude/100,(uint8)(GGA.Longitude-GGA.Longitude/100),GGA.Altitude/10,(uint8)(GGA.Altitude-GGA.Altitude/10),RMC.Speed/10,(uint8)(RMC.Speed-RMC.Speed/10),RMC.Direction/10,(uint8)(RMC.Direction-RMC.Direction/10),GGA.nSatellites);         
            }
            strcat(dataArray, tempArray1);

            //send it!
            UART_HC12_PutString(dataArray);
            UART_HC12_PutString("\r\n\0");
            
            //Add control Data
            sprintf(tempArray2,"%u,%u,%i,%u,%u,%i,%i,%i",FC._Roll, FC._Pitch, FC._Yaw, FC._Flap, FC._Throttle, FC._TrimRoll, FC._TrimPitch, FC._TrimYaw);
            strcat(dataArray, tempArray2);
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
            }
            
        }
      
    }    
    
}/* [] END OF MAIN  */




CY_ISR(refreshHandler)
{

    /* Clear pending Interrupt */
    isr_HC12_Rx_ClearPending();
    refreshTimerFlag = 1;    
    
}

int32 decimal(int32 value, uint8 type)
{
    //typedef uint32 GPS_UTC_t; // hhmmss.ss * 100
    //typedef uint16 GPS_DATE_t;// ddmmyy
    //typedef int32 GPS_COORD_t;// ±dddmm.mm * 100 (DMM format)
    //typedef int32 GPS_1D_t;   // xxx.x * 10
    
}