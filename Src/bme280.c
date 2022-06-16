/*
 * bme280.c
 *
 *  Created on: Jun 3, 2022
 *      Author: drCsabesz
 */

#include "stm32f4xx_hal.h"
#include "main.h"
#include "bme280.h"

#define BME280_ADDR    0xECu

#define REGADDR_ID         0xD0u
#define REGADDR_RESET      0xE0u
#define REGADDR_CTRL_HUM   0xF2u
#define REGADDR_STATUS     0xF3u
#define REGADDR_CTRL_MEAS  0xF4u
#define REGADDR_CONFIG     0xF5u
#define REGADDR_PRESS_MSB  0xF7u
#define REGADDR_CALIB00    0x88u
#define REGADDR_CALIB26    0xE1u

/**\name Macro to combine two 8 bit data's to form a 16 bit data */
#define BME280_CONCAT_BYTES(msb, lsb)             (((uint16_t)msb << 8) | (uint16_t)lsb)

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
  uint8_t  padding1;
  int16_t  dig_H2;
  uint16_t dig_H3;
  int16_t  dig_H4;
  int16_t  dig_H5;
  int8_t   dig_H6;
  uint8_t  padding2;
  int32_t  t_Fine;
} CalibData_t;

static uint8_t BME280_TxBuffer[32u];
static uint8_t BME280_RxBuffer[32u];
static CalibData_t CalibData;

extern I2C_HandleTypeDef hi2c1;

bool BME280_Available = false;
BME280_RawMeasVal_t BME280_RawValues = {0u};
BME280_PhysValues_t BME280_PhysicalValues;

static void CalculateTemperature( void );
static void CalculatePressure( void );
static void CalculateHumidity( void );
static void ReadCalibParams( void );

void BME280_Detect( void )
{
  BME280_TxBuffer[0u] = REGADDR_ID;
  HAL_I2C_Master_Transmit(&hi2c1, BME280_ADDR, BME280_TxBuffer, 1u, 100u);
  HAL_I2C_Master_Receive(&hi2c1, BME280_ADDR, BME280_RxBuffer, 1u, 100u);
  // Set avilability flag
  if( BME280_RxBuffer[0u] == 0x60u )
  {
    BME280_Available = true;
    // Read calibration
    ReadCalibParams();
  }
}

void BME280_StartMeasurement( BME280_Oversampling_e ovsTemp, BME280_Oversampling_e ovsHum, BME280_Oversampling_e ovsPres )
{
  // Set ctrl hum register value:
  BME280_TxBuffer[0u] = REGADDR_CTRL_HUM;
  BME280_TxBuffer[1u] = ovsHum;
  HAL_I2C_Master_Transmit(&hi2c1, BME280_ADDR, BME280_TxBuffer, 2u, 100u);
  // Set ctrl_meas register and set normal mode
  BME280_TxBuffer[0u] = REGADDR_CTRL_MEAS;
  BME280_TxBuffer[1u] = 0x03u | (ovsPres << 2u) | (ovsTemp << 5u);
  HAL_I2C_Master_Transmit(&hi2c1, BME280_ADDR, BME280_TxBuffer, 2u, 100u);
}

void BME280_ReadMeasResult( void )
{
  // Set start address
  BME280_TxBuffer[0u] = REGADDR_PRESS_MSB;
  HAL_I2C_Master_Transmit(&hi2c1, BME280_ADDR, BME280_TxBuffer, 1u, 100u);
  HAL_I2C_Master_Receive(&hi2c1, BME280_ADDR, BME280_RxBuffer, 8u, 100u);
  // Extract the raw values
  BME280_RawValues.RawPres = (((uint32_t)BME280_RxBuffer[0u]) << 12u) | (((uint32_t)BME280_RxBuffer[1u]) << 4u) | (((uint32_t)BME280_RxBuffer[2u]) >> 4u);
  BME280_RawValues.RawTemp = (((uint32_t)BME280_RxBuffer[3u]) << 12u) | (((uint32_t)BME280_RxBuffer[4u]) << 4u) | (((uint32_t)BME280_RxBuffer[5u]) >> 4u);
  BME280_RawValues.RawHum  = (((uint32_t)BME280_RxBuffer[6u]) <<  8u) | (uint32_t)BME280_RxBuffer[7u];
  // Convert To Physical
  CalculateTemperature();
  CalculateHumidity();
  CalculatePressure();
}

