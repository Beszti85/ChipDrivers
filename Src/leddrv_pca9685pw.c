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

#define LEDDRV_ADDR 0x00u

void LEDDRV_PCA9685_ReadModeRegs( void )
{
	HAL_I2C_Master_Transmit(&hi2c1, LEDDRV_ADDR, PCA9685_TxBuffer, 1u, 100u);
	HAL_I2C_Master_Receive(&hi2c1, LEDDRV_ADDR, PCA9685_RxBuffer, 2u, 100u);
}
