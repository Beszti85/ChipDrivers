/*
 * max7219a.h
 *
 *  Created on: 2022. jún. 5.
 *      Author: drCsabesz
 */

#ifndef INC_MAX7219_H_
#define INC_MAX7219_H_

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

typedef struct
{
  SPI_HandleTypeDef* ptrHSpi;
  GPIO_TypeDef* portCS;
  uint16_t pinCS;
  uint8_t TxBuffer[24u];
  uint8_t Available;
  uint8_t Padding[3u];
} MAX7219_Handler_t;

typedef enum
{
  Shutdown = 0u,
  Normal
} MAX7219_Mode_e;

typedef enum
{
  Digit0 = 1u,
  Digit1,
  Digit2,
  Digit3,
  Digit4,
  Digit5,
  Digit6,
  Digit7,
  EndOfDigits,
} MAX7219_Digit_e;

typedef enum
{
  Intensity_1_32 = 0u,
  Intensity_3_32,
  Intensity_5_32,
  Intensity_7_32,
  Intensity_9_32,
  Intensity_11_32,
  Intensity_13_32,
  Intensity_15_32,
  Intensity_17_32,
  Intensity_19_32,
  Intensity_21_32,
  Intensity_23_32,
  Intensity_25_32,
  Intensity_27_32,
  Intensity_29_32,
  Intensity_31_32,
  Intensity_NumOfValues
} MAX7219_Intensity_e;

typedef enum
{
  TestOff = 0u,
  TestOn
} MAX7219_TestMode_e;

#ifdef __cplusplus
extern "C" {
#endif

void MAX7219_Init( MAX7219_Handler_t* ptrHandler );
void MAX7219_SetMode( MAX7219_Handler_t* ptrHandler, MAX7219_Mode_e mode );
void MAX7219_SetDisplayTestMode( MAX7219_Handler_t* ptrHandler, MAX7219_TestMode_e mode );
void MAX7219_SetIntensity50( MAX7219_Handler_t* ptrHandler );
void MAX7219_SetIntensity( MAX7219_Handler_t* ptrHandler, uint8_t percentage );
void MAX7219_SetScanLimit( MAX7219_Handler_t* ptrHandler, MAX7219_Digit_e digit );
void MAX7219_SetDigit( MAX7219_Handler_t* ptrHandler, MAX7219_Digit_e digit, uint8_t value );
void MAX7219_SetAllDigits( MAX7219_Handler_t* ptrHandler, const uint8_t* ptrBuffer );

#ifdef __cplusplus
}
#endif

#endif /* INC_MAX7219_H_ */