static void ReadCalibParams( void )
{
  // Set start address calib 00...25
  BME280_TxBuffer[0u] = REGADDR_CALIB00;
  HAL_I2C_Master_Transmit(&hi2c1, BME280_ADDR, BME280_TxBuffer, 1u, 100u);
  HAL_I2C_Master_Receive(&hi2c1, BME280_ADDR, BME280_RxBuffer, 25u, 100u);
  // Set start address calib 26...41
  BME280_TxBuffer[0u] = REGADDR_CALIB26;
  HAL_I2C_Master_Transmit(&hi2c1, BME280_ADDR, BME280_TxBuffer, 1u, 100u);
  HAL_I2C_Master_Receive(&hi2c1, BME280_ADDR, &BME280_RxBuffer[25u], 7u, 100u);
  // Set values
  CalibData.dig_T1 = BME280_CONCAT_BYTES(BME280_RxBuffer[1], BME280_RxBuffer[0]);
  CalibData.dig_T2 = (int16_t)BME280_CONCAT_BYTES(BME280_RxBuffer[3], BME280_RxBuffer[2]);
  CalibData.dig_T3 = (int16_t)BME280_CONCAT_BYTES(BME280_RxBuffer[5], BME280_RxBuffer[4]);
  CalibData.dig_P1 = BME280_CONCAT_BYTES(BME280_RxBuffer[7], BME280_RxBuffer[6]);
  CalibData.dig_P2 = (int16_t)BME280_CONCAT_BYTES(BME280_RxBuffer[9], BME280_RxBuffer[8]);
  CalibData.dig_P3 = (int16_t)BME280_CONCAT_BYTES(BME280_RxBuffer[11], BME280_RxBuffer[10]);
  CalibData.dig_P4 = (int16_t)BME280_CONCAT_BYTES(BME280_RxBuffer[13], BME280_RxBuffer[12]);
  CalibData.dig_P5 = (int16_t)BME280_CONCAT_BYTES(BME280_RxBuffer[15], BME280_RxBuffer[14]);
  CalibData.dig_P6 = (int16_t)BME280_CONCAT_BYTES(BME280_RxBuffer[17], BME280_RxBuffer[16]);
  CalibData.dig_P7 = (int16_t)BME280_CONCAT_BYTES(BME280_RxBuffer[19], BME280_RxBuffer[18]);
  CalibData.dig_P8 = (int16_t)BME280_CONCAT_BYTES(BME280_RxBuffer[21], BME280_RxBuffer[20]);
  CalibData.dig_P9 = (int16_t)BME280_CONCAT_BYTES(BME280_RxBuffer[23], BME280_RxBuffer[22]);
  CalibData.dig_H1 = BME280_RxBuffer[24];
  CalibData.dig_H2 = (int16_t)BME280_CONCAT_BYTES(BME280_RxBuffer[26], BME280_RxBuffer[25]);
  CalibData.dig_H3 = BME280_RxBuffer[27];
  CalibData.dig_H4 = ((int16_t)((int8_t)BME280_RxBuffer[28]) << 4) | ((int16_t)(BME280_RxBuffer[29] & 0x0Fu));
  CalibData.dig_H5 = ((int16_t)((int8_t)BME280_RxBuffer[30]) << 4) | ((int16_t)(BME280_RxBuffer[29] >> 4u & 0x0Fu));
  CalibData.dig_H6 = (int8_t)BME280_RxBuffer[31];
}

