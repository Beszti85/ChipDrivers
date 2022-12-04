/*
 * lcd_charapp.c
 *
 *  Created on: 2022. júl. 30.
 *      Author: drCsabesz
 */
#include "lcd_char.h"
#include "lcd_6pin_io.h"
#include "lcd_6pin_mcp23s17.h"
#include "lcd_6pin_pcf8574.h"

/***********************************************
*
* LCD initialisation
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
void LcdInit_MSP23S17(uint8_t dispMode, SPI_HandleTypeDef* ptrSpi, GPIO_TypeDef* ptrGpioPort, uint16_t gpioPin)
{
  LcdPortInit_MCP23S17(ptrSpi, ptrGpioPort, gpioPin);

  LcdWrite(LCD_FUNCTION_DEFAULT, 0);

  DELAY_MS(5);

  LcdWrite(LCD_DISP_OFF, 0);

  LcdClrScr();

  LcdWrite(dispMode, 0);
}

/***********************************************
*
* LCD initialisation
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
void LcdInit_GPIO(uint8_t dispMode)
{
  LcdPortInit_GPIO();

  LcdWrite(LCD_FUNCTION_DEFAULT, 0);

  DELAY_MS(5);

  LcdWrite(LCD_DISP_OFF, 0);

  LcdClrScr();

  LcdWrite(dispMode, 0);
}

/***********************************************
*
* LCD initialisation
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
void LcdInit_PCF8574(uint8_t dispMode, I2C_HandleTypeDef* ptrHI2c, uint8_t chipAddress)
{
  LcdPortInit_PCF8574(ptrHI2c, chipAddress);

  LcdWrite(LCD_FUNCTION_DEFAULT, 0);

  DELAY_MS(5);

  LcdWrite(LCD_DISP_OFF, 0);

  LcdClrScr();

  LcdWrite(dispMode, 0);
}

/*************************************************************************
Display string without auto linefeed
Input:    string to be displayed
Returns:  none
*************************************************************************/

#if (LCD_LINES == 1)
void LcdPuts(const char *s, uint8_t pos)
#else
void LcdPuts(const char *s, uint8_t line, uint8_t pos)
#endif
/* print string on lcd (no auto linefeed) */
{
    char c;

  /* Set starting point */
  if (pos >= LCD_DISP_LENGTH)
  {
    /* Error handling*/
    pos = LCD_DISP_LENGTH - 1;
  }

  #if (LCD_LINES == 1)
  LcdWrite((1<<LCD_DDRAM) + LCD_START_LINE1 + pos,0);
  #elif ((LCD_LINES == 2) || (LCD_LINES == 4))
  if (line == 1)
  {
    LcdWrite((1<<LCD_DDRAM) + LCD_START_LINE1 + pos,0);
  }
  else
  if (line == 2)
  {
    LcdWrite((1<<LCD_DDRAM) + LCD_START_LINE2 + pos,0);
  }
  #if (LCD_LINES == 4)
  else
  if (line == 3)
  {
    LcdWrite((1<<LCD_DDRAM) + LCD_START_LINE3 + pos,0);
  }
  else
  if (line == 4)
  {
    LcdWrite((1<<LCD_DDRAM) + LCD_START_LINE2 + pos,0);
  }
  #endif
  else
  {
    /* Valami error kezelés */
  }
  #else
  #error: "Value of LCD_LINES is wrong"
  #endif

  /* Write out the chars */
    while ((c = *s))
  {
        LcdWrite(c, 1);
    s++;
    }
}



/*************************************************************************
Display string without auto linefeed
Input:    string to be displayed
Returns:  none
*************************************************************************/

void LcdPuts_basic(char *s)

/* print string on lcd (no auto linefeed) */
{
    char c;

  /* Write out the chars */
    while ((c = *s))
  {
        LcdWrite(c, 1);
    s++;
    }
}

/***********************************************
*
* LCD clear screen
*
***********************************************/

void LcdClrScr(void)
{
    LcdWrite(0x01, 0);
}

