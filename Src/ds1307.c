#include "ds1307.h"


/* Init to fix value 181h 37  */
/* 2014/01/28 Tuesday */
void DS1307_Init ( DS1307_Handler_t* ptrHandler, DS1307_TimeDate_t* ptrCurrTime )
{
  ptrHandler->TxBuffer[0u] = REG_SECONDS;
  ptrHandler->TxBuffer[1u] = ptrCurrTime->Seconds;
  ptrHandler->TxBuffer[2u] = ptrCurrTime->Minutes;
  ptrHandler->TxBuffer[3u] = ptrCurrTime->Hours;
  ptrHandler->TxBuffer[4u] = ptrCurrTime->Day;
  ptrHandler->TxBuffer[5u] = ptrCurrTime->Date;
  ptrHandler->TxBuffer[6u] = ptrCurrTime->Month;
  ptrHandler->TxBuffer[7u] = ptrCurrTime->Year;

  HAL_I2C_Master_Transmit(ptrHandler->ptrHI2c, ptrHandler->Address, ptrHandler->TxBuffer, 8u, 100u);
}

#if 0
/* Init to fix value 181h 37  */
/* 2014/01/28 Tuesday */
void DS1307_Set_Date ( DS1307_Handler_t* ptrHandler )
{
    I2CStart();
    I2CWrite(DS1307ADDR_W);
    I2CWrite(REG_DAY);
    I2CWrite(0x06);
    I2CWrite(0x12);
    I2CWrite(0x04);
    I2CWrite(0x14);
    I2CWrite(0x00);
    I2CStop();
}
#endif

/* Allow 1 sec SQWE output */
void DS1307_SquareWaveOutput( DS1307_Handler_t* ptrHandler, DS1307_Frequency_e frequency )
{
  ptrHandler->TxBuffer[0u] = REG_CONTROL;
  ptrHandler->TxBuffer[1u] = 0x10u | (uint8_t)frequency;

  HAL_I2C_Master_Transmit(ptrHandler->ptrHI2c, ptrHandler->Address, ptrHandler->TxBuffer, 2u, 100u);
}

void DS1307_Read_Date ( DS1307_Handler_t* ptrHandler, uint8_t *day, uint8_t *date, uint8_t *month, uint8_t *year )
{
	ptrHandler->TxBuffer[0u] = REG_DAY;
	HAL_I2C_Master_Transmit(ptrHandler->ptrHI2c, ptrHandler->Address, ptrHandler->TxBuffer, 1u, 100u);
	HAL_I2C_Master_Receive(ptrHandler->ptrHI2c, ptrHandler->Address, ptrHandler->RxBuffer, 4u, 100u);

	*day = ptrHandler->RxBuffer[0u];
	*date = ptrHandler->RxBuffer[1u];
	*month = ptrHandler->RxBuffer[2u];
	*year = ptrHandler->RxBuffer[3u];
}

void DS1307_Read_Time ( DS1307_Handler_t* ptrHandler, uint8_t *sec, uint8_t *min, uint8_t *hour )
{
  ptrHandler->TxBuffer[0u] = REG_SECONDS;
  HAL_I2C_Master_Transmit(ptrHandler->ptrHI2c, ptrHandler->Address, ptrHandler->TxBuffer, 1u, 100u);
  HAL_I2C_Master_Receive(ptrHandler->ptrHI2c, ptrHandler->Address, ptrHandler->RxBuffer, 3u, 100u);

  *sec = ptrHandler->RxBuffer[0u];
  *min = ptrHandler->RxBuffer[1u];
  *hour = ptrHandler->RxBuffer[2u];
}

void DS1307_Convert_Time (char *s, uint8_t hour, uint8_t min, uint8_t sec)
{
	/* fix size: 9 */ 
	s[0] = ((hour >> 4) & 0x03) + '0';
	s[1] = (hour & 0x0F) + '0';
	s[2] = ':';
	s[3] = (min >> 4) +'0';
	s[4] = (min & 0x0F) + '0';
	s[5] = ':';
	s[6] = (sec >> 4) +'0';
	s[7] = (sec & 0x0F) + '0';
	s[8] = '\0';
}

void DS1307_Convert_Date(char *s, uint8_t year, uint8_t month, uint8_t date)
{
	/* fix size: 9 */
	s[0] = '2';
	s[1] = '0';
	s[2] = ((year >> 4) & 0x0F) + '0';
	s[3] = (year & 0x0F) + '0';
	s[4] = '/';
	s[5] = ((month >> 4) & 0x01)+'0';
	s[6] = (month & 0x0F) + '0';
	s[7] = '/';
	s[8] = ((date >> 4) & 0x03) +'0';
	s[9] = (date & 0x0F) + '0';
	s[10] = '\0';
}

void DS1307_ROM_WriteByte( DS1307_Handler_t* ptrHandler, uint8_t address, uint8_t data )
{
  ptrHandler->TxBuffer[0u] = address;
  ptrHandler->TxBuffer[1u] = data;
  HAL_I2C_Master_Transmit(ptrHandler->ptrHI2c, ptrHandler->Address, ptrHandler->TxBuffer, 2u, 100u);
}

uint8_t DS1307_ROM_ReadByte( DS1307_Handler_t* ptrHandler, uint8_t address )
{
  ptrHandler->TxBuffer[0u] = address;
  HAL_I2C_Master_Transmit(ptrHandler->ptrHI2c, ptrHandler->Address, ptrHandler->TxBuffer, 1u, 100u);
  HAL_I2C_Master_Receive(ptrHandler->ptrHI2c, ptrHandler->Address, ptrHandler->RxBuffer, 1u, 100u);

  return ptrHandler->RxBuffer[0u];
}

void DS1307_Set_Out( DS1307_Handler_t* ptrHandler, uint8_t value )
{
  ptrHandler->TxBuffer[0u] = REG_CONTROL;
  ptrHandler->TxBuffer[1u] = ( value ? 0x80u : 0x00u );

  HAL_I2C_Master_Transmit(ptrHandler->ptrHI2c, ptrHandler->Address, ptrHandler->TxBuffer, 2u, 100u);
}



