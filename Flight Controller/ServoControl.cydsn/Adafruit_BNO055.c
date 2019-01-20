/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "Adafruit_BNO055.h"


//-------------------Orientation (ACCEL/GYRO/MAG) Functions --------------------


void AGM_Start(){
    //Starts and configures the Accel/Gyro/Mag Sensor
    
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
    
    
    
    //Set to configuration mode
    Write_AGM_Config(BNO055_OPR_MODE_ADDR , OPERATION_MODE_CONFIG);
    
    /* Set to normal power mode */
    Write_AGM_Config(BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL);

    


    Write_AGM_Config(BNO055_PAGE_ID_ADDR, 0);
  
    
    /* Set the output units */
    uint8_t unitsel = (0 << 7) | // Orientation:    0 = Android  1 = Windows
                      (1 << 4) | // Temperature:    0 = Celcius  1 = Farenheit
                      (1 << 2) | // Euler:          0 = Degrees  1 = Radians
                      (1 << 1) | // Gyro:           0 = Dps      1 = Rad/s
                      (0 << 0);  // Accelerometer   0 = m/s^2    1 = mg
    
    Write_AGM_Config(BNO055_UNIT_SEL_ADDR, unitsel); 
  


    // Configure axis mapping (see section 3.4)
    Write_AGM_Config(BNO055_AXIS_MAP_CONFIG_ADDR, REMAP_CONFIG_P0); // P0-P7, Default is P1


    Write_AGM_Config(BNO055_SYS_TRIGGER_ADDR, 0x0); //WHAT DOES THIS DO??
    /* Set the requested operating mode (see section 3.3) */
  
      
    //NDOF Fusion Mode. Check Euler registers or Quaternion 
   Write_AGM_Config(BNO055_OPR_MODE_ADDR, OPERATION_MODE_NDOF);

    
    /* -------------------- CALIBRATION -----------------------*/
    uint8 CALIB=0  , temp = 0;
    Timer_1_WritePeriod(500);
    
    while (CALIB <= 0b00010101){//FIX
        //Send the module the address of the register to read from
        I2C_Orientation_MasterSendStart(BNO055_ADDRESS_A, WRITE);
        I2C_Orientation_MasterWriteByte(BNO055_CALIB_STAT_ADDR);
        I2C_Orientation_MasterSendStop();
        
        //Read in data  from the next 6 registers (it will spit out until a NAck)
        I2C_Orientation_MasterSendStart(BNO055_ADDRESS_A, READ);
        CALIB = I2C_Orientation_MasterReadByte(I2C_Orientation_NAK_DATA);
        I2C_Orientation_MasterSendStop();
    
        temp = Timer_1_ReadCounter();
        if (temp >= 250)
        {
            LED_Write(1);
        }
        else
        {
            LED_Write(0);
        }
    
    }
    LED_Write(0u);
    Timer_1_WritePeriod(1000);
    
    //-----------------------------------------------------------------
    
    

   return;
}
    
    
    




void Write_AGM_Config(uint8 RegisterAdrress, uint8 DataByte){
    //Check if error by checking passed value from master write byte
    //and master start send with defined value 'I2C_Orientation_MSTR_NO_ERROR'
    
    I2C_Orientation_MasterSendStart(BNO055_ADDRESS_A, WRITE);
    I2C_Orientation_MasterWriteByte(RegisterAdrress);
    I2C_Orientation_MasterWriteByte(DataByte);
    I2C_Orientation_MasterSendStop();
    
    CyDelay(50);
    
    return;
}
    








 


