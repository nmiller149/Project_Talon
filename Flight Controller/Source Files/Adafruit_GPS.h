/* =========================================
Last edited 07/06/2019
 * "Adafruit_BMP280.h"
 *
 * Author:  Nathan Miller
 *
 * Version: 1.1
 *
 * Description:
 * HEADER FILE - Contains all files, functions, and variables used for the Bosch MTk33x9 GPS chipset implemented on the Adafruit Ultimate GPS Module
 *

 * Reasons for Revision:
 *    -  Adding more functionality to extract data from GPS NMEA strings and making library more 
 *    - 
 *    -
 *
**/
 /* ======================================== */
/***********************************
This is the Adafruit GPS library - the ultimate GPS library
for the ultimate GPS module!

Tested and works great with the Adafruit Ultimate GPS module
using MTK33x9 chipset
    ------> http://www.adafruit.com/products/746
Pick one up today at the Adafruit electronics shop 
and help support open source hardware & software! -ada

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above must be included in any redistribution
****************************************/

/********************************************
            Nathan Miller's EDIT
Most of the stuff from Adafruit's library has been erased
because it was not valuable for the PSOC. Only defined constants
could stay.

My own functions were added at the bottom of the defintions
*********************************************/

#ifndef _ADAFRUIT_GPS_H
#define _ADAFRUIT_GPS_H

#include <project.h>
#include <string.h>
#include <stdlib.h>

// different commands to set the update rate from once a second (1 Hz) to 10 times a second (10Hz)
// Note that these only control the rate at which the position is echoed, to actually speed up the
// position fix you must also send one of the position fix rate commands below too.
#define PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ  "$PMTK220,10000*2F" // Once every 10 seconds, 100 millihertz.
#define PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ  "$PMTK220,5000*1B"  // Once every 5 seconds, 200 millihertz.
#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
#define PMTK_SET_NMEA_UPDATE_2HZ  "$PMTK220,500*2B"
#define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C"
#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"
// Position fix update rate commands.
#define PMTK_API_SET_FIX_CTL_100_MILLIHERTZ  "$PMTK300,10000,0,0,0,0*2C" // Once every 10 seconds, 100 millihertz.
#define PMTK_API_SET_FIX_CTL_200_MILLIHERTZ  "$PMTK300,5000,0,0,0,0*18"  // Once every 5 seconds, 200 millihertz.
#define PMTK_API_SET_FIX_CTL_1HZ  "$PMTK300,1000,0,0,0,0*1C"
#define PMTK_API_SET_FIX_CTL_5HZ  "$PMTK300,200,0,0,0,0*2F"
// Can't fix position faster than 5 times a second!

//Be careful. So far have found that the thing is inoperable in any other buad rate
#define PMTK_SET_BAUD_57600 "$PMTK251,57600*2C\r\n"
#define PMTK_SET_BAUD_38400 "PMTK251,38400*03\r\n"
#define PMTK_SET_BAUD_19200 "PMTK251,19200*06\r\n"
#define PMTK_SET_BAUD_9600 "$PMTK251,9600*17\r\n"  

// turn on only the second sentence (GPRMC)
#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n"
// turn on GPRMC and GGA
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"
// turn on ALL THE DATA
#define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"
// turn off output
#define PMTK_SET_NMEA_OUTPUT_OFF "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"

// to generate your own sentences, check out the MTK command datasheet and use a checksum calculator
// such as the awesome http://www.hhhh.org/wiml/proj/nmeaxor.html

#define PMTK_LOCUS_STARTLOG  "$PMTK185,0*22"
#define PMTK_LOCUS_STOPLOG "$PMTK185,1*23"
#define PMTK_LOCUS_STARTSTOPACK "$PMTK001,185,3*3C"
#define PMTK_LOCUS_QUERY_STATUS "$PMTK183*38"
#define PMTK_LOCUS_ERASE_FLASH "$PMTK184,1*22"
#define LOCUS_OVERLAP 0
#define LOCUS_FULLSTOP 1

#define PMTK_ENABLE_SBAS "$PMTK313,1*2E"
#define PMTK_ENABLE_WAAS "$PMTK301,2*2E"

// standby command & boot successful message
#define PMTK_STANDBY "$PMTK161,0*28\r\n"
#define PMTK_STANDBY_SUCCESS "$PMTK001,161,3*36"  // Not needed currently
#define PMTK_AWAKE "$PMTK010,002*2D\r\n"

// ask for the release and version
#define PMTK_Q_RELEASE "$PMTK605*31"

// request for updates on antenna status 
#define PGCMD_ANTENNA "$PGCMD,33,1*6C" 
#define PGCMD_NOANTENNA "$PGCMD,33,0*6D" 

// how long to wait when we're looking for a response
#define MAXWAITSENTENCE 10
        

    
/**************** GLOBAL VARIABLES *************************/ 
char GPS_RMC_Data[100];
char GPS_GGA_Data[100];

extern uint8 NewGPSData;

/************** PSOC FUNCTION PROTOTYPES ******************/
void GPS_Start();
void GPS_Refresh(); 
char *GPS_GetString();
uint8 GPS_RefreshData(char *GPS_String);
/************************************************************/

typedef uint32 GPS_UTC_t; // hhmmss.ss * 100
typedef uint16 GPS_DATE_t;// ddmmyy
typedef int32 GPS_COORD_t;// ±dddmm.mm * 100 (DMM format)
typedef int32 GPS_1D_t;   // xxx.x * 10

struct GGA 
{ //*hh What to do with checkSum
      //$GPGGA,hhmmss.ss,ddmm.mm,a,dddmm.mm,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh
      //NMEA_Code, UTC, Latitude, NS, Longitude, EW, Quality, Satellites, Horizontal_Dilution, Altitude, Geoidal_Separation, Last_Update, Ref_Station_ID
    GPS_UTC_t UTC; 
    GPS_COORD_t Latitude, Longitude;
    uint8 Fix_Quality;//enum 0 ->8 (we want 1);
    uint8 nSatellites;
    GPS_1D_t Horizontal_Dilution, Altitude, Geoidal_Separation;
    uint16 Last_Update, Ref_Station_ID; //No Last_Update, No Ref_Station_ID (JUNK);
}GGA; 


struct RMC 
{ //*hh What to do with checkSum
      //GPRMC,hhmmss.ss,A,ddmm.mm,a,dddmm.mm,a,x.x,x.x,ddmmyy,x.x,a*hh
      //NMEA_Code, UTC, Status, Longitude, NS, Latitude, EW, Speed, Direction, Date, Magnetic_Variation, Magnetic_Variation_EW, Checksum
      //         UTC, data status, Lat, NS, Long, EW, Speed over ground (knots), Direction of Track (Degrees)(if display then good track??), UTC?, Mag Variation, EW_mag, checksum 
      GPS_UTC_t UTC;
      GPS_COORD_t Latitude, Longitude;
      GPS_1D_t Speed, Direction, Magnetic_Variation; //Speed is knots, Direction in degrees and True Course, Mag_Var in degrees
      GPS_DATE_t Date;         
}RMC;


#endif
