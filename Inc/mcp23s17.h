/*
 * mcp23s17_spi.h
 *
 *  Created on: 2022. jún. 20.
 *      Author: drCsabesz
 */

#ifndef INC_MCP23S17_H_
#define INC_MCP23S17_H_

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

typedef struct
{
  SPI_HandleTypeDef* ptrHSpi;
  GPIO_TypeDef* portCS;
  uint16_t pinCS;
  uint8_t TxBuffer[24u];
  uint8_t RxBuffer[24u];
  uint8_t Available;
  uint8_t Padding[3u];
} MCP23S17_Handler_t;

void MCP23S17_Init( MCP23S17_Handler_t* ptrHandler );
void MCP23S17_SetIODirectionAB( MCP23S17_Handler_t* ptrHandler, uint8_t portAValue, uint8_t portBValue );
void MCP23S17_WriteOutputA( MCP23S17_Handler_t* ptrHandler, uint8_t value );
void MCP23S17_WriteOutputB( MCP23S17_Handler_t* ptrHandler, uint8_t value );
void MCP23S17_ReadAllRegs( MCP23S17_Handler_t* ptrHandler );
uint8_t MCP23S17_ReadPortA( MCP23S17_Handler_t* ptrHandler );
void MCP23S17_WritePortA( MCP23S17_Handler_t* ptrHandler, uint8_t value );
uint8_t MCP23S17_ReadPortA( MCP23S17_Handler_t* ptrHandler );
void MCP23S17_WritePortB( MCP23S17_Handler_t* ptrHandler, uint8_t value );
void MCP23S17_WritePortAB( MCP23S17_Handler_t* ptrHandler, uint8_t portAValue, uint8_t portBValue );
void MCP23S17_WritePinA( MCP23S17_Handler_t* ptrHandler, uint8_t pin, uint8_t value );
void MCP23S17_WritePinB( MCP23S17_Handler_t* ptrHandler, uint8_t pin, uint8_t value );

#endif /* INC_MCP23S17_H_ */
