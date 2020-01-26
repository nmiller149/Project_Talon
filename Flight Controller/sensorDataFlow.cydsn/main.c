/* ========================================
Last edited 01/12/2020
 * File Name: "main.c" 
 * Project  : sensorDataFlow
 *
 * Author:  Nathan Miller
 *
 * Version: 1.2
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
 *    - [1.2] Counter_Duration is now Counter_PPS
 *    - 
 *    -
 *
 *
**//* ======================================== */

#ifndef PROJECT_H
#define PROJECT_H
#include <project.h>
#endif

//Included files for writing to SD Card
#include <FS.h>
#include <string.h>
#include <Global.h>
#include <stdio.h>

#include "Adafruit_BNO055.h"
#include "Adafruit_GPS.h"
#include "Adafruit_BMP280.h"


#define DEFAULT_PRIORITY                    (3u)
#define HIGHER_PRIORITY                     (2u)



//PROTOTYPES
void Start_SD_Card();
void Close_SD_Card();



//GLOBAL VARIABLES
char sdFile[13] = "TEMPDATA.TXT";
FS_FILE * pFile;
char sdVolName[12];     /* Buffer that will hold SD card Volume name */  

uint8 fileNumber;



CY_ISR_PROTO(GPS_Handler);



int main(void)
{
    
    
    //=========================== Initialization =====================================
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    //isr_GPS_StartEx(GPS_Handler);
    //isr_GPS_SetPriority(DEFAULT_PRIORITY);
      
    Timer_1_Start();
    Counter_PPS_Start();
    
    GPS_Start();
    UART_HC12_Start();

    Stabilizer_Start();
    Aileron_Start();
    Motor_Speed_Start();
    
    VDAC8_1_Start();
    SPI_Altitude_Start();
    
    
    //Start Accel/Gyro/Mag sensor
    AGM_Start();

    EEPROM_Start();
       
    fileNumber = EEPROM_ReadByte(0x1A); //gets the next file number from eeprom

    //Starts and configures emFile and file system
    Start_SD_Card();  
    //===========================================================================================
      
    
    
    //duration,X,Y,Z,Yaw,Roll,Pitch,TEMP,$GPS,,,,,,,,,,,, 

    uint16  refreshTime = 10  ,  timerPeriod = 1000;  //mS
    uint duration = 0;
    uint8 endLoop = 0;
    char dataArray[300]; 
    
    for(;;)
    {
      
        if (Timer_1_ReadCounter() <= (timerPeriod - refreshTime))
        {
            
            AGM_EulersRefresh();
            AGM_AccelRefresh();
            AGM_GetTemp();
            GPS_Refresh();
            
            duration = Counter_PPS_ReadCounter();
            
            
            //Write the comma seperated vector to save and send   
            sprintf(dataArray,"%u,%i,%i,%i,%i,%i,%i,%i,",duration,PITCH,ROLL,YAW,XACCEL,YACCEL,ZACCEL,AGMTEMP);
            
            
            //Concatinate the gps data with the orientation data array
            if(NewGPSData == 1){
                strcat(dataArray,GPS_RMC_Data);
                NewGPSData = 2;
            }
            
            else if(NewGPSData == 3){
                strcat(dataArray,GPS_GGA_Data);
                NewGPSData = 0;
            }
            
            
            strcat(dataArray,"\r\n");
            
            
            
            //send it!
            UART_HC12_PutString(dataArray);
                   
            
            
            
            
            //Save it!
            if(pFile)//Checks if file was created
            {
                if(0 != FS_Write(pFile, &dataArray, strlen(dataArray))) 
                {
                    /* Indicate that data was written to a file */
                }
                else
                {

                }
            }
            
            
            
            //Nullify the data Array
            for (U16 i = 100; i < 300; ++i){
                dataArray[i] = '\0';
            }
            
            
            
            //Reset the timer
            Timer_1_WriteCounter(timerPeriod);
        }//End if 
        
        
        
        
        //Turn off program to save the file
        if (Button_Read() == 0){
            
            int start = Counter_PPS_ReadCounter();
            int current;
            
            while(Button_Read() == 0){                
                current = Counter_PPS_ReadCounter();
                
                if ( (current - start) >= 2){
                    LED_Write(1u);
                    endLoop = 1;
                    break;
                }
            }
        }
        
        if (endLoop ==1){
            break;    
        }
        
    } //End for(;;) loop   
    
    
    //ShutDown Operation
    Close_SD_Card();
    
    while(1){
        CyDelay(100);
        LED_Write(0);
        CyDelay(100);
        LED_Write(1);
    }
    
}/* [] END OF MAIN  */









/*========================= FUNCTION  DEFINITIONS ============================*/
//    -------------------------------------------------------------------




//-------------------- SD Card Functions ----------------------------------

void Start_SD_Card(){
    // set  'char sdFile[15];' as a global
    // set 'FS_FILE * pFile;' as a global too
    // set char sdVolName[12];    as a global
    // fileNumber as a global
   
    
    
    /* Initialize file system */
    FS_Init(); 
    CyDelay(250);
    
    

    /* Get volume name of SD card #0 */
    if(0 != FS_GetVolumeName(0u, &sdVolName[0], 12u))
    {
        //CAN PUT SOMETHING HERE IF VOLUME NAME IS SUCCESFULLY FOUND
    }
    
    else
    {
        //PUT SOMETHING HERE IF VOLUME IS NOT SUCCESFULLY FOUND
    }
     
    
    FS_Mount (&sdVolName[0]);
    
       
    
    
    /* Create specific file for modification */
    pFile = FS_FOpen(sdFile, "a");/*
    r -for reading
    w -Truncate to zero length or create file for writing
    "a"	-Append; open and create file for writing at end-of-file
    "r+ or rb+ or r+b"	Open file for update (reading and writing)
    "w+ or wb+ or w+b"	Truncate to zero length or create file for update
    "a+ or ab+ or a+b"	Append; open or create for update, writing at end-of-file
    */    
   
    
    
    
    if (pFile){
        EEPROM_WriteByte((fileNumber + 1), 0x1A);   // increments, in eeprom, the file number for next use     
    }
    return;


}




void Close_SD_Card(){
    //close file
    if(0 == FS_FClose(pFile))
    {
    }
    else
    {
    }
    
    
  
    
    char NewDestination[32];
    //Copy a file to a previously created directory
    sprintf(NewDestination,"\\SSASPIRE\\FLIGHT%u.csv",fileNumber);
    if(0 == FS_CopyFile(sdFile,NewDestination))
    {
    }
    else
    {
    }
    FS_SetFileTime(sdFile,  0x5ADA0420);
    FS_SetFileTime(NewDestination,  0x5ADA0420);
    
    FS_Unmount (&sdVolName[0]); 
    
    
    return;    
}
















CY_ISR(GPS_Handler)
{

}