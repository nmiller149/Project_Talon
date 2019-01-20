/***************************************************************************
  This is a library for the BMP280 pressure sensor

  Designed specifically to work with the Adafruit BMP280 Breakout
  ----> http://www.adafruit.com/products/2651

  These sensors use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include "Adafruit_BMP280.h"





/********************************************************************
                    begin()
 
********************************************************************/

bool beginBMP280() {
  _i2caddr = BMP280_ADDRESS;

  SPI_Altitude_Start();    
  

  if (readBMP280(BMP280_REGISTER_CHIPID,1) != BMP280_CHIPID)
    return false;

  readCoefficients();
  writeBMP280(BMP280_REGISTER_CONTROL, 0x3F);
  return true;
}





/**************************************************************************
                            readCoefficients()
    
 - @brief  Reads the factory-set coefficients
**************************************************************************/
void readCoefficients(void)
{
    _bmp280_calib.dig_T1 = readBMP280_LE(BMP280_REGISTER_DIG_T1);
    _bmp280_calib.dig_T2 = readBMP280_LE(BMP280_REGISTER_DIG_T2);
    _bmp280_calib.dig_T3 = readBMP280_LE(BMP280_REGISTER_DIG_T3);

    _bmp280_calib.dig_P1 = readBMP280_LE(BMP280_REGISTER_DIG_P1);
    _bmp280_calib.dig_P2 = readBMP280_LE(BMP280_REGISTER_DIG_P2);
    _bmp280_calib.dig_P3 = readBMP280_LE(BMP280_REGISTER_DIG_P3);
    _bmp280_calib.dig_P4 = readBMP280_LE(BMP280_REGISTER_DIG_P4);
    _bmp280_calib.dig_P5 = readBMP280_LE(BMP280_REGISTER_DIG_P5);
    _bmp280_calib.dig_P6 = readBMP280_LE(BMP280_REGISTER_DIG_P6);
    _bmp280_calib.dig_P7 = readBMP280_LE(BMP280_REGISTER_DIG_P7);
    _bmp280_calib.dig_P8 = readBMP280_LE(BMP280_REGISTER_DIG_P8);
    _bmp280_calib.dig_P9 = readBMP280_LE(BMP280_REGISTER_DIG_P9);
}




/********************************************************************
                    readTemperature()
 
********************************************************************/
float readTemperature(void)
{
  int32_t var1, var2;

  int32_t adc_T = readBMP280(BMP280_REGISTER_TEMPDATA,3);
  adc_T >>= 4;

  var1  = ((((adc_T>>3) - ((int32_t)_bmp280_calib.dig_T1 <<1))) *
	   ((int32_t)_bmp280_calib.dig_T2)) >> 11;

  var2  = (((((adc_T>>4) - ((int32_t)_bmp280_calib.dig_T1)) *
	     ((adc_T>>4) - ((int32_t)_bmp280_calib.dig_T1))) >> 12) *
	   ((int32_t)_bmp280_calib.dig_T3)) >> 14;

  t_fine = var1 + var2;

  float T  = (t_fine * 5 + 128) >> 8;
  return T/100;
}



/********************************************************************
                    readPressure()
 
********************************************************************/
float readPressure(void) {
  int64_t var1, var2, p;

  // Must be done first to get the t_fine variable set up
  readTemperature();

  int32_t adc_P = readBMP280(BMP280_REGISTER_PRESSUREDATA,3);
  adc_P >>= 4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)_bmp280_calib.dig_P6;
  var2 = var2 + ((var1*(int64_t)_bmp280_calib.dig_P5)<<17);
  var2 = var2 + (((int64_t)_bmp280_calib.dig_P4)<<35);
  var1 = ((var1 * var1 * (int64_t)_bmp280_calib.dig_P3)>>8) +
    ((var1 * (int64_t)_bmp280_calib.dig_P2)<<12);
  var1 = (((((int64_t)1)<<47)+var1))*((int64_t)_bmp280_calib.dig_P1)>>33;

  if (var1 == 0) {
    return 0;  // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p<<31) - var2)*3125) / var1;
  var1 = (((int64_t)_bmp280_calib.dig_P9) * (p>>13) * (p>>13)) >> 25;
  var2 = (((int64_t)_bmp280_calib.dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t)_bmp280_calib.dig_P7)<<4);
  return (float)p/256;
}


