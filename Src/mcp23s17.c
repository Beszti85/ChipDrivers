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

uint8_t MCP23S17_TxBuffer[16u];
uint8_t MCP23S17_RxBuffer[16u];
static uint8_t MCP23S17_Available = 0u;

extern SPI_HandleTypeDef hspi1;

static void SpiWrite( uint8_t length );
static void SpiRead( uint8_t length );

void MCP23S17_Init( void )
{
  MCP23S17_ReadAllRegs();
  if(   ( MCP23S17_RxBuffer[1] == 0xFFu )
     && ( MCP23S17_RxBuffer[2] == 0xFFu ) )
  {
    MCP23S17_Available = 1u;
  }
}

void MCP23S17_ReadAllRegs( void )
{
  MCP23S17_TxBuffer[1] = 0x41u;
  MCP23S17_TxBuffer[3] = 0x00u;
  SpiRead( 23u );
}

void MCP23S17_SetIODirectionA( uint8_t value )
{
  MCP23S17_TxBuffer[1u] = REGADDR_IODIRA;
  MCP23S17_TxBuffer[2u] = value;
  SpiWrite(3u);
}

void MCP23S17_SetIODirectionB( uint8_t value )
{
  MCP23S17_TxBuffer[1u] = REGADDR_IODIRB;
  MCP23S17_TxBuffer[2u] = value;
  SpiWrite(3u);
}

void MCP23S17_SetIODirectionAB( uint8_t portAValue, uint8_t portBValue )
{
  MCP23S17_TxBuffer[1u] = REGADDR_IODIRA;
  MCP23S17_TxBuffer[2u] = portAValue;
  MCP23S17_TxBuffer[3u] = portBValue;
  SpiWrite(4u);
}

void MCP23S17_WriteOutputA( uint8_t value )
{
  MCP23S17_TxBuffer[1u] = REGADDR_OLATA;
  MCP23S17_TxBuffer[2u] = value;
  SpiWrite(3u);
}

void MCP23S17_WriteOutputB( uint8_t value )
{
  MCP23S17_TxBuffer[1u] = REGADDR_OLATB;
  MCP23S17_TxBuffer[2u] = value;
  SpiWrite(3u);
}

void MCP23S17_WriteOutputAB( uint8_t portAValue, uint8_t portBValue )
{
  MCP23S17_TxBuffer[1u] = REGADDR_OLATA;
  MCP23S17_TxBuffer[2u] = portAValue;
  MCP23S17_TxBuffer[3u] = portBValue;
  SpiWrite(4u);
}

static void SpiWrite( uint8_t length )
{
  // Set write address
  MCP23S17_TxBuffer[0] = 0x40u;
  HAL_GPIO_WritePin(CS_MCP23S17_GPIO_Port, CS_MCP23S17_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, MCP23S17_TxBuffer, length, 100u);
  HAL_GPIO_WritePin(CS_MCP23S17_GPIO_Port, CS_MCP23S17_Pin, GPIO_PIN_SET);
}

static void SpiRead( uint8_t length )
{
  // Set Read address
  MCP23S17_TxBuffer[0] = 0x41u;
  HAL_GPIO_WritePin(CS_MCP23S17_GPIO_Port, CS_MCP23S17_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi1, MCP23S17_TxBuffer, MCP23S17_RxBuffer, length, 100u);
  HAL_GPIO_WritePin(CS_MCP23S17_GPIO_Port, CS_MCP23S17_Pin, GPIO_PIN_SET);
}
