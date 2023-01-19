/*
 * pca9685pw.c
 *
 *  Created on: 2022. jún. 2.
 *      Author: drCsabesz
 */

#include "main.h"
#include "pca9685pw.h"

void PCA9685_ReadModeRegs( PCA9685_Handler_t* ptrHandler )
{
  HAL_GPIO_WritePin(ptrHandler->portOE, ptrHandler->pinOE, GPIO_PIN_RESET);
  // Set mode register auto increment
  ptrHandler->TxBuffer[0u] = 0x00u;
  ptrHandler->TxBuffer[1u] = 0x20u;
  // Set output logic state to inverted
  ptrHandler->TxBuffer[2u] = 0x10u;
  //PCA9685_TxBuffer[3u] = 0x20u;
  //PCA9685_TxBuffer[4u] = 0x00u;
	HAL_I2C_Master_Transmit(ptrHandler->ptrHI2c, ptrHandler->Address, ptrHandler->TxBuffer, 3u, 100u);
	HAL_I2C_Master_Receive(ptrHandler->ptrHI2c, ptrHandler->Address, ptrHandler->RxBuffer, 6u, 100u);
	ptrHandler->TxBuffer[0u] = 0xFAu;
  HAL_I2C_Master_Transmit(ptrHandler->ptrHI2c, ptrHandler->Address, ptrHandler->TxBuffer, 1u, 100u);
  HAL_I2C_Master_Receive(ptrHandler->ptrHI2c, ptrHandler->Address, &ptrHandler->RxBuffer[6u], 6u, 100u);
}
