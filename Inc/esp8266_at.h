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
#include <string.h>
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

void ESP8266_Init(UART_HandleTypeDef* ptrUart, uint8_t * ptrBuffer);
void ESP8266_AtReportHandler( uint8_t* ptrReport );

#ifdef __cplusplus
}
#endif

#endif /* INC_ESP8266_AT_H_ */
