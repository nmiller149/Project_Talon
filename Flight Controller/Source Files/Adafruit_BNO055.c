/* =========================================
Last edited 03/15/2020
 * "Adafruit_BNO055.c"
 *
 * Author:  Nathan Miller
 *
 * Version: 3.2
 *
 * Description:
 * HEADER FILE - Contains all files, functions, and variables used for the BNO055 9-DOF sensor
 *               which utilizes accelerometers, gyros, and magnetometers (IMU)
 *
 * Reasons for Revision:
 *    - 06/16/2019 - Completed Build = Fully functional now
 *    - 03/15/2020 - Renamed function calls from AGM to IMU. Reconstructed functions to call readBNO055 and writeBNO055 functions to isolate the read functionallity for more flexible.
 *    - 05/14/2020 - IMU temp wrong conversion. Converts from C to F properly now with compensation for fixed point.
 *    - 05/20/2020 - TYPENAME_BNO055_U8 changed to T16 for IMUTEMP to carry negative F values and values greater than 63.5 F.
 *
**/
 /* ======================================== */


#include "Adafruit_BNO055.h"


//------------------------- Private Functions -------------------------------------
//---------------------------------------------------------------------------------

//static _Bool writeBNO055(uint8 chipAddr, uint8 regAddr, uint8 dataBytes, uint8 *Data);


//------------------------------ readBNO055 ------------------------------------
static uint8 *readBNO055(uint8 chipAddr, uint8 regAddr, uint8 bytes)
{
   static uint8 tmp[8];
    //Send the module the address of the register to read from
    I2C_Orientation_MasterSendStart(chipAddr, WRITE);
    I2C_Orientation_MasterWriteByte(regAddr);
    //I2C_Orientation_MasterSendStop();

    I2C_Orientation_MasterSendRestart(chipAddr, READ);
     
    //Read in data from next i registers (it will spit out until a NAck)
    uint8 i;
    for (i=0; i < bytes-1; i++)
    {
      tmp[i] = I2C_Orientation_MasterReadByte(I2C_Orientation_ACK_DATA);
    }
    
    tmp[i] = I2C_Orientation_MasterReadByte(I2C_Orientation_NAK_DATA); //Send a No acknowledg to end data read
    I2C_Orientation_MasterSendStop(); //send stop vs NAK? !!!
    
   return tmp;

    //------------------- ALTERNATE METHOD ------------------------
    /*
    I2C_Orientation_MasterSendStart(chipAddr, WRITE);
    I2C_Orientation_MasterWriteByte(regAddr);
    I2C_Orientation_MasterSendStop();
    I2C_Orientation_MasterReadBuf(chipAddr, tmp, bytes, I2C_Orientation_MODE_COMPLETE_XFER | I2C_Orientation_MODE_NO_STOP);
    return tmp;
    */
}


uint8 BNO055_Error()
{
    
    return 1;   
}


//------------------------- Public Functions -------------------------------------
//---------------------------------------------------------------------------------


