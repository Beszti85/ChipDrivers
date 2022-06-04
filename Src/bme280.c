/*
 * bme280.c
 *
 *  Created on: Jun 3, 2022
 *      Author: drCsabesz
 */

#include "bme280.h"

#define BME280_ADDR    0xECu

uint8_t BME280_TxBuffer[32u];
uint8_t BME280_RxBuffer[32u];

extern I2C_HandleTypeDef hi2c1;

void BME280_Detect( void )
{
  BME280_TxBuffer[0u] = 0xD0u;
  HAL_I2C_Master_Transmit(&hi2c1, BME280_ADDR, BME280_TxBuffer, 1u, 100u);
  HAL_I2C_Master_Receive(&hi2c1, BME280_ADDR, BME280_RxBuffer, 1u, 100u);
}
