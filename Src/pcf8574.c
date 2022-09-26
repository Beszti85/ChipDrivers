/*
 * pcf8574.c
 *
 *  Created on: 2022. jún. 15.
 *      Author: drCsabesz
 */

#include "main.h"
#include "pcf8574.h"

#define PCF8574_ADDR  0x76u

extern I2C_HandleTypeDef hi2c1;

static uint8_t Pcf8574IoVal = 0u;

uint8_t PCF8574_ReadPort( void )
{
  HAL_I2C_Master_Receive(&hi2c1, PCF8574_ADDR, &Pcf8574IoVal, 1u, 100u);

  return Pcf8574IoVal;
}

void PCF8574_WritePort( uint8_t value )
{
  Pcf8574IoVal = value;

  HAL_I2C_Master_Transmit(&hi2c1, PCF8574_ADDR, &Pcf8574IoVal, 1u, 100u);
}

bool PCF8574_ReadPin( uint8_t pin )
{
  bool retval = false;

  HAL_I2C_Master_Receive(&hi2c1, PCF8574_ADDR, &Pcf8574IoVal, 1u, 100u);

  if( Pcf8574IoVal & (1 << pin) )
  {
    retval = true;
  }

  return retval;
}

void PCF8574_WritePin( uint8_t pin, uint8_t value )
{
  // Read actual value
  HAL_I2C_Master_Receive(&hi2c1, PCF8574_ADDR, &Pcf8574IoVal, 1u, 100u);
  // Set it
  if( value == 0u )
  {
    Pcf8574IoVal &= ~(1 << pin);
  }
  else
  {
    Pcf8574IoVal |= (1 << pin);
  }

  HAL_I2C_Master_Transmit(&hi2c1, PCF8574_ADDR, &Pcf8574IoVal, 1u, 100u);
}
