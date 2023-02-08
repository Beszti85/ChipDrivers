/*
 * mcp23s17_spi.h
 *
 *  Created on: 2022. jún. 20.
 *      Author: drCsabesz
 */

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

#ifndef INC_DS1307_H_
#define INC_DS1307_H_

/* ADDRESS: 0xD0 */

#define DS1307ADDR_R 0xD1
#define DS1307ADDR_W 0xD0

#define REG_SECONDS 0x00
#define REG_MINUTES 0x01
#define REG_HOURS   0x02
#define REG_DAY     0x03
#define REG_DATE    0x04
#define REG_MONTH   0x05
#define REG_YEAR    0x06
#define REG_CONTROL 0x07

#define RAM_START   0x09
#define RAM_END     0x3F

#define RAM_SIZE    56

typedef struct
{
  I2C_HandleTypeDef* ptrHI2c;
  uint8_t Address;
  uint8_t Available;
  uint8_t Padding[2u];
  uint8_t TxBuffer[56u];
  uint8_t RxBuffer[56u];
} DS1307_Handler_t;

typedef struct
{
  uint8_t Seconds;
  uint8_t Minutes;
  uint8_t Hours;
  uint8_t Day;
  uint8_t Date;
  uint8_t Month;
  uint8_t Year;
} DS1307_TimeDate_t;

typedef enum
{
  Freq1Hz = 0,
  Freq4096Hz,
  Freq8192Hz,
  Freq32768Hz
} DS1307_Frequency_e;

#ifdef __cplusplus
extern "C" {
#endif

void DS1307_Init( DS1307_Handler_t* ptrHandler, DS1307_TimeDate_t* ptrCurrTime );
//void DS1307_Set_Date ( DS1307_Handler_t* ptrHandler );
void DS1307_SquareWaveOutput( DS1307_Handler_t* ptrHandler, DS1307_Frequency_e frequency );
void DS1307_Read_Date( DS1307_Handler_t* ptrHandler, uint8_t *day, uint8_t *date, uint8_t *month, uint8_t *year);
void DS1307_Read_Time( DS1307_Handler_t* ptrHandler, uint8_t *sec, uint8_t *min, uint8_t *hour);
void DS1307_Convert_Time(char *s, uint8_t hour, uint8_t min, uint8_t sec);
void DS1307_Convert_Date(char *s, uint8_t year, uint8_t month, uint8_t date);
uint8_t DS1307_ConvertData( char *ptrBuffer, DS1307_TimeDate_t* ptrDateTime );

void DS1307_ROM_WriteByte( DS1307_Handler_t* ptrHandler, uint8_t address, uint8_t data);
uint8_t DS1307_ROM_ReadByte( DS1307_Handler_t* ptrHandler, uint8_t address);
void DS1307_Set_Out( DS1307_Handler_t* ptrHandler, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif /* INC_DS1307_H_ */
