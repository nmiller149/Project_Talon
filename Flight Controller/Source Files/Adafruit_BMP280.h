/* =========================================
Last edited 06/16/2019
 * "Adafruit_BMP280.h"
 *
 * Author:  Nathan Miller
 *
 * Version: 2.0
 *
 * Description:
 * HEADER FILE - Contains all files, functions, and variables used for the BMP280 pressure and temp sensor to acquire Alititude 
 *

 * Reasons for Revision:
 *    -  Completed Build = Fully functional now
 *    - 
 *    -
 *
**/
 /* ======================================== */
#ifndef __BMP280_H__
#define __BMP280_H__

#ifndef PROJECT_H
#define PROJECT_H
#include <project.h>
#endif
    
#include <stdbool.h>
#include <math.h>

/*=========================================================================
    Conrol Settings
    -----------------------------------------------------------------------*/
    #define BMP280_SAMPLING_OFF           0
    #define BMP280_OVERSAMPLING_1         1
    #define BMP280_OVERSAMPLING_2         2
    #define BMP280_OVERSAMPLING_4         3
    #define BMP280_OVERSAMPLING_8         4
    #define BMP280_OVERSAMPLING_16        5

/*  Power Mode Settings
    -----------------------------------------------------------------------*/
    #define BMP280_SLEEP_MODE             0b00
    #define BMP280_FORCED_MODE            0b10
    #define BMP280_NORMAL_MODE            0b11

/*    Conrol Settings
    -----------------------------------------------------------------------*/
    #define BMP280_FILTER_COEFFICIENT_OFF  0
    #define BMP280_FILTER_COEFFICIENT_2    1        //THESE VALUES ARE NOT CERTAIN!!!!
    #define BMP280_FILTER_COEFFICIENT_4    2
    #define BMP280_FILTER_COEFFICIENT_8    3
    #define BMP280_FILTER_COEFFICIENT_16   4
  
/*    Standby Time
    -----------------------------------------------------------------------*/
    #define BMP280_STANDBY_MS_0_5          0b000
    #define BMP280_STANDBY_MS_62_5         0b001
    #define BMP280_STANDBY_MS_125          0b010
    #define BMP280_STANDBY_MS_250          0b011
    #define BMP280_STANDBY_MS_500          0b100
    #define BMP280_STANDBY_MS_1000         0b101
    #define BMP280_STANDBY_MS_2000         0b110
    #define BMP280_STANDBY_MS_4000         0b111
    
/*=========================================================================*/


/*=========================================================================
    I2C ADDRESS/BITS/SETTINGS
    -----------------------------------------------------------------------*/
    #define BMP280_ADDRESS                0x77u
    #define BMP280_CHIPID                 0x58u
/*=========================================================================*/

    
/*=========================================================================
    REGISTERS
    -----------------------------------------------------------------------*/
    enum
    {
      BMP280_REGISTER_DIG_T1              = 0x88,
      BMP280_REGISTER_DIG_T2              = 0x8A,
      BMP280_REGISTER_DIG_T3              = 0x8C,

      BMP280_REGISTER_DIG_P1              = 0x8E,
      BMP280_REGISTER_DIG_P2              = 0x90,
      BMP280_REGISTER_DIG_P3              = 0x92,
      BMP280_REGISTER_DIG_P4              = 0x94,
      BMP280_REGISTER_DIG_P5              = 0x96,
      BMP280_REGISTER_DIG_P6              = 0x98,
      BMP280_REGISTER_DIG_P7              = 0x9A,
      BMP280_REGISTER_DIG_P8              = 0x9C,
      BMP280_REGISTER_DIG_P9              = 0x9E,

      BMP280_REGISTER_CHIPID             = 0xD0,
      BMP280_REGISTER_VERSION            = 0xD1,
      BMP280_REGISTER_SOFTRESET          = 0xE0,

      BMP280_REGISTER_CAL26              = 0xE1,  // R calibration stored in 0xE1-0xF0

      BMP280_REGISTER_CONTROL            = 0xF4,
      BMP280_REGISTER_CONFIG             = 0xF5,
      BMP280_REGISTER_PRESSUREDATA       = 0xF7,
      BMP280_REGISTER_TEMPDATA           = 0xFA,
    };

/*=========================================================================*/

    
/*=========================================================================
    CALIBRATION DATA
    -----------------------------------------------------------------------*/
typedef struct
    {
      uint16_t dig_T1;
      int16_t  dig_T2;
      int16_t  dig_T3;

      uint16_t dig_P1;
      int16_t  dig_P2;
      int16_t  dig_P3;
      int16_t  dig_P4;
      int16_t  dig_P5;
      int16_t  dig_P6;
      int16_t  dig_P7;
      int16_t  dig_P8;
      int16_t  dig_P9;

      uint8_t  dig_H1;
      int16_t  dig_H2;
      uint8_t  dig_H3;
      int16_t  dig_H4;
      int16_t  dig_H5;
      int8_t   dig_H6;
    } bmp280_calib_data;
/*=========================================================================*/
bmp280_calib_data _bmp280_calib;

#define BMP_ALT_ESTIMATE_CONST 28046  //27637    
#define BMP_ALT_ESTIMATE_CONST_W_TEMP 97
    
typedef int32 BMP280_S32_t;
typedef uint32 BMP280_U32_t;
    
//Public Functions
bool  beginBMP280();
BMP280_S32_t readTemperature(void);
BMP280_U32_t readPressure(void);
BMP280_S32_t bmp280_compensate_T_int32(BMP280_S32_t adc_T);
BMP280_U32_t bmp280_compensate_P_int32(BMP280_S32_t adc_P);
uint32 * readPressureTemperature(void);
BMP280_U32_t *readAltitude(BMP280_U32_t seaLevelhPa); 

    







/*
class Adafruit_BMP280
{
  public:
    Adafruit_BMP280();
    Adafruit_BMP280(int8_t cspin);
    Adafruit_BMP280(int8_t cspin, int8_t mosipin, int8_t misopin, int8_t sckpin);

    bool  begin(uint8_t addr = BMP280_ADDRESS, uint8_t chipid = BMP280_CHIPID);
    float readTemperature(void);
    float readPressure(void);
    float readAltitude(float seaLevelhPa = 1013.25);

  private:

    void readCoefficients(void);
    uint8_t spixfer(uint8_t x);

    void      write8(byte reg, byte value);
    uint8_t   read8(byte reg);
    uint16_t  read16(byte reg);
    uint32_t  read24(byte reg);
    int16_t   readS16(byte reg);
    uint16_t  read16_LE(byte reg); // little endian
    int16_t   readS16_LE(byte reg); // little endian

    uint8_t   _i2caddr;
    int32_t   _sensorID;
    int32_t t_fine;

    int8_t _cs, _mosi, _miso, _sck;

    bmp280_calib_data _bmp280_calib;

};
*/
    
#endif
