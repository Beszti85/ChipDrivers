/*
 * lcd_6pin_pcf8574.h
 *
 *  Created on: 2022. nov. 30.
 *      Author: drCsabesz
 */

#ifndef INC_LCD_6PIN_PCF8574_H_
#define INC_LCD_6PIN_PCF8574_H_

#ifdef ATMEL_PROJECT
#include "mcu_global.h"
#include "my_typedef.h"
#else
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

#define DELAY_MS(x)   HAL_Delay(x)
#endif

void LcdPortInit_PCF8574( I2C_HandleTypeDef* ptrI2c, uint8_t address );
void LcdWrite(uint8_t data, uint8_t rs);

#endif /* INC_LCD_6PIN_PCF8574_H_ */
