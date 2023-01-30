/*
 * esp8266_at.c
 *
 *  Created on: Oct 30, 2022
 *      Author: drCsabesz
 */

#include "esp8266_at.h"
#include "string.h"

#define TIMEOUT_MAX 0xFFFFu

char EspAtBuffer[120u];
char ReceivedMessage[120u];

bool EspRespOk = false;
ESP8266_Status_u EspStatusFlags;

char EspAtMessages[30u][20u] =
{
  "ready",
  "busy p",
  "Will force restart!!!",
  "WIFI CONNECTED",
  "WIFI GOT IP",
  "WIFI DISCONNECTED"
};

ESP8266_AtReportTable_t EspAtReportTable[] =
{
  { "ready",     NULL, 0x01u, 0xFBu, TIMEOUT_MAX },
  { "busy p...", NULL, 0x02u, 0xFDu, TIMEOUT_MAX },
};

const uint8_t EspGetAtTableSize = (uint8_t)( sizeof(EspAtReportTable) / sizeof(EspAtReportTable[0] ) );

void ESP8266_Init(UART_HandleTypeDef* ptrUart, uint8_t * ptrBuffer)
{
#if 1
  // Init ESP01
  strncpy(EspAtBuffer, "AT+RST\r\n", 8);
  HAL_UART_Transmit( ptrUart, EspAtBuffer, 8u, 100u);
  HAL_UART_Receive( ptrUart, ptrBuffer, 100u, 5000u);
  HAL_Delay(100u);
#endif
  strncpy(EspAtBuffer, "AT\r\n", 4);
  HAL_UART_Transmit( ptrUart, EspAtBuffer, 4u, 100u);
  HAL_UART_Receive( ptrUart, ptrBuffer, 10u, 100u);
  HAL_Delay(100u);
  strncpy(EspAtBuffer, "ATE0\r\n", 6);
  HAL_UART_Transmit( ptrUart, EspAtBuffer, 6u, 100u);
  HAL_UART_Receive( ptrUart, ptrBuffer, 10u, 100u);
  HAL_Delay(100u);
  strncpy(EspAtBuffer, "AT+GMR\r\n", 8);
  HAL_UART_Transmit( ptrUart, EspAtBuffer, 8u, 100u);
  HAL_UART_Receive( ptrUart, ptrBuffer, 200u, 5000u);
  HAL_Delay(100u);
  strncpy(EspAtBuffer, "AT+CWSTATE?\r\n", 13);
  HAL_UART_Transmit( ptrUart, EspAtBuffer, 13u, 100u);
  HAL_UART_Receive( ptrUart, ptrBuffer, 100u, 2000u);
  HAL_Delay(100u);
  strncpy(EspAtBuffer, "AT+CWMODE=3\r\n", 13);
  HAL_UART_Transmit( ptrUart, EspAtBuffer, 13u, 100u);
  HAL_UART_Receive( ptrUart, ptrBuffer, 50u, 2000u);
  HAL_Delay(100u);
  strncpy(EspAtBuffer, "AT+CWJAP=\" \",\" \"\r\n", 50);
  HAL_UART_Transmit( ptrUart, EspAtBuffer, 50u, 100u);
  HAL_UART_Receive( ptrUart, ptrBuffer, 50u, 2000u);
}

void ESP8266_AtReportHandler( uint8_t* ptrReport )
{
  // Check the command table for the received report
  for( uint8_t index = 0u; index <  EspGetAtTableSize; index++ )
  {
    if( !strcmp( EspAtReportTable[index].ReportMsg, ptrReport ) )
    {
      EspStatusFlags.EspFlagsUnion &= EspAtReportTable[index].statusClear;
      EspStatusFlags.EspFlagsUnion |= EspAtReportTable[index].statusSet;
      break;
    }
  }
}

void ESP8266_SetOkResponseFlag( bool value )
{
  EspRespOk = value;
}

static uint8_t EspGetAtTableSizeFun( void )
{
  return( (uint8_t)( sizeof(EspAtReportTable) / sizeof(EspAtReportTable[0] ) ) );
}