void AGM_EulersRefresh(){
    //Get Eulers Angles from Registers of module
       
    /* --------------- Euler Angles ----------------------------- */
    //Send the module the address of the register to read from
    I2C_Orientation_MasterSendStart(BNO055_ADDRESS_A, WRITE);
    I2C_Orientation_MasterWriteByte(BNO055_EULER_H_LSB_ADDR);
    I2C_Orientation_MasterSendStop();
    
    
    //Read in data  from the next 6 registers (it will spit out until a NAck)
    I2C_Orientation_MasterSendStart(BNO055_ADDRESS_A, READ);
    YAW = I2C_Orientation_MasterReadByte(I2C_Orientation_ACK_DATA);
    YAW = YAW + ( I2C_Orientation_MasterReadByte(I2C_Orientation_ACK_DATA) << 8 );

    ROLL = I2C_Orientation_MasterReadByte(I2C_Orientation_ACK_DATA);
    ROLL = ROLL + ( I2C_Orientation_MasterReadByte(I2C_Orientation_ACK_DATA) << 8 );
    
    PITCH = I2C_Orientation_MasterReadByte(I2C_Orientation_ACK_DATA);
    PITCH = PITCH + ( I2C_Orientation_MasterReadByte(I2C_Orientation_NAK_DATA) << 8 ); //Send a No acknowledg to end data read
    
    I2C_Orientation_MasterSendStop();
    
      
    YAW = (YAW * 18000) / 2827;
    ROLL = (ROLL * 18000) / 2827;
    PITCH= (PITCH * 18000) / 2827;
    
    //------------------- ALTERNATE METHOD ------------------------
    // Probably doesn't work. Have no idea how MasterReadBuf is supposed to work
    /*
    uint16 ORIENTATION[7];
    
    I2C_Orientation_MasterSendStart(BNO055_ADDRESS_A, WRITE);
    I2C_Orientation_MasterWriteByte(BNO055_EULER_H_LSB_ADDR);
    I2C_Orientation_MasterSendStop();

    I2C_Orientation_MasterReadBuf(BNO055_ADDRESS_A, ORIENTATION, 6, I2C_Orientation_MODE_COMPLETE_XFER);
    */

    return;
}
    






void AGM_AccelRefresh(){
    //Get the linear Acceleration of the chip
        
    
    /* --------------------LINEAR ACCELERATION-----------------------*/
    //Send the module the address of the register to read from
    I2C_Orientation_MasterSendStart(BNO055_ADDRESS_A, WRITE);
    I2C_Orientation_MasterWriteByte(BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR);
    I2C_Orientation_MasterSendStop();
    
    //Read in data  from the next 6 registers (it will spit out until a NAck)
    I2C_Orientation_MasterSendStart(BNO055_ADDRESS_A, READ);
    XACCEL = I2C_Orientation_MasterReadByte(I2C_Orientation_ACK_DATA);
    XACCEL = XACCEL + ( I2C_Orientation_MasterReadByte(I2C_Orientation_ACK_DATA) << 8 );

    YACCEL = I2C_Orientation_MasterReadByte(I2C_Orientation_ACK_DATA);
    YACCEL = YACCEL + ( I2C_Orientation_MasterReadByte(I2C_Orientation_ACK_DATA) << 8 );
    
    ZACCEL = I2C_Orientation_MasterReadByte(I2C_Orientation_ACK_DATA);
    ZACCEL = ZACCEL + ( I2C_Orientation_MasterReadByte(I2C_Orientation_NAK_DATA) << 8 ); //Send a No acknowledg to end data read
    
    I2C_Orientation_MasterSendStop();
    
    return;
}








void AGM_GetTemp(){
        
    
    /* --------------------ACCELEROMETER TEMPERATURE-----------------------*/
        //Send the module the address of the register to read from
    I2C_Orientation_MasterSendStart(BNO055_ADDRESS_A, WRITE);
    I2C_Orientation_MasterWriteByte(BNO055_TEMP_ADDR);
    I2C_Orientation_MasterSendStop();
    
    //Read in data  from the next 6 registers (it will spit out until a NAck)
    I2C_Orientation_MasterSendStart(BNO055_ADDRESS_A, READ);
    AGMTEMP = I2C_Orientation_MasterReadByte(I2C_Orientation_NAK_DATA);
    I2C_Orientation_MasterSendStop();
    
    //IF IN FARENHEIT THEN multiply by 2
    AGMTEMP = AGMTEMP * 2;
    
    return;
    
}






/* [] END OF FILE */