/********************************************************************
                    readAltitude
 
********************************************************************/
float readAltitude(float seaLevelhPa) {
  seaLevelhPa = 1013.25;
    
  float altitude;

  float pressure = readPressure(); // in Si units for Pascal
  pressure /= 100;

  altitude = 44330 * (1.0 - pow((pressure / seaLevelhPa), 0.1903));

  return altitude;
}

uint8 BMP280_State = 0; //Used for BMP280 State Machine
uint8 BMP280_byteSize = 0;


/*=========================================================================
                       PRIVATE FUNCTIONS
=========================================================================*/

/********************************************************************
                    writeBMP280
 
********************************************************************/

bool writeBMP280(uint8 regAddr, uint8 Data){
    //Needs to use SPI_Altitude_GetTxBufferSize(); if software buffer not used. 
    //Otherwise, make sure software buffer is large enough for that frequency of which 
    //this function will be called
   
    //SPI_Altitude_WriteTxData((regAddr<<8) + Data);
    if (BMP280_State == 0)
    { 
        SPI_Altitude_WriteTxData(regAddr);
        SPI_Altitude_WriteTxData(Data);
        return 1;
    }//endif
    
    else return 0;
    
}


/********************************************************************
                    readBMP280
 
********************************************************************/
uint32 readBMP280(uint8 regAddr, uint8 bytes){
    uint32_t tmp = 0;
    uint8_t timer = 0;
    
    BMP280_CSB_Write(0);
    SPI_Altitude_WriteTxData(regAddr | 0x80);
    while ((SPI_Altitude_GetRxBufferSize() <= bytes) && (timer <200))
    {
        timer++;
    }
    if (timer < 200)
    {
        for (uint8 i = 1; i <= bytes; i++){
            tmp = (tmp << 8) | SPI_Altitude_ReadRxData();    
        }
    }
    else BMP280_Error();    
    BMP280_CSB_Write(1);
    
    return tmp;
}



/********************************************************************
                    readBMP280_LE
 
********************************************************************/
uint16 readBMP280_LE(uint8 reg) {
  uint16 tmp = (uint16)readBMP280(reg,2);
  return (tmp >> 8) | (tmp << 8);
}


/********************************************************************
                    BMP280_Error
 
********************************************************************/
void BMP280_Error(){
    
}










//JUNK 

    //The BMP280_State starts at 0, indicating that it is ready for read command
    //Once a readBMP280Addr is called, a control byte is sent and the state switches to 1
    //Which turns on the read checker "checkReadBMP280()" that waits for the buffer to get a byte
    // and will return that byte and increment the state until the the total desired bytes have been read
    // then the state is set to 100, making it ready to parse.
    //After parsing, the state is set back to 0
    
    //Is all of this process necessary? Or is the module fast enough to cut out some of the bs?
uint16 checkReadBMP280()
{
    //Put this function in the infinite loop. 
    //Will automatically check state
    

    //TO DO: sum the read bytes and the buffered bytes to let the chip select ss pin back to high
    // Is optimizing the code to pick up bytes and go even necessary? Or is that how the master should work while 
    //seperate chip is running the speed intensive controls code?

    if ((BMP280_State >= 1) && (BMP280_State < 100) && (SPI_Altitude_GetRxBufferSize() > 0))
    {
        BMP280_State ++;
        return SPI_Altitude_ReadRxData();
        
        if (BMP280_State > BMP280_byteSize){
            BMP280_State = 100;
        }
    }

    
    /* This method reads in all bytes at once
    if (BMP280_State == 1){
        if (SPI_Altitude_GetRxBufferSize() >= BMP280_byteSize)
        {
            while(SPI_Altitude_GetRxBufferSize() >= 0){
                //SPIM_ReadRxData();
            
            }
        }
    }
    */  
    return 0;
}