static void CalculateTemperature( void )
{
  double var1;
  double var2;
  double temperature;
  double temperature_min = -40;
  double temperature_max = 85;

  var1 = ((double)BME280_RawValues.RawTemp) / 16384.0 - ((double)CalibData.dig_T1) / 1024.0;
  var1 = var1 * ((double)CalibData.dig_T2);
  var2 = (((double)BME280_RawValues.RawTemp) / 131072.0 - ((double)CalibData.dig_T1) / 8192.0);
  var2 = (var2 * var2) * ((double)CalibData.dig_T3);
  CalibData.t_Fine = (int32_t)(var1 + var2);
  temperature = (var1 + var2) / 5120.0;

  if (temperature < temperature_min)
  {
      temperature = temperature_min;
  }
  else if (temperature > temperature_max)
  {
      temperature = temperature_max;
  }

  BME280_PhysicalValues.Temperature = temperature;
}

/*!
 * @brief This internal API is used to compensate the raw pressure data and
 * return the compensated pressure data in double data type.
 */
static void CalculatePressure( void )
{
  double var1;
  double var2;
  double var3;
  double pressure;
  double pressure_min = 30000.0;
  double pressure_max = 110000.0;

  var1 = ((double)CalibData.t_Fine / 2.0) - 64000.0;
  var2 = var1 * var1 * ((double)CalibData.dig_P6) / 32768.0;
  var2 = var2 + var1 * ((double)CalibData.dig_P5) * 2.0;
  var2 = (var2 / 4.0) + (((double)CalibData.dig_P4) * 65536.0);
  var3 = ((double)CalibData.dig_P3) * var1 * var1 / 524288.0;
  var1 = (var3 + ((double)CalibData.dig_P2) * var1) / 524288.0;
  var1 = (1.0 + var1 / 32768.0) * ((double)CalibData.dig_P1);

  /* avoid exception caused by division by zero */
  if (var1 > (0.0))
  {
    pressure = 1048576.0 - (double) BME280_RawValues.RawPres;
    pressure = (pressure - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double)CalibData.dig_P9) * pressure * pressure / 2147483648.0;
    var2 = pressure * ((double)CalibData.dig_P8) / 32768.0;
    pressure = pressure + (var1 + var2 + ((double)CalibData.dig_P7)) / 16.0;

    if (pressure < pressure_min)
    {
      pressure = pressure_min;
    }
    else if (pressure > pressure_max)
    {
      pressure = pressure_max;
    }
  }
  else /* Invalid case */
  {
    pressure = pressure_min;
  }

  BME280_PhysicalValues.Pressure = pressure;
}

/*!
 * @brief This internal API is used to compensate the raw humidity data and
 * return the compensated humidity data in double data type.
 */
static void CalculateHumidity( void )
{
  double humidity;
  double humidity_min = 0.0;
  double humidity_max = 100.0;
  double var1;
  double var2;
  double var3;
  double var4;
  double var5;
  double var6;

  var1 = ((double)CalibData.t_Fine) - 76800.0;
  var2 = (((double)CalibData.dig_H4) * 64.0 + (((double)CalibData.dig_H5) / 16384.0) * var1);
  var3 = BME280_RawValues.RawHum - var2;
  var4 = ((double)CalibData.dig_H2) / 65536.0;
  var5 = (1.0 + (((double)CalibData.dig_H3) / 67108864.0) * var1);
  var6 = 1.0 + (((double)CalibData.dig_H6) / 67108864.0) * var1 * var5;
  var6 = var3 * var4 * (var5 * var6);
  humidity = var6 * (1.0 - ((double)CalibData.dig_H1) * var6 / 524288.0);

  if (humidity > humidity_max)
  {
      humidity = humidity_max;
  }
  else if (humidity < humidity_min)
  {
      humidity = humidity_min;
  }

  BME280_PhysicalValues.Humidity = humidity;
}
