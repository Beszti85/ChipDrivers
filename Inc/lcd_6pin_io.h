#ifndef _LCD_6PIN_H_
#define _LCD_6PIN_H_

#include "lcd_6pin_port.h"

/***********************************************/

/***********************************************
*
* Toggling the LCD Enable Pin
*
************************************************/

void LcdToggle(void);
void LcdPortInit_GPIO(void);
void LcdWrite(uint8_t data, uint8_t rs);

#endif
