/*
 * mcp2515.c
 *
 *  Created on: Feb 13, 2023
 *      Author: drCsabesz
 */

#include "mcp2515.h"

static void SpiWrite( MCP2515_Handler_t* ptrHandler, uint8_t length );
static void SpiRead( MCP2515_Handler_t* ptrHandler, uint8_t length );

#define CMD_RESET       0xC0u
#define CMD_READ        0x03u
#define CMD_READ_RX_BUF 0x90u
#define CMD_WRITE       0x02u
#define CMD_LOAD_TX_BUF 0x40u
#define CMD_RTS         0x80u
#define CMD_READ_STATUS 0xA0u
#define CMD_RX_STATUS   0xB0u
#define CMD_BIT_MODIFY  0x05u

#define REG_CANCTRL     0x0Fu

void MCP2515_Init( MCP2515_Handler_t* ptrHandler )
{
  MCP2515_Reset( ptrHandler );
}

void MCP2515_Reset( MCP2515_Handler_t* ptrHandler )
{
  ptrHandler->TxBuffer[0u] = CMD_RESET;
  SpiWrite( ptrHandler, 1u );
}

void MCP2515_SetMode( MCP2515_Handler_t* ptrHandler, MCP2515_OpMode_e opMode )
{
  ptrHandler->TxBuffer[0u] = CMD_WRITE;
  ptrHandler->TxBuffer[1u] = REG_CANCTRL;
  ptrHandler->TxBuffer[2u] = REG_CANCTRL;
  SpiWrite( ptrHandler, 1u );
}

void MCP2515_ReadStatus( MCP2515_Handler_t* ptrHandler )
{
  ptrHandler->TxBuffer[0u] = CMD_READ_STATUS;
  SpiRead( ptrHandler, 3u );
}

static void SpiWrite( MCP2515_Handler_t* ptrHandler, uint8_t length )
{
  // Set write address
  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_RESET);
#if (HAL_SPI_DMA == 1)
  HAL_SPI_Transmit_DMA(ptrHandler->ptrHSpi, ptrHandler->TxBuffer, length);
#else
  HAL_SPI_Transmit(ptrHandler->ptrHSpi, ptrHandler->TxBuffer, length, 100u);
#endif
  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_SET);
}

static void SpiRead( MCP2515_Handler_t* ptrHandler, uint8_t length )
{
  // Set Read address
  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_RESET);
#if (HAL_SPI_DMA == 1)
  HAL_SPI_TransmitReceive(ptrHandler->ptrHSpi, ptrHandler->TxBuffer, ptrHandler->RxBuffer, length);
#else
  HAL_SPI_TransmitReceive(ptrHandler->ptrHSpi, ptrHandler->TxBuffer, ptrHandler->RxBuffer, length, 100u);
#endif
  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_SET);
}
