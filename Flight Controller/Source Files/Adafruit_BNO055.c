/* =========================================
Last edited 07/10/2020
 * "Adafruit_BNO055.c"
 *
 * Author:  Nathan Miller
 *
 * Version: 4.2
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
 *    - 07/06/2020 - Changed read and write to use UART bus instead of I2C
 *    - 07/09/2020 - Added read error checking on each function call. Added CALIB as global and a method to query it.
 *
**/
 /* ======================================== */


#include "Adafruit_BNO055.h"


//------------------------- Private Functions -------------------------------------
//---------------------------------------------------------------------------------

//------------------------------ Start Hardware ------------------------------------
void BNO055_BUS_Start()
{
    //I2C_Orientation_Start();
    UART_IMU_Start();
    return;
}

//------------------------------ writeBNO055 UART------------------------------------
static uint8 writeBNO055(uint8 regAddr, uint8 dataBytes, uint8 *Data){    
    //Make sure UART Tx Buffer is > 4 + max dataBytes when using this method (5)
    //Returns 0 for good write, 1 for no acknowledge, else Status Error code from BNO055
    
    UART_IMU_ClearTxBuffer(); //If necessary, rewrite to check TxBuffer and only execute rest of function if clear instead.
    UART_IMU_ClearRxBuffer();
    
    //Write Command
    UART_IMU_PutChar(0xAA);
    UART_IMU_PutChar(WRITE);
    UART_IMU_PutChar(regAddr);
    UART_IMU_PutChar(dataBytes);
    UART_IMU_PutArray(Data,dataBytes);
    
    //Wait until all bytes are sent
    while(UART_IMU_GetTxBufferSize() > 0){}
    
    //Wait until acknowledge response
    uint16 counter = 0;
    while(UART_IMU_GetRxBufferSize() < 2)
    {
            if (counter++ >= 8000)
                return 1;
    }
    
    //Varify Transaction was good
    uint8 Response_Header = UART_IMU_GetChar();
    uint8 Response_Status = UART_IMU_GetChar();
    if ((Response_Header == 0xEE) && (Response_Status == 0x01))
    {
        return 0;
    }
    else
        return Response_Status;
}


//------------------------------ readBNO055 UART------------------------------------
static uint8 readBNO055(uint8 regAddr, uint8 bytes, uint8 *array)
{
    //Make sure UART Rx Buffer is atleast max num of data bytes + header (9)
    //Returns 0 for good read, 0x02 - 0x0A for IMU error codes, 0x10 no reponse from IMU, 0x11 UART error, 0x12 Unknown response byte, 0x13 length mismatch, 
    //0x2X and 0x3X are succesful retries with original fault code.
    
    uint8 status = 0x00, oldstatus = 0x00;
    uint8 do_retry = 0; //Set to 1 to retry failed reads. SET TO 0 BECAUSE IMU ALWAYS REPEATES PREV DATA ON RETRY
    
    UART_IMU_ClearTxBuffer(); //If necessary, rewrite to check TxBuffer and only execute rest of function if clear instead.
    UART_IMU_ClearRxBuffer();
    
    //Limit number of bytes read
    if (bytes > 16) bytes = 16;
    
    //Read Command
    UART_IMU_PutChar(0xAA);
    UART_IMU_PutChar(READ);
    UART_IMU_PutChar(regAddr);
    UART_IMU_PutChar(bytes);
    
    //Wait until all bytes are sent
    while(UART_IMU_GetTxBufferSize() > 0){}
    
    uint16 rawByte=0, counter=0;
    uint8 temp_array[16]; //Bad Practice -- use definite size?
    
    //Read through all expected data bytes + response_byte + response_length
    for (uint8 i=0; i <= bytes + 1; i++)
    {
        //Status and retry handling
        if ((status == 0) && (oldstatus == 0)) //Good status
        {
            //Wait for Bytes
            while(UART_IMU_GetRxBufferSize() == 0)
            {
                if (counter++ >= 8000)
                    return 0x10; //IMU no response
            }
            counter = 0;
            rawByte = UART_IMU_GetByte();
            
            //Check UART Error Status
            if ((rawByte >> 8) > 0)
            {
                return 0x11; //UART Error
            }
            
            temp_array[i] = ((uint8)(rawByte & 0xFF));
            
            //Check Response_Byte for success
            if (i == 0)
            {
                if ((temp_array[0] != 0xBB) && (temp_array[0] != 0xEE))
                {
                    status = 0x12; //Unknown response_byte
                }
            }
            //Check Response_Length/Status Byte
            else if (i == 1)
            {
                //Read Failure on IMU's end
                if (temp_array[0] == 0xEE)
                {
                    status = temp_array[1]; //Return IMU Error Code
                }
                //Check if returned length matches expected
                else if (temp_array[1] != bytes)
                {
                    status = 0x13; //Lengths do Not match
                }  
            }
        } //End good status
        else if (!do_retry)
        {
            return status;
        }
        else if ((status > 0) && (oldstatus == 0)) //Save original fault code and retry
        {
            oldstatus = status;
            status = 0x00;
            i=0;
            
            //Read Command
            UART_IMU_PutChar(0xAA);
            UART_IMU_PutChar(READ);
            UART_IMU_PutChar(regAddr);
            UART_IMU_PutChar(bytes);   
            //Wait until all bytes are sent
            while(UART_IMU_GetTxBufferSize() > 0){}
        }
        else if ((status == 0) && (oldstatus > 0)) //Retry Successful
        {
            return oldstatus+=0x20;
        }
        else //Retry failed
        {
            return oldstatus;
        }      
    } //end for
    
    //memcpy(temp_array+2, array, bytes);
    for(uint8 i=0; i < bytes; ++i)
    {
        array[i] = temp_array[i+2];
    }
    
    return 0;
}

