/*
 * mcp4725.h
 *
 *  Created on: Aug 16, 2025
 *      Author: drCsabesz
 */

#ifndef INC_MCP4725_H_
#define INC_MCP4725_H_

#include "main.h"

typedef struct
{
  I2C_HandleTypeDef* ptrHI2c;
  uint8_t TxBuffer[6u];
  uint8_t RxBuffer[6u];
  uint8_t Address;
  uint8_t VddValue;
} MCP4725_Handle_s;

void MCP4725_Read(MCP4725_Handle_s* ptrHandler);
void MCP4725_Write(MCP4725_Handle_s* ptrHandler);

#endif /* INC_MCP4725_H_ */
