/*
 * lcd_6pin_pcf8574.c
 *
 *  Created on: 2022. nov. 30.
 *      Author: drCsabesz
 */

#include "lcd_6pin_pcf8574.h"
#include "pcf8574.h"

#define PIN_D7 7u
#define PIN_D6 6u
#define PIN_D5 5u
#define PIN_D4 4u
#define PIN_E  2u
#define PIN_RS 3u

static PCF8574_Handler_t ChipHandler;

/********************************************
* Character LCD driver library
* 4 bit mode, R/W connected to ground
*********************************************/

static void ClearPort(void);
static void WrieDataPins(uint8_t data);
static void LcdToggleE(void);

/***********************************************
*
* Toggling the LCD Enable Pin
*
************************************************/
static void LcdToggleE(void)
{
  PCF8574_WritePin(&ChipHandler, PIN_E, 1);
  DELAY_MS(1);
  PCF8574_WritePin(&ChipHandler, PIN_E, 0);
  DELAY_MS(1);
}


/***********************************************
*
* Write data to LCD port in 4bit mode
*
************************************************/
void LcdWrite(uint8_t data, uint8_t rs)
{
  uint8_t tempData = 0u;

  if (rs == 1)
  {
    PCF8574_WritePin(&ChipHandler, PIN_RS, 1);
  }
  else
  {
    PCF8574_WritePin(&ChipHandler, PIN_RS, 0);
  }
  // set data port value
  if (data & 0x80)
  {
    tempData |= (1 << PIN_D7);
  }
  if (data & 0x40)
  {
    tempData |= (1 << PIN_D6);
  }
  if (data & 0x20)
  {
    tempData |= (1 << PIN_D5);
  }
  if (data & 0x10)
  {
    tempData |= (1 << PIN_D4);
  }
  /* write high nibble */
  WrieDataPins(tempData);
  LcdToggleE();

  /* write low nibble */
  // set data port value
  tempData = 0u;
  if (data & 0x08)
  {
    tempData |= (1 << PIN_D7);
  }
  if (data & 0x04)
  {
    tempData |= (1 << PIN_D6);
  }
  if (data & 0x02)
  {
    tempData |= (1 << PIN_D5);
  }
  if (data & 0x01)
  {
    tempData |= (1 << PIN_D4);
  }
  WrieDataPins(tempData);
  LcdToggleE();

  DELAY_MS(1);
}


/***********************************************
*
* LCD Port initialisation
*
* PORT DIR initialisation done in global board
* Initialize display and select type of cursor
* Input:    dispMode LCD_DISP_OFF            display off
*                    LCD_DISP_ON             display on, cursor off
*                    LCD_DISP_ON_CURSOR      display on, cursor on
*                    LCD_DISP_CURSOR_BLINK   display on, cursor on flashing
* Returns:  none
*
***********************************************/

void LcdPortInit_PCF8574( I2C_HandleTypeDef* ptrI2c, uint8_t address )
{
  ChipHandler.ptrHI2c = ptrI2c;
  ChipHandler.Address = address;

  PCF8574_WritePort(&ChipHandler, 0x00u);

  DELAY_MS(100);

  // Write D4-D5 1
  WrieDataPins(0x30u);

  LcdToggleE();

  DELAY_MS(6);

  LcdToggleE();

  DELAY_MS(1);

  LcdToggleE();

  DELAY_MS(1);

  // Write D5 1
  WrieDataPins(0x20u);

  LcdToggleE();

  DELAY_MS(6);
}

static void ClearPort(void)
{
  uint8_t portValue = 0u;
  // read data port
  portValue = PCF8574_ReadPort(&ChipHandler);
  // clear port bits
  portValue &= 0x03u;
  // write dat port
  PCF8574_WritePort(&ChipHandler, portValue);
}

static void WrieDataPins(uint8_t data)
{
  uint8_t portValue = 0u;
  // read data port
  portValue = PCF8574_ReadPort(&ChipHandler);
  // clear data bits
  data &= 0xF0u;
  portValue &= 0x0Fu;
  portValue |= data;
  // write dat port
  PCF8574_WritePort(&ChipHandler, portValue);
}
