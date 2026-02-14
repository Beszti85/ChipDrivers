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

uint8_t MCP4725_RxBuffer[6u];
uint8_t MCP4725_TxBuffer[6u];

void MCP4725_Read(MCP4725_Handle_s* ptrHandler)
{
  HAL_I2C_Master_Receive(ptrHandler->ptrHI2c, MCP4725_ADDR, ptrHandler->RxBuffer, 6u, 100u);
}

void MCP4725_Write(MCP4725_Handle_s* ptrHandler)
{
  HAL_I2C_Master_Transmit(ptrHandler->ptrHI2c, MCP4725_ADDR, ptrHandler->TxBuffer, 6u, 100u);
}
