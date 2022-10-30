/*
 * esp8266_at.h
 *
 *  Created on: Oct 30, 2022
 *      Author: drCsabesz
 */

#ifndef INC_ESP8266_AT_H_
#define INC_ESP8266_AT_H_

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

void ESP8266_Init(UART_HandleTypeDef* ptrUart, uint8_t * ptrBuffer);


#endif /* INC_ESP8266_AT_H_ */
