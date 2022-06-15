/*
 * pcf8574.h
 *
 *  Created on: 2022. jún. 15.
 *      Author: drCsabesz
 */

#ifndef INC_PCF8574_H_
#define INC_PCF8574_H_

#include <stdint.h>
#include <stdbool.h>

uint8_t PCF8574_ReadPort( void );
void PCF8574_WritePort( uint8_t value );
void PCF8574_WritePin( uint8_t pin, uint8_t value );
bool PCF8574_ReadPin( uint8_t pin );

#endif /* INC_PCF8574_H_ */
