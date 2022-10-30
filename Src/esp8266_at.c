/*
 * esp8266_at.c
 *
 *  Created on: Oct 30, 2022
 *      Author: drCsabesz
 */

#include "esp8266_at.h"

char EspAtBuffer[120u];

void ESP8266_Init(UART_HandleTypeDef* ptrUart, uint8_t * ptrBuffer)
{
#if 0
  // Init ESP01
  strncpy(EspAtBuffer, "AT+RST\r\n", 8u);
  HAL_UART_Transmit( ptrUart, EspAtBuffer, 8u, 100u);
  HAL_UART_Receive( ptrUart, ptrBuffer, 100u, 5000u);
  HAL_Delay(100u);
#endif
  strncpy(EspAtBuffer, "AT\r\n", 4u);
  HAL_UART_Transmit( ptrUart, EspAtBuffer, 4u, 100u);
  HAL_UART_Receive( ptrUart, ptrBuffer, 10u, 100u);
  HAL_Delay(100u);
  strncpy(EspAtBuffer, "ATE0\r\n", 6u);
  HAL_UART_Transmit( ptrUart, EspAtBuffer, 6u, 100u);
  HAL_UART_Receive( ptrUart, ptrBuffer, 10u, 100u);
  HAL_Delay(100u);
  strncpy(EspAtBuffer, "AT+GMR\r\n", 8u);
  HAL_UART_Transmit( ptrUart, EspAtBuffer, 8u, 100u);
  HAL_UART_Receive( ptrUart, ptrBuffer, 200u, 5000u);
#if 0
  HAL_Delay(100u);
  strncpy(EspAtBuffer, "AT+CWSTATE?\r\n", 13u);
  HAL_UART_Transmit( ptrUart, EspAtBuffer, 13u, 100u);
  HAL_UART_Receive( ptrUart, ptrBuffer, 100u, 2000u);
  HAL_Delay(100u);
#endif
}

void ESP8266_AtReportHandler( char* ptrReport )
{

}
