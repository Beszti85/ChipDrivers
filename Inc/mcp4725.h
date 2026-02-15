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
  uint16_t VddValue;   //!< Power supply value in 2 decimal points (x100)
  uint8_t Address;
} MCP4725_Handle_s;

typedef enum
{
  MCP4725_FAST_MODE    = 0u,
  MCP4725_WRITE_DAC    = 0x40u,
  MCP4725_WRITE_DAC_EE = 0x60u
} MCP4725_WriteCmd_e;

void MCP4725_Read(MCP4725_Handle_s* ptrHandler, uint8_t length);
void MCP4725_Write(MCP4725_Handle_s* ptrHandler, uint8_t length);

#endif /* INC_MCP4725_H_ */
