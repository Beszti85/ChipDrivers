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

typedef enum
{
  Shutdown = 0u,
  Normal
} MAX7219_Mode_e;

typedef enum
{
  TestOff = 0u,
  TestOn
} MAX7219_TestMode_e;

void MAX7219_SetMode( MAX7219_Mode_e mode );
void MAX7219_SetDisplayTestMode( MAX7219_TestMode_e mode );
void MAX7219_SetIntensity( uint8_t value );

#endif /* INC_MAX7219_H_ */