//-------------------Orientation (ACCEL/GYRO/MAG) Functions --------------------
void IMU_Start(){
    //Starts and configures the Accel/Gyro/Mag Sensor
    
    Timer_1_Start();
    I2C_Orientation_Start();
    CyDelay(300);
   

    
    /*while (I2C_Orientation_MSTAT_RD_CMPLT != I2C_Orientation_MasterStatus())
    {
    }*/
    
    //BNO055_ADDRESS_A;
    //adafruit_bno055_reg_t           Reg    = 2;
    //adafruit_bno055_powermode_t     Power  = POWER_MODE_NORMAL;
    //adafruit_bno055_opmode_t        OPMODE = OPERATION_MODE_CONFIG;
    
    //static uint8 DataWrite;
    //uint8* DataAddr = &DataWrite;
    
    
    
    //Set to configuration mode to change settings
    Write_IMU_Config(BNO055_OPR_MODE_ADDR, OPERATION_MODE_CONFIG);
    
    /* Set to normal power mode */
    Write_IMU_Config(BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL);

    /* What does this do again? */
    Write_IMU_Config(BNO055_PAGE_ID_ADDR, 0);
  
    
    /* Set the output units */
    uint8_t unitsel = (0 << 7) | // Orientation:    0 = Android  1 = Windows
                      (1 << 4) | // Temperature:    0 = Celcius  1 = Farenheit
                      (0 << 2) | // Euler:          0 = Degrees  1 = Radians
                      (0 << 1) | // Gyro:           0 = Dps      1 = Rad/s
                      (0 << 0);  // Accelerometer   0 = m/s^2    1 = mg
    
    Write_IMU_Config(BNO055_UNIT_SEL_ADDR, unitsel); 

    // Configure axis mapping (see section 3.4)
    Write_IMU_Config(BNO055_AXIS_MAP_CONFIG_ADDR, REMAP_CONFIG_P0); // P0-P7, Default is P1


    Write_IMU_Config(BNO055_SYS_TRIGGER_ADDR, 0x0); //WHAT DOES THIS DO??
    /* Set the requested operating mode (see section 3.3) */
  
      
    //NDOF Fusion Mode. Check Euler registers or Quaternion 
   Write_IMU_Config(BNO055_OPR_MODE_ADDR, OPERATION_MODE_NDOF); //OPERATION_MODE_IMUPLUS

    
    /* -------------------- CALIBRATION -----------------------*/
    uint8 CALIB=0;
    uint16 tempTime = 0; //100us
    Timer_1_WriteCounter(5000);//500ms
    
    while (CALIB <= 0b00010101){//FIX
        //Send the module the address of the register to read from
        I2C_Orientation_MasterSendStart(BNO055_ADDRESS_A, WRITE);
        I2C_Orientation_MasterWriteByte(BNO055_CALIB_STAT_ADDR);
        I2C_Orientation_MasterSendStop();
        
        //Read in data  from the next 6 registers (it will spit out until a NAck)
        I2C_Orientation_MasterSendStart(BNO055_ADDRESS_A, READ);
        CALIB = I2C_Orientation_MasterReadByte(I2C_Orientation_NAK_DATA);
        I2C_Orientation_MasterSendStop();
    
        tempTime = Timer_1_ReadCounter();
        if (tempTime >= 2500) //250ms
        {
            LED_Write(1);
        }
        else
        {
            LED_Write(0);
        }
    
    }
    LED_Write(0u);
    //Timer_1_WritePeriod(0xFFFF);
    
    //-----------------------------------------------------------------
    
    

   return;
}
    
    
    




void Write_IMU_Config(uint8 RegisterAdrress, uint8 DataByte){
    //Check if error by checking passed value from master write byte
    //and master start send with defined value 'I2C_Orientation_MSTR_NO_ERROR'
    I2C_Orientation_MasterSendStart(BNO055_ADDRESS_A, WRITE);
    I2C_Orientation_MasterWriteByte(RegisterAdrress);
    I2C_Orientation_MasterWriteByte(DataByte);
    I2C_Orientation_MasterSendStop();
    
    CyDelay(50);
    
    return;
}
    








 


void IMU_EulersRefresh(){
    //Get Eulers Angles from Registers of module
       
    /* --------------- Euler Angles ----------------------------- */
    uint8 * tmp = readBNO055(BNO055_ADDRESS_A, BNO055_EULER_H_LSB_ADDR, 6);
    
    
    YAW = tmp[0];
    YAW = YAW + ( tmp[1] << 8 );

    ROLL = tmp[2];
    ROLL = ROLL + ( tmp[3] << 8 );
    
    PITCH = tmp[4];
    PITCH = PITCH + ( tmp[5] << 8 );
    
   

    YAW+=(180<<4); //shifted by 180 degrees
    if(YAW >= (360<<4)) YAW-=(360<<4); //compensate max of 360 for shift to work
    
    
    /* decimal 16 = 1 deg
    ROLL = ROLL                       %-180 --> +180   ccw --> cw   *depends on format
    PITCH = PITCH                     %-90  --> +90   down --> up
    YAW = HEADING                     % 0   --> 360  compass heading
    
    */

    return;
}
    



void IMU_AccelRefresh(){
    //Get the linear Acceleration of the chip
     
    /* --------------------LINEAR ACCELERATION-----------------------*/
    uint8 * tmp = readBNO055(BNO055_ADDRESS_A, BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR, 6);

    XACCEL = tmp[0];
    XACCEL = XACCEL + ( tmp[1] << 8 );

    YACCEL = tmp[2];
    YACCEL = YACCEL + ( tmp[3] << 8 );
    
    ZACCEL = tmp[4];
    ZACCEL = ZACCEL + ( tmp[5] << 8 ); //Send a No acknowledg to end data read
        
    return;
}



void IMU_GetTemp(){
        
    
    /* --------------------ACCELEROMETER TEMPERATURE-----------------------*/
    uint8 * tmp = readBNO055(BNO055_ADDRESS_A, BNO055_TEMP_ADDR, 1);
    
    IMUTEMP = *tmp;

    //Fahrenheit conversion with left shift compensation
    IMUTEMP = (IMUTEMP * 9)/5 + 64;
    
    return;
    
}






/* [] END OF FILE */
