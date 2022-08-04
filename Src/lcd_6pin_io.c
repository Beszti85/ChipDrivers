#include <lcd_6pin_io.h>

/********************************************
* Character LCD driver library
* 4 bit mode, R/W connected to ground
*********************************************/

static void ClearPort(void);
static void ClearData(void);

/***********************************************
*
* Toggling the LCD Enable Pin
*
************************************************/

void LcdToggleE(void)
{
	LCD_E_1();
	DELAY_MS(1);
	LCD_E_0();
	DELAY_MS(1);
}


void LcdWrite(uint8_t data, uint8_t rs)
{
	if (rs == 1)
	{
		LCD_RS_1();
	}
	else
	{
		LCD_RS_0();
	}
	/* write high nibble */
	LCD_DATA_0();
	if (data & 0x80)
	{
		LCD_D7_1();
	}
	if (data & 0x40)
	{
		LCD_D6_1();
	}
	if (data & 0x20)
	{
		LCD_D5_1();
	}
	if (data & 0x10)
	{
		LCD_D4_1();
	}
	LcdToggleE();

	/* write low nibble */
	LCD_DATA_0();
	if (data & 0x08)
	{
		LCD_D7_1();
	}
	if (data & 0x04)
	{
		LCD_D6_1();
	}
	if (data & 0x02)
	{
		LCD_D5_1();
	}
	if (data & 0x01)
	{
		LCD_D4_1();
	}
	LcdToggleE();
	
	/* High impedance -> inactive state ? */
	//LCD_DATA_H()
	
	DELAY_MS(1);
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

void LcdInit(uint8_t dispMode)
{
	LCD_PORT_0();
	DELAY_MS(100);
	
	LCD_D5_1();
	LCD_D4_1();
	
	LcdToggleE();
	
	DELAY_MS(5);
	
	LcdToggleE();
	
	DELAY_MS(1);
	
	LcdToggleE();
	
	DELAY_MS(1);
	
	LCD_D4_0();
	
	LcdToggleE();
	
	DELAY_MS(5);
	
	LcdWrite(LCD_FUNCTION_DEFAULT, 0);
	
	DELAY_MS(5);
	
	LcdWrite(LCD_DISP_OFF, 0);
	
	LcdClrScr();
	
	LcdWrite(dispMode, 0);
}

static void ClearPort(void)
{
	LCD_RS_0();
	LCD_E_0();
	LCD_D7_0();
	LCD_D6_0();
	LCD_D5_0();
	LCD_D4_0();
}

static void ClearData(void)
{
	LCD_D7_0();
	LCD_D6_0();
	LCD_D5_0();
	LCD_D4_0();
}
