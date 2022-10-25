/*
 * lcd_6pin_mcp23s17.c
 *
 *  Created on: 2022. júl. 30.
 *      Author: drCsabesz
 */

#include "lcd_6pin_mcp23s17.h"
#include "mcp23s17.h"

#define PIN_D7 0u
#define PIN_D6 1u
#define PIN_D5 2u
#define PIN_D4 3u
#define PIN_E  4u
#define PIN_RS 5u

static MCP23S17_Handler_t SpiHandler;

/********************************************
* Character LCD driver library
* 4 bit mode, R/W connected to ground
*********************************************/

static void ClearPort(void);
static void WrieDataPins(uint8_t data);

/***********************************************
*
* Toggling the LCD Enable Pin
*
************************************************/
void LcdToggleE(void)
{
  MCP23S17_WritePinA(&SpiHandler, PIN_E, 1);
  DELAY_MS(1);
  MCP23S17_WritePinA(&SpiHandler, PIN_E, 0);
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
    MCP23S17_WritePinA(&SpiHandler, PIN_RS, 1);
  }
  else
  {
    MCP23S17_WritePinA(&SpiHandler, PIN_RS, 0);
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

void LcdPortInit_MCP23S17( SPI_HandleTypeDef* ptrSpi, GPIO_TypeDef* ptrGpioPort, uint16_t gpioPin  )
{
  SpiHandler.ptrHSpi = ptrSpi;
  SpiHandler.portCS  = ptrGpioPort;
  SpiHandler.pinCS   = gpioPin;

  MCP23S17_Init(&SpiHandler);
  MCP23S17_SetIODirectionAB(&SpiHandler, 0u, 0u);
  MCP23S17_WritePortAB(&SpiHandler, 0xC0u, 0xFFu);

  DELAY_MS(100);

  // Write D4-D5 1
  WrieDataPins(0x0Cu);

  LcdToggleE();

  DELAY_MS(6);

  LcdToggleE();

  DELAY_MS(1);

  LcdToggleE();

  DELAY_MS(1);

  WrieDataPins(0x04u);

  LcdToggleE();

  DELAY_MS(6);
}

static void ClearPort(void)
{
  uint8_t portValue = 0u;
  // read data port
  portValue = MCP23S17_ReadPortA(&SpiHandler);
  // clear port bits
  portValue &= 0xC0u;
  // write dat port
  MCP23S17_WritePortA(&SpiHandler, portValue);
}

static void WrieDataPins(uint8_t data)
{
  uint8_t portValue = 0u;
  // read data port
  portValue = MCP23S17_ReadPortA(&SpiHandler);
  // clear data bits
  data &= 0x0Fu;
  portValue &= 0xF0u;
  portValue |= data;
  // write dat port
  MCP23S17_WritePortA(&SpiHandler, portValue);
}

