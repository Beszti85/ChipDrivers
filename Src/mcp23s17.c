/*
 * mcp23s17_spi.c
 *
 *  Created on: 2022. jún. 20.
 *      Author: drCsabesz
 */

#include "main.h"
#include "mcp23s17.h"

// DEfault settings: BANK = 0
#define REGADDR_IODIRA   0x00u
#define REGADDR_IODIRB   0x01u
#define REGADDR_IPOLA    0x02u
#define REGADDR_IPOLB    0x03u
#define REGADDR_GPINTENA 0x04u
#define REGADDR_GPINTENB 0x05u
#define REGADDR_DEFVALA  0x06u
#define REGADDR_DEFVALB  0x07u
#define REGADDR_INTCONA  0x08u
#define REGADDR_INTCONB  0x09u
#define REGADDR_IOCONA   0x0Au
#define REGADDR_IOCONB   0x0Bu
#define REGADDR_GPPUA    0x0Cu
#define REGADDR_GPPUB    0x0Du
#define REGADDR_INTFA    0x0Eu
#define REGADDR_INTFB    0x0Fu
#define REGADDR_INTCAPA  0x10u
#define REGADDR_INTCAPB  0x11u
#define REGADDR_GPIOA    0x12u
#define REGADDR_GPIOB    0x13u
#define REGADDR_OLATA    0x14u
#define REGADDR_OLATB    0x15u

static void SpiWrite( MCP23S17_Handler_t* ptrHandler, uint8_t length );
static void SpiRead( MCP23S17_Handler_t* ptrHandler, uint8_t length );

void MCP23S17_Init( MCP23S17_Handler_t* ptrHandler )
{
  MCP23S17_ReadAllRegs(ptrHandler);
  if(   ( ptrHandler->RxBuffer[1] == 0xFFu )
     && ( ptrHandler->RxBuffer[2] == 0xFFu ) )
  {
    ptrHandler->Available = 1u;
  }
}

void MCP23S17_ReadAllRegs( MCP23S17_Handler_t* ptrHandler )
{
  ptrHandler->TxBuffer[1] = 0x41u;
  ptrHandler->TxBuffer[3] = 0x00u;
  SpiRead( ptrHandler, 23u );
}

void MCP23S17_SetIODirectionA( MCP23S17_Handler_t* ptrHandler, uint8_t value )
{
  ptrHandler->TxBuffer[1u] = REGADDR_IODIRA;
  ptrHandler->TxBuffer[2u] = value;
  SpiWrite( ptrHandler, 3u );
}

void MCP23S17_SetIODirectionB( MCP23S17_Handler_t* ptrHandler, uint8_t value )
{
  ptrHandler->TxBuffer[1u] = REGADDR_IODIRB;
  ptrHandler->TxBuffer[2u] = value;
  SpiWrite( ptrHandler, 3u );
}

void MCP23S17_SetIODirectionAB( MCP23S17_Handler_t* ptrHandler, uint8_t portAValue, uint8_t portBValue )
{
  ptrHandler->TxBuffer[1u] = REGADDR_IODIRA;
  ptrHandler->TxBuffer[2u] = portAValue;
  ptrHandler->TxBuffer[3u] = portBValue;
  SpiWrite( ptrHandler, 4u );
}

uint8_t MCP23S17_ReadPortA( MCP23S17_Handler_t* ptrHandler )
{
  ptrHandler->TxBuffer[1u] = REGADDR_GPIOA;
  SpiRead( ptrHandler, 3u );
  return ptrHandler->RxBuffer[2u];
}

void MCP23S17_WritePortA( MCP23S17_Handler_t* ptrHandler, uint8_t value )
{
  ptrHandler->TxBuffer[1u] = REGADDR_OLATA;
  ptrHandler->TxBuffer[2u] = value;
  SpiWrite( ptrHandler, 3u );
}

uint8_t MCP23S17_ReadPortB( MCP23S17_Handler_t* ptrHandler )
{
  ptrHandler->TxBuffer[1u] = REGADDR_GPIOB;
  SpiRead( ptrHandler, 3u );
  return ptrHandler->RxBuffer[2u];
}

void MCP23S17_WritePortB( MCP23S17_Handler_t* ptrHandler, uint8_t value )
{
  ptrHandler->TxBuffer[1u] = REGADDR_OLATB;
  ptrHandler->TxBuffer[2u] = value;
  SpiWrite( ptrHandler, 3u );
}

void MCP23S17_WritePortAB( MCP23S17_Handler_t* ptrHandler, uint8_t portAValue, uint8_t portBValue )
{
  ptrHandler->TxBuffer[1u] = REGADDR_OLATA;
  ptrHandler->TxBuffer[2u] = portAValue;
  ptrHandler->TxBuffer[3u] = portBValue;
  SpiWrite( ptrHandler, 4u );
}

void MCP23S17_WritePinA( MCP23S17_Handler_t* ptrHandler, uint8_t pin, uint8_t value )
{
  uint8_t portValue = 0u;
  ptrHandler->TxBuffer[1u] = REGADDR_GPIOA;
  SpiRead( ptrHandler, 3u );
  portValue = ptrHandler->RxBuffer[2u];
  if( 0u == value)
  {
    portValue &= ~(1 << pin);
  }
  else
  {
    portValue |= (1 << pin);
  }
  ptrHandler->TxBuffer[1u] = REGADDR_OLATA;
  ptrHandler->TxBuffer[2u] = portValue;
  SpiWrite( ptrHandler, 3u );
}

void MCP23S17_WritePinB( MCP23S17_Handler_t* ptrHandler, uint8_t pin, uint8_t value )
{
  uint8_t portValue = 0u;
  ptrHandler->TxBuffer[1u] = REGADDR_GPIOB;
  SpiRead( ptrHandler, 3u );
  portValue = ptrHandler->RxBuffer[2u];
  if( 0u == value)
  {
    portValue &= ~(1 << pin);
  }
  else
  {
    portValue |= (1 << pin);
  }
  ptrHandler->TxBuffer[1u] = REGADDR_OLATB;
  ptrHandler->TxBuffer[2u] = portValue;
  SpiWrite( ptrHandler, 3u );
}

static void SpiWrite( MCP23S17_Handler_t* ptrHandler, uint8_t length )
{
  // Set write address
  ptrHandler->TxBuffer[0] = 0x40u;
  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_RESET);
  HAL_SPI_Transmit(ptrHandler->ptrHSpi, ptrHandler->TxBuffer, length, 100u);
  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_SET);
}

static void SpiRead( MCP23S17_Handler_t* ptrHandler, uint8_t length )
{
  // Set Read address
  ptrHandler->TxBuffer[0] = 0x41u;
  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(ptrHandler->ptrHSpi, ptrHandler->TxBuffer, ptrHandler->RxBuffer, length, 100u);
  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_SET);
}
