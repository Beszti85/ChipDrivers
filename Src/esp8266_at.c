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
bool EspAtCmdRespOk = false;
ESP8266_Status_u EspStatusFlags;
ESP8266_CMD_ID ESP8266_LastAtCmd = ESP8266_CMD_ID_NONE;

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
  { "ready",             NULL, 0x01u, 0xFBu, TIMEOUT_MAX },
  { "busy p...",         NULL, 0x02u, 0xFDu, TIMEOUT_MAX },
  { "WIFI CONNECTED",    NULL, 0x08u, 0xF7u, TIMEOUT_MAX },
  { "WIFI GOT IP",       NULL, 0x10u, 0xDFu, TIMEOUT_MAX },
  { "WIFI DISCONNECTED", NULL, 0x80u, 0x7Fu, TIMEOUT_MAX },
};

ESP8266_AtCmdTable_t EspAtCmdTable[] =
{
  { ESP8266_CMD_ID_NONE,            " ",                      NULL,                "OK",            NULL,          "ERROR",  NULL,   1000u,   NULL },
  { ESP8266_CMD_ID_RST,             "AT+RST\r\n",             NULL,                "OK",            NULL,          "ERROR",  NULL,   1000u,   NULL },
  { ESP8266_CMD_ID_AT_TEST,         "AT\r\n",                 NULL,                "OK",            NULL,          "ERROR",  NULL,   1000u,   NULL },
  { ESP8266_CMD_ID_VERSION,         "AT+GMR\r\n",             NULL,                "OK",            NULL,          "ERROR",  NULL,   1000u,   NULL },
  { ESP8266_CMD_ID_AT_CMD_LIST,     "AT+CMD?\r\n",            NULL,                "OK",            NULL,          "ERROR",  NULL,   1000u,   NULL },
  { ESP8266_CMD_ID_DEEP_SLEEP,      "AT+GSLP=\r\n",           NULL,                "OK",            NULL,          "ERROR",  NULL,   1000u,   NULL },
  { ESP8266_CMD_ID_AT_ECHO_ON,      "ATE1\r\n",               NULL,                "OK",            NULL,          "ERROR",  NULL,   1000u,   NULL },
  { ESP8266_CMD_ID_AT_ECHO_OFF,     "ATE0\r\n",               NULL,                "OK",            NULL,          "ERROR",  NULL,   1000u,   NULL },
  { ESP8266_CMD_ID_RESTORE_DEF,     "AT+RESTORE\r\n",         NULL,                "OK",            NULL,          "ERROR",  NULL,   1000u,   NULL },
  { ESP8266_CMD_ID_GET_UART_CFG_TEMP,   "AT+UART_CUR?\r\n",       NULL,                "OK",            NULL,          "ERROR",  NULL,   1000u,   NULL },
  { ESP8266_CMD_ID_GET_UART_CFG_PERM,   "AT+UART_DEF?\r\n",       NULL,                "OK",            NULL,          "ERROR",  NULL,   1000u,   NULL },
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

void ESP8266_ProcessAtCmd( UART_HandleTypeDef* ptrUart, ESP8266_CMD_ID cmdId )
{
  // Issue DMA transfer
  ESP8266_LastAtCmd = cmdId;
  ESP8266_AtCmdTable_t* ptrCmd = &EspAtCmdTable[cmdId];
  HAL_UART_Transmit_DMA( ptrUart, ptrCmd->AtCmd, strlen(EspAtCmdTable[cmdId].AtCmd) );
}

void ESP8266_AtReportHandler( uint8_t* ptrReport )
{
  // Check the command table for the received report
  for( uint8_t index = 0u; index <  EspGetAtTableSize; index++ )
  {
    if( !strcmp( EspAtReportTable[index].ReportMsg, ptrReport ) )
    {

      EspStatusFlags.EspFlagsUnion |= EspAtReportTable[index].statusSet;
      break;
    }
  }
}

void ESP8266_AtResponseHandler( uint8_t* ptrReport )
{
  // Compare the response with the expected one
  if( !strncmp(EspAtCmdTable[ESP8266_LastAtCmd].AtPrimResp, ptrReport, sizeof(EspAtCmdTable[ESP8266_LastAtCmd].AtPrimResp) ) )
  {
    // Command response ok
    EspAtCmdRespOk = true;
  }
  else
  {
    // Set error
    EspAtCmdRespOk = false;
  }
}

ESP8266_CMD_ID ESP8266_GetLastAtCmd( void )
{
  return ESP8266_LastAtCmd;
}

void ESP8266_SetOkResponseFlag( bool value )
{
  EspRespOk = value;
}

static uint8_t EspGetAtTableSizeFun( void )
{
  return( (uint8_t)( sizeof(EspAtReportTable) / sizeof(EspAtReportTable[0] ) ) );
}

