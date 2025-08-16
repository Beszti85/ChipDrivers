/*
 * mcp4725.c
 *
 *  Created on: Aug 16, 2025
 *      Author: drCsabesz
 */

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

#include "main.h"
#include "mcp23s17.h"

#define MCP4725_ADDR 0xC0u

uint8_t MCP4725_RxBuffer[6u];
uint8_t MCP4725_TxBuffer[6u];

void MCP4725_Read(I2C_HandleTypeDef* ptrI2C)
{
  HAL_I2C_Master_Receive(ptrI2C, MCP4725_ADDR, MCP4725_RxBuffer, 6u, 100u);
}
