/* =========================================
Last edited 03/29/2020
 * "Adafruit_BMP280.c"
 *
 * Author:  Nathan Miller
 *
 * Version: 2.1
 *
 * Description:
 * SOURCE FILE - Contains all files, functions, and variables used for the BMP280 pressure and temp sensor to acquire Alititude 
 *
 *
 * Reasons for Revision:
 *    - 6/16/2019 Completed Build = Now fully functional
 *    - 3/29/2020 Reorganized Functions and removed some junk
 *    -
 *
**/
 /* ======================================== */

#include "Adafruit_BMP280.h"

BMP280_S32_t t_fine;

/*=========================================================================
                       PRIVATE FUNCTIONS
=========================================================================*/

/********************************************************************
                    writeBMP280
 
********************************************************************/

static _Bool writeBMP280(uint8 regAddr, uint8 Data){
    //Needs to use SPI_Altitude_GetTxBufferSize(); if software buffer not used. 
    //Otherwise, make sure software buffer is large enough for that frequency of which 
    //this function will be called

    //SPI_Altitude_WriteTxData((regAddr<<8) + Data);
    BMP280_CSB_Write(0);
    SPIM_1_WriteTxData(regAddr & 0x7F);
    SPIM_1_WriteTxData(Data);
    SPIM_1_ClearRxBuffer();
    BMP280_CSB_Write(1);
    return 0;
    
}


/********************************************************************
                    readBMP280
 
********************************************************************/

static uint8* readBMP280(uint8 regAddr, uint8 bytes){
    static uint8 tmp[10];
    
    BMP280_CSB_Write(0);
    SPIM_1_ClearRxBuffer();
    
    SPIM_1_WriteByte(regAddr  | 0x80);
    for(uint8 i=0; i<bytes; i++)
    {
        SPIM_1_WriteByte(0);
    }
    int32 timeOut = 0;
    while(SPIM_1_GetRxBufferSize() < bytes  && timeOut < 500000){timeOut++;}
    SPIM_1_ReadRxData();//SPIM_1_ClearRxBuffer();
    uint8 i;
    for(i=0; i<bytes; i++)
    {
        tmp[i] = SPIM_1_ReadRxData(); //Read
    }
	tmp[i] = 0;
    BMP280_CSB_Write(1);
    return &tmp[0];
}

static uint16_t readBMP280_LE(uint8 reg) {
  uint8_t * temp = readBMP280(reg,2);
  return (temp[0]) | (temp[1] << 8);

}


/**************************************************************************
                            readCoefficients()
    
 - @brief  Reads the factory-set coefficients
**************************************************************************/
static void readCoefficients(void)
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
                    BMP280_Error
 
********************************************************************/
void BMP280_Error(){
    
}

/*=========================================================================
                       PUBLIC FUNCTIONS
=========================================================================*/

/********************************************************************
                    beginBMP280()
 
********************************************************************/

bool beginBMP280() {
  //_i2caddr = BMP280_ADDRESS; //? What is this for?

  VDAC8_1_Start();
  SPIM_1_Start();    
  CyDelay(100);

  if (*readBMP280(BMP280_REGISTER_CHIPID,1) != BMP280_CHIPID)
    return false;

  readCoefficients();

  uint8 osrs_t     = BMP280_OVERSAMPLING_2;
  uint8 osrs_p     = BMP280_OVERSAMPLING_8;
  uint8 power_mode = BMP280_NORMAL_MODE;

  uint8 t_sb   = BMP280_STANDBY_MS_0_5;
  uint8 filter = BMP280_FILTER_COEFFICIENT_16;

  writeBMP280(BMP280_REGISTER_CONTROL,((osrs_t<<5)|(osrs_p<<2)|(power_mode)));
  writeBMP280(BMP280_REGISTER_CONFIG,((t_sb<<5)|(filter<<2)));
  CyDelay(300);

  
  return true;
}







/********************************************************************
                    readTemperature()
 
********************************************************************/
BMP280_S32_t readTemperature(void)
{
    uint8* ptr;
    ptr = readBMP280(BMP280_REGISTER_TEMPDATA,3);    
    BMP280_S32_t adc_T = (ptr[0]<<12) | (ptr[1]<<4) | (ptr[2]);
    
    return adc_T;
}


// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
// t_fine carries fine temperature as global value
BMP280_S32_t bmp280_compensate_T_int32(BMP280_S32_t adc_T)
{
    BMP280_S32_t var1, var2, T;
    //BMP280_S32_t adc_T = readBMP280(BMP280_REGISTER_TEMPDATA,3);
    
    var1 = ((((adc_T>>3) - ((BMP280_S32_t)_bmp280_calib.dig_T1<<1))) * ((BMP280_S32_t)_bmp280_calib.dig_T2)) >> 11;
    var2 = (((((adc_T>>4) - ((BMP280_S32_t)_bmp280_calib.dig_T1)) * ((adc_T>>4) - 
           ((BMP280_S32_t)_bmp280_calib.dig_T1))) >> 12) * ((BMP280_S32_t)_bmp280_calib.dig_T3)) >> 14;
    
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}


/********************************************************************
                    readPressure()
 
********************************************************************/
BMP280_U32_t readPressure(void) {
    uint8* ptr;
    ptr = readBMP280(BMP280_REGISTER_PRESSUREDATA,3); 
    BMP280_S32_t adc_P = (ptr[0]<<12) | (ptr[1]<<4) | (ptr[2]);
    
    return adc_P;
}


// Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
BMP280_U32_t bmp280_compensate_P_int32(BMP280_S32_t adc_P)
{
    BMP280_S32_t var1, var2;
    BMP280_U32_t p;
    
    var1 = (((BMP280_S32_t)t_fine)>>1) - (BMP280_S32_t)64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((BMP280_S32_t)_bmp280_calib.dig_P6);
    var2 = var2 + ((var1*((BMP280_S32_t)_bmp280_calib.dig_P5))<<1);
    var2 = (var2>>2)+(((BMP280_S32_t)_bmp280_calib.dig_P4)<<16);
    var1 = (((_bmp280_calib.dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((BMP280_S32_t)_bmp280_calib.dig_P2) * var1)>>1))>>18;
    var1 =((((32768+var1))*((BMP280_S32_t)_bmp280_calib.dig_P1))>>15);
    
    if (var1 == 0)
    {
        return 0; // avoid exception caused by division by zero
    }
    
    p = (((BMP280_U32_t)(((BMP280_S32_t)1048576)-adc_P)-(var2>>12)))*3125;
    if (p < 0x80000000)
    {
        p = (p << 1) / ((BMP280_U32_t)var1);
    }
    else
    {
        p = (p / (BMP280_U32_t)var1) * 2;
    }
    
    var1 = (((BMP280_S32_t)_bmp280_calib.dig_P9) * ((BMP280_S32_t)(((p>>3) * (p>>3))>>13)))>>12;
    var2 = (((BMP280_S32_t)(p>>2)) * ((BMP280_S32_t)_bmp280_calib.dig_P8))>>13;
    
    p = (BMP280_U32_t)((BMP280_S32_t)p + ((var1 + var2 + _bmp280_calib.dig_P7) >> 4));
    return p;
}


/********************************************************************
                    readAltitude
 
********************************************************************/
// Returns altitude in ft as unsigned 32 bit integer with fixed decimal place of two. Output value of “96386” equals 963.86 ft
BMP280_U32_t *readAltitude(BMP280_U32_t seaLevelhPa) {    
    if (seaLevelhPa == 0)
        seaLevelhPa = 101325;
    
    //uint8 STATUS = *readBMP280(0xF3,1);
    
    BMP280_U32_t *tmp = readPressureTemperature(); 
    BMP280_U32_t pressure, adc_P = tmp[0];
    BMP280_S32_t temperature, adc_T = tmp[1];
    
    temperature = bmp280_compensate_T_int32(adc_T);
    pressure    = bmp280_compensate_P_int32(adc_P);
    
    //Altitude Approximation for feet
    BMP280_U32_t altitude =(BMP_ALT_ESTIMATE_CONST*(seaLevelhPa-pressure))/1000; //ft with 2 decimal places
    //altitude=(BMP_ALT_ESTIMATE_CONST_W_TEMP*(temperature+273)*(seaLevelhPa-pressure))/1000;   
    //altitude = 44330 * (1 - pow((pressure / seaLevelhPa), 0.1903));
    
    static BMP280_U32_t output[3];
    output[0] = altitude; output[1] = pressure; output[2] = temperature;
    return &output[0];
}


/********************************************************************
                    readPressureTemperature()
 
********************************************************************/
uint32 * readPressureTemperature(void) {
    uint8*ptr;
    static BMP280_U32_t adc[2];
    ptr = readBMP280(BMP280_REGISTER_PRESSUREDATA,6); 
    adc[0] = (ptr[0]<<12) | (ptr[1]<<4) | (ptr[2]);
    adc[1] = (ptr[3]<<12) | (ptr[4]<<4) | (ptr[5]);
    return &adc[0];
}

