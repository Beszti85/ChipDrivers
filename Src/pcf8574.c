/*
 * pcf8574.c
 *
 *  Created on: 2022. jún. 15.
 *      Author: drCsabesz
 */

#include "main.h"
#include "pcf8574.h"

static uint8_t Pcf8574IoVal = 0u;

uint8_t PCF8574_ReadPort( PCF8574_Handler_t* ptrHandler )
{
  uint8_t addrValue = ptrHandler->Address | 0x01u;

  HAL_I2C_Master_Receive(ptrHandler->ptrHI2c, addrValue, &Pcf8574IoVal, 1u, 100u);

  return Pcf8574IoVal;
}

void PCF8574_WritePort( PCF8574_Handler_t* ptrHandler, uint8_t value )
{
  Pcf8574IoVal = value;

  HAL_I2C_Master_Transmit(ptrHandler->ptrHI2c, ptrHandler->Address, &Pcf8574IoVal, 1u, 100u);
}

bool PCF8574_ReadPin( PCF8574_Handler_t* ptrHandler, uint8_t pin )
{
  bool retval = false;
  uint8_t addrValue = ptrHandler->Address | 0x01u;

  HAL_I2C_Master_Receive(ptrHandler->ptrHI2c, addrValue, &Pcf8574IoVal, 1u, 100u);

  if( Pcf8574IoVal & (1 << pin) )
  {
    retval = true;
  }

  return retval;
}

void PCF8574_WritePin( PCF8574_Handler_t* ptrHandler, uint8_t pin, uint8_t value )
{
  uint8_t addrValue = ptrHandler->Address | 0x01u;
  // Read actual value
  HAL_I2C_Master_Receive(ptrHandler->ptrHI2c, addrValue, &Pcf8574IoVal, 1u, 100u);
  // Set it
  if( value == 0u )
  {
    Pcf8574IoVal &= ~(1 << pin);
  }
  else
  {
    Pcf8574IoVal |= (1 << pin);
  }

  HAL_I2C_Master_Transmit(ptrHandler->ptrHI2c, ptrHandler->Address, &Pcf8574IoVal, 1u, 100u);
}
