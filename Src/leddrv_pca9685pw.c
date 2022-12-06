/*
 * leddrv_pca9685pw.c
 *
 *  Created on: 2022. jún. 2.
 *      Author: drCsabesz
 */

#include "main.h"
#include "leddrv_pca9685pw.h"

static uint8_t PCA9685_RxBuffer[32u];
static uint8_t PCA9685_TxBuffer[32u];

#define LEDDRV_ADDR 0x80u

void LEDDRV_PCA9685_ReadModeRegs( void )
{
  HAL_GPIO_WritePin(PCA9685_OE_GPIO_Port, PCA9685_OE_Pin, GPIO_PIN_RESET);
  // Set mode register auto increment
  PCA9685_TxBuffer[0u] = 0x00u;
  PCA9685_TxBuffer[1u] = 0x20u;
  //PCA9685_TxBuffer[2u] = 0x00u;
  //PCA9685_TxBuffer[3u] = 0x20u;
  //PCA9685_TxBuffer[4u] = 0x00u;
	HAL_I2C_Master_Transmit(&hi2c1, LEDDRV_ADDR, PCA9685_TxBuffer, 2u, 100u);
	HAL_I2C_Master_Receive(&hi2c1, LEDDRV_ADDR, PCA9685_RxBuffer, 6u, 100u);
	PCA9685_TxBuffer[0u] = 0xFAu;
  HAL_I2C_Master_Transmit(&hi2c1, LEDDRV_ADDR, PCA9685_TxBuffer, 1u, 100u);
  HAL_I2C_Master_Receive(&hi2c1, LEDDRV_ADDR, &PCA9685_RxBuffer[6u], 6u, 100u);
}
