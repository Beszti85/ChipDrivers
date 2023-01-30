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

typedef struct
{
  unsigned int Ready             : 1;
  unsigned int Busy              : 1;
  unsigned int Error             : 1;
  unsigned int Wifi_Connected    : 1;
  unsigned int Wifi_Got_Ip       : 1;
  unsigned int Wifi_Got_Ipv6_LL  : 1;
  unsigned int Wifi_Got_Ipv6_GL  : 1;
  unsigned int Wifi_Disconnected : 1;
} ESP8266_StatusBits_t;

typedef union
{
  ESP8266_StatusBits_t StatusBits;
  uint8_t EspFlagsUnion;

} ESP8266_Status_u;

typedef struct
{
  char*     ReportMsg;
  void*     ptrCallback;
  uint8_t   statusSet;
  uint8_t   statusClear;
  uint16_t  timeout;
} ESP8266_AtReportTable_t;

#ifdef __cplusplus
extern "C" {
#endif

void ESP8266_Init(UART_HandleTypeDef* ptrUart, uint8_t * ptrBuffer);
void ESP8266_AtReportHandler( uint8_t* ptrReport );
void ESP8266_SetOkResponseFlag( bool value );

#ifdef __cplusplus
}
#endif

#endif /* INC_ESP8266_AT_H_ */
