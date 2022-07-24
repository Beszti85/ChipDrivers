/*
 * lcd_6pin_port.h
 *
 *  Created on: Jul 23, 2022
 *      Author: cbeszter
 */

#ifndef INC_LCD_6PIN_PORT_H_
#define INC_LCD_6PIN_PORT_H_

#ifdef ATMEL_PROJECT
#include "mcu_global.h"
#include "my_typedef.h"
#else
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#endif

/************************************************************
 *
 * HW Dependent Part - adapt to actual circuit and display
 *
 ************************************************************/

#ifdef ATMEL_PROJECT
 /* LCD PORTS */
#define LCD_RS_PORT PORTB
#define LCD_RS      5
#define LCD_E_PORT  PORTB
#define LCD_E       4
#define LCD_D_PORT  PORTB
#define LCD_D4      3
#define LCD_D5      2
#define LCD_D6      1
#define LCD_D7      0

#define LCD_DATA_CLEAR 0xF0
#define LCD_PORT_CLEAR 0xC0
#define LCD_DATA_HIGH  0x0F
#endif
/*
 * Definitions for Display Size
 * Change these definitions to adapt setting to your display
 */
#define LCD_LINES           2     /**< number of visible lines of the display */
#define LCD_DISP_LENGTH    16     /**< visibles characters per line of the display */
#define LCD_LINE_LENGTH  0x40     /**< internal line length of the display    */
#define LCD_START_LINE1  0x00     /**< DDRAM address of first char of line 1 */
#define LCD_START_LINE2  0x40     /**< DDRAM address of first char of line 2 */
#define LCD_START_LINE3  0x14     /**< DDRAM address of first char of line 3 */
#define LCD_START_LINE4  0x54     /**< DDRAM address of first char of line 4 */
#define LCD_WRAP_LINES      0     /**< 0: no wrap, 1: wrap at end of visibile line */

/************************************************************/

/* Global defines */
#ifdef ATMEL_PROJECT
#define LCD_RS_1()    (LCD_RS_PORT |= (1 << LCD_RS))
#define LCD_RS_0()    (LCD_RS_PORT &= (~(1 << LCD_RS)))

#define LCD_E_1()     (LCD_E_PORT |= (1 << LCD_E))
#define LCD_E_0()     (LCD_E_PORT &= (~(1 << LCD_E)))

#define LCD_PORT_0()  (LCD_D_PORT &= LCD_PORT_CLEAR)

#define LCD_DATA_0()  (LCD_D_PORT &= LCD_DATA_CLEAR)

#define LCD_DATA_H()  (LCD_D_PORT |= LCD_DATA_HIGH)

#define LCD_D4_1()    (LCD_D_PORT |= (1 << LCD_D4))
#define LCD_D4_0()    (LCD_D_PORT &= (~(1 << LCD_D4)))
#define LCD_D5_1()    (LCD_D_PORT |= (1 << LCD_D5))
#define LCD_D5_0()    (LCD_D_PORT &= (~(1 << LCD_D5)))
#define LCD_D6_1()    (LCD_D_PORT |= (1 << LCD_D6))
#define LCD_D6_0()    (LCD_D_PORT &= (~(1 << LCD_D6)))
#define LCD_D7_1()    (LCD_D_PORT |= (1 << LCD_D7))
#define LCD_D7_0()    (LCD_D_PORT &= (~(1 << LCD_D7)))

#define DELAY_MS(x)   delay_msec(x)
#else

#define LCD_RS_1()    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET)
#define LCD_RS_0()    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);

#define LCD_E_1()     HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_SET)
#define LCD_E_0()     HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET)

#define LCD_PORT_0()  ClearPort()

#define LCD_DATA_0()  ClearData()

#define LCD_DATA_H()  (LCD_D_PORT |= LCD_DATA_HIGH)

#define LCD_D4_1()    HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, GPIO_PIN_SET)
#define LCD_D4_0()    HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, GPIO_PIN_RESET)
#define LCD_D5_1()    HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, GPIO_PIN_SET)
#define LCD_D5_0()    HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, GPIO_PIN_RESET)
#define LCD_D6_1()    HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, GPIO_PIN_SET)
#define LCD_D6_0()    HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, GPIO_PIN_RESET)
#define LCD_D7_1()    HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, GPIO_PIN_SET)
#define LCD_D7_0()    HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, GPIO_PIN_RESET)

#define DELAY_MS(x)   HAL_Delay(x)
#endif

#endif /* INC_LCD_6PIN_PORT_H_ */
