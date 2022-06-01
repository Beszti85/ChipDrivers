/*
 * flash_w25.c
 *
 *  Created on: 2022. jún. 1.
 *      Author: drCsabesz
 */
#include "stm32f4xx_hal.h"
#include "main.h"
#include "flash_w25.h"

uint8_t FLASH_TxBuffer[32u];
uint8_t FLASH_RxBuffer[32u];

void FLASH_W25_Identification( void )
{
  // Read Jedec ID
  FLASH_TxBuffer[0] = 0x9Fu;
  HAL_GPIO_WritePin(CS_FLASH_GPIO_Port, CS_FLASH_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi1, FLASH_TxBuffer, FLASH_RxBuffer, 4u, 100u);
  HAL_GPIO_WritePin(CS_FLASH_GPIO_Port, CS_FLASH_Pin, GPIO_PIN_SET);
}
