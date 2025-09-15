/*
 * flash.h
 *
 *  Created on: 2022. jún. 1.
 *      Author: drCsabesz
 */

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

typedef enum
{
  FLASH_UNKNOWN = 0u,
  W25Q64_M,
  W25Q32JV_IQ,
  W25Q32JV_IM,
  NUM_SUPPORTED_DEVICES
} FLASH_Type_e;

typedef struct
{
  SPI_HandleTypeDef*  ptrHSpi;
  GPIO_TypeDef*       portCS;
  uint16_t            pinCS;
  FLASH_Type_e        DetectedFlash;
  uint32_t            EndAddress;
  uint8_t             PageSize;
  uint8_t             TxBuffer[8u];
  uint8_t             RxBuffer[8u];
} FLASH_Handler_t;

void FLASH_Identification( FLASH_Handler_t* ptrHandler );
void FLASH_Read( FLASH_Handler_t* ptrHandler, uint32_t flashAddress, uint8_t * const ptrTarget, uint16_t length );
void FLASH_Write( FLASH_Handler_t* ptrHandler, uint32_t flashAddress, uint8_t const * const ptrSource, uint16_t length );
void FLASH_Erase( FLASH_Handler_t* ptrHandler );

#endif /* INC_FLASH_H_ */
