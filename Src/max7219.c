/*
 * max7219a.c
 *
 *  Created on: 2022. jún. 5.
 *      Author: drCsabesz
 */

#include <max7219.h>
#include "stm32f4xx_hal.h"
#include "main.h"

#define REGADDR_NOOP       0x00u
#define REGADDR_DIG0       0x01u
#define REGADDR_DIG1       0x02u
#define REGADDR_DIG2       0x03u
#define REGADDR_DIG3       0x04u
#define REGADDR_DIG4       0x05u
#define REGADDR_DIG5       0x06u
#define REGADDR_DIG6       0x07u
#define REGADDR_DIG7       0x08u
#define REGADDR_DECODEMODE 0x09u
#define REGADDR_INTENSITY  0x0Au
#define REGADDR_SCANLIMIT  0x0Bu
#define REGADDR_SHUTDOWN   0x0Cu
#define REGADDR_DPYTEST    0x0Fu

uint8_t MAX7219_TxBuffer[12u] = {0u};

static void SpiWrite( uint8_t length );

extern SPI_HandleTypeDef hspi1;

void MAX7219_SetMode( MAX7219_Mode_e mode )
{
  // transfer buffer
  MAX7219_TxBuffer[0u] = REGADDR_SHUTDOWN;
  MAX7219_TxBuffer[1u] = mode;
  SpiWrite( 2u );
}

void MAX7219_SetDisplayTestMode( MAX7219_TestMode_e mode )
{
  // transfer buffer
  MAX7219_TxBuffer[0u] = REGADDR_DPYTEST;
  MAX7219_TxBuffer[1u] = mode;
  SpiWrite( 2u );
}

void MAX7219_SetDigit( )
{

}

void MAX7219_SetIntensity( uint8_t percentage )
{
  // transfer buffer
  MAX7219_TxBuffer[0u] = REGADDR_INTENSITY;
  // Calculate:

  MAX7219_TxBuffer[1u] = value;
  SpiWrite( 2u );
}

static void SpiWrite( uint8_t length )
{
  HAL_GPIO_WritePin(CS_OUT2_GPIO_Port, CS_OUT2_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, MAX7219_TxBuffer, length, 100u);
  HAL_GPIO_WritePin(CS_OUT2_GPIO_Port, CS_OUT2_Pin, GPIO_PIN_SET);
}



