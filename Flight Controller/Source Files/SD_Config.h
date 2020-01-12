/* ========================================
Last edited 06/11/2019
 * "SD_Config.h"
 *
 * Author:  Nathan Miller
 *
 * Version: 1.0
 *
 * Description:
 * HEADER FILE - For configuration of SD Card using Sunfounder SD Card Module
 *
 *               
 * Reasons for Revision:
 *    - 
 *    -
 *
**/
 /* ======================================== */


#ifndef _SD_CONFIG_H
#define _SD_CONFIG_H
    
#include <FS.h>
#include <string.h>
#include <Global.h>
#include <stdio.h>
    
#include <project.h>

//Constants
#define EEPROM_FILE_NUMBER_ADDR 0x1A
#define TEMP_FILE_NAME "TEMPDATA.TXT"
#define FLIGHT_DATA_FILE_PATH "\\SSASPIRE\\FLIGHT"

//Variables
FS_FILE * pFile;
char sdVolName[12];     /* Buffer that will hold SD card Volume name */  
    
//Functions
void Start_SD_Card();
void Close_SD_Card();

#endif
/* [] END OF FILE */
