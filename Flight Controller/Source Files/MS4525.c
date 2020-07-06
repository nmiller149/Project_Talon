/* =========================================
Last edited 04/09/2020
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


#include "MS4525.h"


/*=========================================================================
                       PRIVATE FUNCTIONS
=========================================================================*/
const int32_t LOWER_RANGE_COUNTS = (MS4525_PRESSURE_RESOLUTION*MS4525_Output_Type/100);
const int32_t upper_range_counts = (MS4525_PRESSURE_RESOLUTION - MS4545_LOWER_RANGE_COUNTS);
const int32_t range_span;

/********************************************************************
                    readMS4525
 
********************************************************************/
static uint8 *readMS4525(uint8 chipAddr, uint8 bytes) 
{
   static uint8 tmp[MS4525_RX_BUFFER_SIZE];
   
    //Send the module the address of the register to read from
    I2C_Orientation_MasterSendStart(chipAddr, WRITE);
    //I2C_Orientation_MasterWriteByte(regAddr);
    //I2C_Orientation_MasterSendStop();
    //I2C_Orientation_MasterSendStart(chipAddr, READ);
     
    //Read in data from next i registers (it will spit out until a NAck)
    uint8 i;
    for (i=0; i < bytes-1; i++)
    {
      tmp[i] = I2C_Orientation_MasterReadByte(I2C_Orientation_ACK_DATA);
    }
    
    tmp[i] = I2C_Orientation_MasterReadByte(I2C_Orientation_NAK_DATA); //Send a No acknowledg to end data read
    I2C_Orientation_MasterSendStop(); //send stop vs NAK? !!!
    
   return tmp;
    
}


uint8 BNO055_Error()
{
    
    return 1;   
}


/*=========================================================================
                       PUBLIC FUNCTIONS
=========================================================================*/

/********************************************************************
                    Airspeed_Start()
 
********************************************************************/
uint8 Airspeed_Start(){
    //TO DO: Check implicit start functions for error and return status
    
    Timer_1_Start();
    I2C_Orientation_Start();
    CyDelay(300);   

   return 1;
}
    
    

/********************************************************************
                    Airspeed_GetPressTemp()
 
********************************************************************/
uint8 Airspeed_GetPressTemp(struct MS4525_data MS4525){
    
	
    uint8 * tmp = readMS4525(MS4525_I2C_Address, 4);
    
    
    MS4525.raw_pressure = tmp[0]<<8 | tmp[1];
    MS4525.raw_temp		= tmp[2]<<8 | tmp[3];
	
	//MS4525.mPSI = (( (Pmax-Pmin) * 1000* (100 * MS4525.raw_pressure - MS4525_PRESS_RESOLUTION*OT)) / ((MS4525_PRESSURE_RESOLUTION*(100-2*OT))/10)) - 10000;
	MS4525.psi = ((200 * (100 * MS4525.raw_pressure - 163830)) / 13106) - 10000;
	
	MS4525.mPa  = (MS4525.psi * 68948) / 10;
	
	MS4525.tempC = MS4525.raw_temp * 2000 / 2047 - 500;
	MS4525.tempF = (MS4525.tempC * 9)/5 +32;

    return 1;
}
    



/********************************************************************
                    Airspeed_GetSpeed()
 
********************************************************************/
uint8 void Airspeed_GetSpeed(struct MS4525_data, uint32_t baro_pressure){
        
    // calculate airspeed based on pressure, altitude and temperature
    // airspeed = sqr(2 * differential_pressure / air_mass_per_kg) ; air mass per kg = pressure  pa / (287.05 * (Temp celcius + 273.15))
    // so airspeed m/sec =sqr( 574.1 * differential_pressure pa * (temperature Celsius + 273.15) / pressure pa )
	// rawAirSpeed =  sqrt( (float) (574 * 10520.56427 * abs(smoothDifPressureAdc) * temperature4525  /  actualPressure) ); // in cm/sec ; actual pressure must be in pa (so 101325 about at sea level)
    MS4525.mps =  (24574 * sqrt((abs(MS4525.mPa) * MS4525.tempC) /  baro_pressure) )/10; // in cm/sec ; actual pressure must be in pa (so 101325 about at sea level)
    MS4525.mph = (MS4525.mps * 2290)>>10; //*2.2369 mph/mps
	MS4525.knots = (MS4525.mph * 11)/10;
    return;
    
}


/* [] END OF FILE */
