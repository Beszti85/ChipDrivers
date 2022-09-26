/*
 * flash.c
 *
 *  Created on: 2022. jún. 1.
 *      Author: drCsabesz
 */

#include "main.h"
#include "flash.h"

uint8_t FLASH_TxBuffer[32u];
uint8_t FLASH_RxBuffer[32u];
uint8_t FLASH_Detected = 0u;
FLASH_W25_Type_e FLASH_Type = FLASH_UNKNOWN;

extern SPI_HandleTypeDef hspi1;

void FLASH_Identification( void )
{
  // Read Jedec ID
  FLASH_TxBuffer[0] = 0x9Fu;
  HAL_GPIO_WritePin(CS_FLASH_GPIO_Port, CS_FLASH_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi1, FLASH_TxBuffer, FLASH_RxBuffer, 4u, 100u);
  HAL_GPIO_WritePin(CS_FLASH_GPIO_Port, CS_FLASH_Pin, GPIO_PIN_SET);
  // Check Manufacturing ID
  if( (FLASH_RxBuffer[1u] == 0xEFu) && (FLASH_RxBuffer[2u] == 0x40u) )
  {
    FLASH_Detected = 1u;
    // Winbond flash
    if( FLASH_RxBuffer[3u] == 0x16u )
    {
      FLASH_Type = W25Q32JV_IQ;
    }
    else
    if( FLASH_RxBuffer[3u] == 0x17u )
    {
      FLASH_Type = W25Q64_M;
    }
  }
}


void FLASH_Read( void )
{

}

void FLASH_Write( void )
{

}
