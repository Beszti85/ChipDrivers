/*
 * nrf24l01.h
 *
 *  Created on: 2022. júl. 6.
 *      Author: drCsabesz
 */

#ifndef INC_NRF24L01_H_
#define INC_NRF24L01_H_

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

typedef enum
{
  NRF24L01_REG_CONFIG = 0u,
  NRF24L01_REG_EN_AA,
  NRF24L01_REG_EN_RXADDR,
  NRF24L01_REG_SETUP_AW,
  NRF24L01_REG_SETUP_RETR,
  NRF24L01_REG_RF_CH,
  NRF24L01_REG_RFSETUP,
  NRF24L01_REG_STATUS,
  NRF24L01_REG_OBSERVE_TX,
  NRF24L01_REG_CD,
  NRF24L01_REG_RX_ADDR_P0,
  NRF24L01_REG_RX_ADDR_P1,
  NRF24L01_REG_RX_ADDR_P2,
  NRF24L01_REG_RX_ADDR_P3,
  NRF24L01_REG_RX_ADDR_P4,
  NRF24L01_REG_RX_ADDR_P5,
  NRF24L01_REG_TX_ADDR,
  NRF24L01_REG_RX_PW_P0,
  NRF24L01_REG_RX_PW_P1,
  NRF24L01_REG_RX_PW_P2,
  NRF24L01_REG_RX_PW_P3,
  NRF24L01_REG_RX_PW_P4,
  NRF24L01_REG_RX_PW_P5,
  NRF24L01_REG_FIFO_STATUS,
  NRF24L01_REG_DYNPD,
  NRF24L01_REG_FEATURE,
  NRF24L01_REG_NUM
} NRF24L01_RegParam_e;

typedef struct
{
  SPI_HandleTypeDef* ptrHSpi;
  GPIO_TypeDef* portCS;
  uint16_t pinCS;
  GPIO_TypeDef* portCE;
  uint16_t pinCE;
  uint8_t TxBuffer[24u];
  uint8_t RxBuffer[24u];
  uint8_t Available;
  uint8_t Padding[3u];
} NRF24L01_Handler_t;

void NRF24L01_Init( NRF24L01_Handler_t* ptrHandler );
void NRF24L01_ReadRegister( NRF24L01_Handler_t* ptrHandler, NRF24L01_RegParam_e regId );
void NRF24L01_WriteRegister( NRF24L01_Handler_t* ptrHandler, NRF24L01_RegParam_e regId );
void NRF24L01_ReadRxPayload( NRF24L01_Handler_t* ptrHandler, uint8_t length );

#endif /* INC_NRF24L01_H_ */
