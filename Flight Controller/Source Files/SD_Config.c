/* ========================================
Last edited 06/11/2019
 * "SD_Config.c"
 *
 * Author:  Nathan Miller
 *
 * Version: 1.0
 *
 * Description:
 * SOURCE FILE - For configuration of SD Card using Sunfounder SD Card Module
 *
 *               
 * Reasons for Revision:
 *    - 
 *    -
 *
**/
 /* ======================================== */

#include "SD_Config.h"

uint8 fileNumber;

void Start_SD_Card(){
    
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
     
    FS_Mount(&sdVolName[0]);
     
    /* Create specific file for modification */
    pFile = FS_FOpen(TEMP_FILE_NAME, "a");
    /*
    r -for reading
    w -Truncate to zero length or create file for writing
    "a"	-Append; open and create file for writing at end-of-file
    "r+ or rb+ or r+b"	Open file for update (reading and writing)
    "w+ or wb+ or w+b"	Truncate to zero length or create file for update
    "a+ or ab+ or a+b"	Append; open or create for update, writing at end-of-file
    */    

    //Get File Number and increment it in eeprom
    fileNumber = EEPROM_ReadByte(EEPROM_FILE_NUMBER_ADDR);
    if (pFile){
        EEPROM_WriteByte((fileNumber + 1), EEPROM_FILE_NUMBER_ADDR); 
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
    sprintf(NewDestination,"%s%u.csv",FLIGHT_DATA_FILE_PATH,fileNumber);
    if(0 == FS_CopyFile(TEMP_FILE_NAME,NewDestination))
    {
    }
    else
    {
    }
    
    FS_SetFileTime(TEMP_FILE_NAME,  0x5ADA0420);  //USE ACTUAL TIME!!!!!!!!!!!!!!!
    FS_SetFileTime(NewDestination,  0x5ADA0420);  
    FS_Unmount (&sdVolName[0]); 

    return;    
}