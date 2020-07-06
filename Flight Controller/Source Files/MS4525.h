/* =========================================
Last edited 04/08/2020
 * "MS4525.h"
 *
 * Author:  Nathan Miller
 *
 * Version: 1.0
 *
 * Description:
 * HEADER FILE - Differential pressure sensor and for calculating airspeed.
 *
 *
 * Reasons for Revision:
 *    - 
 *    - 
 *    - 
 *
**/
 /* ======================================== */

#ifndef __MS4525_AIRSPEED_H__
#define __MS4525_AIRSPEED_H__

#include <project.h>
    

/**************************** DATA TYPES ***************************************/


/************************* GLOBAL VARIABLES ************************************/ 


/*********************** PSOC FUNCTION PROTOTYPES ****************************/ 
uint8 Airspeed_Start();
uint8 Airspeed_GetPressTemp(struct MS4525_data);
uint8 Airspeed_GetSpeed(struct MS4525_Data);

/*******************************************************************************/

#define MS4525_READ_BUFFER_SIZE 5
#define MS4525_PRESS_RESOLUTION (2^14 - 1) //16383
#define MS4525_TEMP_RESOLUTION	(2^11 - 1) //2047

typedef int32_t MS4525_e1_t; //x.x  * 10;
typedef int32_t MS4525_e2_t; //x.xx  * 100;
typedef int32_t MS4525_e3_t; //x.xxx  * 1000;
typedef int32_t MS4525_e4_t; //x.xxxx * 10000;

typedef struct MS4525_Data
{
	MS4525_Options MS4525_Options;
	uint16 		raw_pressure;
	uint16 		raw_temp;
	MS4525_e4_t psi;      //PSI *10000
	uint32	 	mPa; 
	MS4525_e1_t tempF;
	MS4525_e1_t tempC; 	
	MS4525_e2_t	knots;
	MS4525_e2_t	mph;
	MS4525_e2_t	mps;
} MS4525;


typedef struct MS4525_Options
{
	I2C_Address;
	Output_Type;
	Pressure_Range;
	Package_Type;
	Pressure_Type;
} MS4525_Options;

typedef struct MS4525_Options
{
	I2C_Address	   = INTERFACE_TYPE_I;
	Output_Type	   = OUTPUT_TYPE_A;
	Pressure_Range = PSI_001;
	Package_Type   = DUAL_SIDEPORT;
	Pressure_Type  = DIFFERENTIAL;
} MS4525_Options;


typedef enum
{
	INTERFACE_TYPE_I = 0x28,
	INTERFACE_TYPE_J = 0x36,
	INTERFACE_TYPE_K = 0x46,
	INTERFACE_TYPE_0 = 0x48,
	INTERFACE_TYPE_1 = 0x49,
	INTERFACE_TYPE_2 = 0x4A,
	INTERFACE_TYPE_3 = 0x4B,
	INTERFACE_TYPE_4 = 0x4C,
	INTERFACE_TYPE_5 = 0x4D,
	INTERFACE_TYPE_6 = 0x4E,
	INTERFACE_TYPE_7 = 0x4F,
	INTERFACE_TYPE_8 = 0x50,
	INTERFACE_TYPE_9 = 0x51
} MS4525_I2C_Address_t;

typedef enum
{
	OUTPUT_TYPE_A = 10; //10% to 90%
	OUTPUT_TYPE_B = 5;  //5%  to 95%
} MS4525_Output_Type_t;

typedef enum
{
	PSI_001 = 1,
	PSI_002 = 2,
	PSI_005 = 5,
	PSI_015 = 15,
	PSI_030 = 30,
	PSI_050 = 50,
	PSI_100 = 100,
	PSI_150 = 150
} MS4525_Pressure_Range_t;

typedef enum
{
	SINGLE_SIDEPORT, //SS
	DUAL_SIDEPORT,   //DS
	TOP_PORT,		 //TP
	MANIFOLD_MOUNT	 //MM
} MS4525_Package_Type_t;

typedef enum
{
	DIFFERENTIAL, 	//D
	GAUGE,			//G
	ABSOLUTE,		//A
	COMPOUND		//C
} MS4525_Pressure_Type_t;
    
#endif
