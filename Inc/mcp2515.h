/*
 * mcp2515.h
 *
 *  Created on: Feb 13, 2023
 *      Author: drCsabesz
 */

#ifndef INC_MCP2515_H_
#define INC_MCP2515_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
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
} MCP2515_Handler_t;

typedef enum
{
  Configuration = 0u,
  Normal,
  Sleep,
  ListenOnly,
  Loopback
} MCP2515_OpMode_e;

#ifdef __cplusplus
extern "C" {
#endif

void MCP2515_Init( MCP23S17_Handler_t* ptrHandler );
void MCP2515_Reset( MCP23S17_Handler_t* ptrHandler );
void MCP2515_SetMode( MCP23S17_Handler_t* ptrHandler, MCP23S17_OpMode_e opMode );

#ifdef __cplusplus
}
#endif

#endif /* INC_MCP2515_H_ */
