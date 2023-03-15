/*
 * max7219a.c
 *
 *  Created on: 2022. jún. 5.
 *      Author: drCsabesz
 */

#include <max7219.h>

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

const uint8_t MAX7219_IntensityLimits[16u] = { 4u, 10u, 16u, 22u, 29u, 35u, 41u, 47u, 54u, 60u, 66u, 72u, 79, 85u, 91u, 97u };

static void SpiWrite( MAX7219_Handler_t* ptrHandler, uint8_t length );

void MAX7219_Init( MAX7219_Handler_t* ptrHandler )
{
  MAX7219_SetMode( ptrHandler, Normal);
  MAX7219_SetMode( ptrHandler, Normal);
  MAX7219_SetDisplayTestMode( ptrHandler, TestOff );
  MAX7219_SetScanLimit( ptrHandler, Digit7 );
  MAX7219_SetIntensity50( ptrHandler );
}

void MAX7219_SetMode( MAX7219_Handler_t* ptrHandler, MAX7219_Mode_e mode )
{
  // transfer buffer
  ptrHandler->TxBuffer[0u] = REGADDR_SHUTDOWN;
  ptrHandler->TxBuffer[1u] = mode;
  SpiWrite( ptrHandler, 2u );
}

void MAX7219_SetDisplayTestMode( MAX7219_Handler_t* ptrHandler, MAX7219_TestMode_e mode )
{
  // transfer buffer
  ptrHandler->TxBuffer[0u] = REGADDR_DPYTEST;
  ptrHandler->TxBuffer[1u] = mode;
  SpiWrite( ptrHandler, 2u );
}

void MAX7219_SetScanLimit( MAX7219_Handler_t* ptrHandler, MAX7219_Digit_e digit )
{
  // transfer buffer
  ptrHandler->TxBuffer[0u] = REGADDR_SCANLIMIT;
  ptrHandler->TxBuffer[1u] = digit - 1u;
  SpiWrite( ptrHandler, 2u );
}

void MAX7219_SetDigit( MAX7219_Handler_t* ptrHandler, MAX7219_Digit_e digit, uint8_t value )
{
  // transfer buffer
  ptrHandler->TxBuffer[0u] = digit;
  ptrHandler->TxBuffer[1u] = value;
  SpiWrite( ptrHandler, 2u );
}

void MAX7219_SetAllDigits( MAX7219_Handler_t* ptrHandler, const uint8_t* ptrBuffer )
{
  for( int i = Digit0; i < EndOfDigits; i++ )
  {
    ptrHandler->TxBuffer[0u] = i;
    ptrHandler->TxBuffer[1u] = *(ptrBuffer + (i - 1));
    SpiWrite( ptrHandler, 2u );
  }
}

void MAX7219_SetIntensity50( MAX7219_Handler_t* ptrHandler )
{
  // transfer buffer
  ptrHandler->TxBuffer[0u] = REGADDR_INTENSITY;
  ptrHandler->TxBuffer[1u] = Intensity_17_32;
  SpiWrite( ptrHandler, 2u );
}

void MAX7219_SetIntensity( MAX7219_Handler_t* ptrHandler, uint8_t percentage )
{
  // transfer buffer
  ptrHandler->TxBuffer[0u] = REGADDR_INTENSITY;
  // 0%: switch off
  if( percentage == 0u )
  {
    MAX7219_SetMode( ptrHandler, Shutdown );
  }
  else
  {
    for( int i = 0u; i < Intensity_NumOfValues; i++ )
    {
      if( MAX7219_IntensityLimits[i] <= percentage )
      {
        ptrHandler->TxBuffer[1u] = i;
        break;
      }
    }
  }

  SpiWrite( ptrHandler, 2u );
}

static void SpiWrite( MAX7219_Handler_t* ptrHandler, uint8_t length )
{
  HAL_GPIO_WritePin( ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_RESET );
#if (HAL_SPI_DMA == 1)
  HAL_SPI_Transmit_DMA( ptrHandler->ptrHSpi, ptrHandler->TxBuffer, length );
#else
  HAL_SPI_Transmit( ptrHandler->ptrHSpi, ptrHandler->TxBuffer, length, 100u );
#endif
  HAL_GPIO_WritePin( ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_SET );
}



