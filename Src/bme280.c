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
#define REGARDD_PRESS_MSB  0xF7u

static uint8_t BME280_TxBuffer[32u];
static uint8_t BME280_RxBuffer[32u];

extern I2C_HandleTypeDef hi2c1;

bool BME280_Available = false;
BME280_RawMeasVal_t BME280_RawValues = {0u};

void BME280_Detect( void )
{
  BME280_TxBuffer[0u] = REGADDR_ID;
  HAL_I2C_Master_Transmit(&hi2c1, BME280_ADDR, BME280_TxBuffer, 1u, 100u);
  HAL_I2C_Master_Receive(&hi2c1, BME280_ADDR, BME280_RxBuffer, 1u, 100u);
  // Set avilability flag
  if( BME280_RxBuffer[0u] == 0x60u )
  {
    BME280_Available = true;
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
  BME280_TxBuffer[0u] = REGARDD_PRESS_MSB;
  HAL_I2C_Master_Transmit(&hi2c1, BME280_ADDR, BME280_TxBuffer, 1u, 100u);
  HAL_I2C_Master_Receive(&hi2c1, BME280_ADDR, BME280_RxBuffer, 8u, 100u);
  // Extract the raw values
  BME280_RawValues.RawPres = (((uint32_t)BME280_RxBuffer[0u]) << 12u) | (((uint32_t)BME280_RxBuffer[1u]) << 4u) | (((uint32_t)BME280_RxBuffer[2u]) >> 4u);
  BME280_RawValues.RawTemp = (((uint32_t)BME280_RxBuffer[3u]) << 12u) | (((uint32_t)BME280_RxBuffer[4u]) << 4u) | (((uint32_t)BME280_RxBuffer[5u]) >> 4u);
  BME280_RawValues.RawHum  = (((uint32_t)BME280_RxBuffer[6u]) <<  8u) | (uint32_t)BME280_RxBuffer[7u];
}
