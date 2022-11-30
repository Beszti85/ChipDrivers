/*
 * pcf8574.h
 *
 *  Created on: 2022. jún. 15.
 *      Author: drCsabesz
 */

#ifndef INC_PCF8574_H_
#define INC_PCF8574_H_

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

typedef struct
{
  I2C_HandleTypeDef* ptrHI2c;
  uint8_t Address;
  uint8_t Available;
  uint8_t Padding[2u];
} PCF8574_Handler_t;

uint8_t PCF8574_ReadPort( PCF8574_Handler_t* ptrHandler );
void PCF8574_WritePort( PCF8574_Handler_t* ptrHandler, uint8_t value );
void PCF8574_WritePin( PCF8574_Handler_t* ptrHandler, uint8_t pin, uint8_t value );
bool PCF8574_ReadPin( PCF8574_Handler_t* ptrHandler, uint8_t pin );

#endif /* INC_PCF8574_H_ */
