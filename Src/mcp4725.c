/*
 * mcp4725.c
 *
 *  Created on: Aug 16, 2025
 *      Author: drCsabesz
 */

#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "mcp4725.h"

#define MCP4725_ADDR 0xC0u

void MCP4725_SetOutput(MCP4725_Handle_s* ptrHandler, uint8_t voltage, MCP4725_WriteCmd_e wrCmdType)
{
  uint16_t out_value = (uint16_t)((uint32_t)voltage * 4096u * 100u / ptrHandler->VddValue);

  ptrHandler->TxBuffer[0] = wrCmdType;
  switch (wrCmdType)
  {
    case MCP4725_FAST_MODE:
      ptrHandler->TxBuffer[0u] |= (uint8_t)(out_value & 0xFF00u >> 8u);
      ptrHandler->TxBuffer[1u] |= (uint8_t)(out_value & 0x00FFu);
      ptrHandler->TxBuffer[2u] = ptrHandler->TxBuffer[0u];
      ptrHandler->TxBuffer[3u] = ptrHandler->TxBuffer[1u];
      MCP4725_Write(ptrHandler, 4u);
      break;
    case MCP4725_WRITE_DAC:
      break;
    case MCP4725_WRITE_DAC_EE:
      break;
    default:
      break;
  }
}

void MCP4725_Read(MCP4725_Handle_s* ptrHandler, uint8_t length)
{
  HAL_I2C_Master_Receive(ptrHandler->ptrHI2c, ptrHandler->Address, ptrHandler->RxBuffer, length, 100u);
}

void MCP4725_Write(MCP4725_Handle_s* ptrHandler, uint8_t length)
{
  HAL_I2C_Master_Transmit(ptrHandler->ptrHI2c, ptrHandler->Address, ptrHandler->TxBuffer, length, 100u);
}