/*
//------------------------------ writeBNO055 I2C ------------------------------------
static _Bool writeBNO055(uint8 regAddr, uint8 dataBytes, uint8 *Data){
    //Check if error by checking passed value from master write byte
    //and master start send with defined value 'I2C_Orientation_MSTR_NO_ERROR'
    I2C_Orientation_MasterSendStart(BNO055_ADDRESS_A, WRITE);
    I2C_Orientation_MasterWriteByte(regAddr);
    I2C_Orientation_MasterWriteByte(*Data);
    I2C_Orientation_MasterSendStop();
    
    
    return 1;

//------------------------------ readBNO055 I2C------------------------------------

static uint8 *readBNO055(uint8 regAddr, uint8 bytes)
{
   static uint8 tmp[8];
    //Send the module the address of the register to read from
    I2C_Orientation_MasterSendStart(BNO055_ADDRESS_A, WRITE);
    I2C_Orientation_MasterWriteByte(regAddr);
    //I2C_Orientation_MasterSendStop();

    I2C_Orientation_MasterSendRestart(BNO055_ADDRESS_A, READ);
     
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
*/

uint8 BNO055_Error()
{
    
    return 1;   
}


//------------------------- Public Functions -------------------------------------
//---------------------------------------------------------------------------------


//-------------------Orientation (ACCEL/GYRO/MAG) Functions --------------------
uint8 IMU_Start(){
    //Starts and configures the Accel/Gyro/Mag Sensor
    
    Timer_1_Start();
    BNO055_BUS_Start();
    
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
    uint8 rx_status=0;
    uint16 start_time=0, curr_time = 0;
    int16 duration;

    start_time = Timer_1_ReadCounter(); 
    
    while (CALIB <= 0b00010101){//FIX
        rx_status = IMU_GetCalib();
        if (rx_status == 0) //Good read
        {
            curr_time = Timer_1_ReadCounter();
            duration = start_time - curr_time;
            
            if (((uint16)duration) >= 5000) //500ms
            {
                LED_Write(0);
                return 0xFF; //Return timer expired error
            }
            //Toggle LED every 51.2 ms
            else if ( (((uint16)duration) >> 9)%2 == 0) //Divide by 512 and check if even number
            {
                LED_Write(1);
            }
            else
            {
                LED_Write(0);
            }
        } //end if (rx_status == 0)
        else //bad read
        {
            return rx_status;
        }
    }//end while
    LED_Write(0u);
    return 0;
    //-----------------------------------------------------------------   
}
      

void Write_IMU_Config(uint8 RegisterAddress, uint8 DataByte){
    writeBNO055(RegisterAddress, 1, &DataByte);
    CyDelay(50);
    return;
}
    

uint8 IMU_EulersRefresh(){
    //Get Eulers Angles from Registers of module
    
    /* decimal 16 = 1 deg
    ROLL = ROLL                       %-180 --> +180   ccw --> cw   *depends on format
    PITCH = PITCH                     %-90  --> +90   down --> up
    YAW = HEADING                     % 0   --> 360  compass heading
    */
       
    /* --------------- Euler Angles ----------------------------- */
    uint8 tmp[6], rx_status;
    
    rx_status = readBNO055(BNO055_EULER_H_LSB_ADDR, 6, tmp);
    
    if (rx_status == 0) //Good read
    {
        YAW = tmp[0];
        YAW = YAW + ( tmp[1] << 8 );

        ROLL = tmp[2];
        ROLL = ROLL + ( tmp[3] << 8 );
        
        PITCH = tmp[4];
        PITCH = PITCH + ( tmp[5] << 8 );   

        YAW+=(180<<4); //shifted by 180 degrees
        if(YAW >= (360<<4)) YAW-=(360<<4); //compensate max of 360 for shift to work
    
        return 0;
    }
    
    else  //bad read
        return rx_status;
}
    



uint8 IMU_AccelRefresh(){
    //Get the linear Acceleration of the chip
     
    /* --------------------LINEAR ACCELERATION-----------------------*/
    uint8 tmp[6], rx_status;
    rx_status = readBNO055(BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR, 6, tmp);
    
    if (rx_status == 0) //Good read
    {
        XACCEL = tmp[0];
        XACCEL = XACCEL + ( tmp[1] << 8 );

        YACCEL = tmp[2];
        YACCEL = YACCEL + ( tmp[3] << 8 );
        
        ZACCEL = tmp[4];
        ZACCEL = ZACCEL + ( tmp[5] << 8 ); //Send a No acknowledge to end data read
            
        return 0;
    }
    else //bad  read
        return rx_status;
}



uint8 IMU_GetTemp(){
        
    /* --------------------ACCELEROMETER TEMPERATURE-----------------------*/
    uint8 * tmp =0, rx_status;
    rx_status = readBNO055(BNO055_TEMP_ADDR, 1, tmp);
    
    if (rx_status == 0 ) //Good read
    {
        //Fahrenheit conversion with left shift compensation
        *tmp = (*tmp * 9)/5 + 64;
        IMUTEMP = *tmp;
        return 0;
    }
    else //bad read
        return rx_status;
    
}

uint8 IMU_GetCalib(){
    uint8 rx_status;
    
    rx_status = readBNO055(BNO055_CALIB_STAT_ADDR, 1, &CALIB);
    return rx_status;
}


/* [] END OF FILE */
