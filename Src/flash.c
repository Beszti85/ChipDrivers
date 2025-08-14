/*
 * flash.c
 *
 *  Created on: 2022. jún. 1.
 *      Author: drCsabesz
 */

#include "main.h"
#include "flash.h"

#define CMD_WRITE_STATUS1  0x01u
#define CMD_WRITE          0x02u
#define CMD_READ           0x03u
#define CMD_WRITE_DISABLE  0x04u
#define CMD_READ_STATUS1   0x05u
#define CMD_WRITE_ENABLE   0x06u
#define CMD_CHIP_ERASE     0x07u
#define CMD_JEDEC_ID       0x9Fu

#define BUSY               0x01u

static void BusyWait( FLASH_Handler_t* ptrHandler );
static void WriteEnable( FLASH_Handler_t* ptrHandler );

void FLASH_Identification( FLASH_Handler_t* ptrHandler )
{
  // Read Jedec ID
  ptrHandler->TxBuffer[0] = CMD_JEDEC_ID;
  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(ptrHandler->ptrHSpi, ptrHandler->TxBuffer, ptrHandler->RxBuffer, 4u, 100u);
  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_SET);
  // Check Manufacturing ID
  if( (ptrHandler->RxBuffer[1u] == 0xEFu) && (ptrHandler->RxBuffer[2u] == 0x40u) )
  {
    // Winbond flash
    if( ptrHandler->RxBuffer[3u] == 0x16u )
    {
      ptrHandler->DetectedFlash = W25Q32JV_IQ;
      ptrHandler->EndAddress    = 0x003FFFFFu;
    }
    else
    if( ptrHandler->RxBuffer[3u] == 0x17u )
    {
      ptrHandler->DetectedFlash = W25Q64_M;
      ptrHandler->EndAddress    = 0x007FFFFFu;
    }
  }
}

void FLASH_Read( FLASH_Handler_t* ptrHandler, uint32_t flashAddress, uint8_t * const ptrTarget, uint16_t length )
{
  uint16_t count = 0u;
  // Valid source input
  if(   (ptrTarget != NULL)
     && (flashAddress <= ptrHandler->EndAddress)
     && (length != 0u) )
  {
    // prevent flash overflow
    count = length;
    if( flashAddress + length > ptrHandler->EndAddress )
    {
      count = (uint16_t)(ptrHandler->EndAddress - flashAddress);
    }
    // Send command and read response
    ptrHandler->TxBuffer[0u] = CMD_READ;
    ptrHandler->TxBuffer[1u] = (uint8_t)(flashAddress >> 16u);
    ptrHandler->TxBuffer[2u] = (uint8_t)(flashAddress >> 8u);
    ptrHandler->TxBuffer[3u] = (uint8_t)(flashAddress);

    HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(ptrHandler->ptrHSpi, ptrHandler->TxBuffer, ptrTarget, count+4u, 100u);
    HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_SET);
  }
}

void FLASH_Write( FLASH_Handler_t* ptrHandler, uint32_t flashAddress, uint8_t const * const ptrSource, uint16_t length )
{
  uint16_t currLength = 0u;
  uint16_t remainLength = length;

  if(   (flashAddress <= ptrHandler->EndAddress)
     && (remainLength != 0u) )
  {
    if( (flashAddress + remainLength) > ptrHandler->EndAddress )
    {
      // Data to write longer than flash - only write valid range
      remainLength = (uint16_t)(ptrHandler->EndAddress - flashAddress);
    }
    // Data to write smaller than 1 page?
    if( (flashAddress % ptrHandler->PageSize) + remainLength <= ptrHandler->PageSize )
    {
      // Write Page only once
    }
  }
}

void FLASH_Erase( FLASH_Handler_t* ptrHandler )
{
  ptrHandler->TxBuffer[0u] = CMD_CHIP_ERASE;

  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_RESET);
  HAL_SPI_Transmit(ptrHandler->ptrHSpi, ptrHandler->TxBuffer, 1u, 100u);
  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_SET);

  BusyWait(ptrHandler);
}

static void BusyWait( FLASH_Handler_t* ptrHandler )
{
  ptrHandler->TxBuffer[0] = CMD_READ_STATUS1;

  do
  {
    HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(ptrHandler->ptrHSpi, ptrHandler->TxBuffer, ptrHandler->RxBuffer, 1u, 100u);
    HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_SET);
  } while( (ptrHandler->RxBuffer[0] & BUSY) == BUSY );
}

static void WriteEnable( FLASH_Handler_t* ptrHandler )
{
  ptrHandler->TxBuffer[0u] = CMD_WRITE_ENABLE;

  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_RESET);
  HAL_SPI_Transmit(ptrHandler->ptrHSpi, ptrHandler->TxBuffer, 1u, 100u);
  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_SET);

  BusyWait(ptrHandler);
}

static void WritePage( FLASH_Handler_t* ptrHandler, uint32_t flashAddress, uint8_t const * const ptrSource, uint16_t length )
{
  if( ((flashAddress & ptrHandler->PageSize) + length) < ptrHandler->PageSize )
  {
    WriteEnable(ptrHandler);
    // Send command and read response
    ptrHandler->TxBuffer[0u] = CMD_READ;
    ptrHandler->TxBuffer[1u] = (uint8_t)(flashAddress >> 16u);
    ptrHandler->TxBuffer[2u] = (uint8_t)(flashAddress >> 8u);
    ptrHandler->TxBuffer[3u] = (uint8_t)(flashAddress);
    // Write SPI: flashAddress, then data
    HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_RESET);
    HAL_SPI_Transmit(ptrHandler->ptrHSpi, ptrHandler->TxBuffer, 4u, 100u);
    HAL_SPI_Transmit(ptrHandler->ptrHSpi, ptrSource, length, 1000u);
    HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_SET);

    BusyWait(ptrHandler);
  }
}
