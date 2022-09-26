/*
 * lcd_6pin_mcp23s17.h
 *
 *  Created on: 2022. júl. 30.
 *      Author: drCsabesz
 */

#ifndef INC_LCD_6PIN_MCP23S17_H_
#define INC_LCD_6PIN_MCP23S17_H_

#ifdef ATMEL_PROJECT
#include "mcu_global.h"
#include "my_typedef.h"
#else
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

#define DELAY_MS(x)   HAL_Delay(x)
#endif

void LcdPortInit(void);
void LcdWrite(uint8_t data, uint8_t rs);

#endif /* INC_LCD_6PIN_MCP23S17_H_ */
