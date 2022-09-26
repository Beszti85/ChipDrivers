/*
 * flash.h
 *
 *  Created on: 2022. jún. 1.
 *      Author: drCsabesz
 */

typedef enum
{
  FLASH_UNKNOWN = 0u,
  W25Q64_M,
  W25Q32JV_IQ,
  W25Q32JV_IM,
  NUM_SUPPORTED_DEVICES
} FLASH_W25_Type_e;

void FLASH_W25_Identification( void );
