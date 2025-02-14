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
  NRF24L01_REG_RF_SETUP,
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
  uint8_t TxBuffer[36u];
  uint8_t RxBuffer[36u];
  uint8_t RegisterMap[64u];
  uint8_t StatusReg;
  uint8_t Available;
  uint8_t Padding[2u];
} NRF24L01_Handler_t;

#ifdef __cplusplus
extern "C" {
#endif

void NRF24L01_Init( NRF24L01_Handler_t* ptrHandler );
uint8_t NRF24L01_ReadRegister1Byte( NRF24L01_Handler_t* ptrHandler, NRF24L01_RegParam_e regId );
void NRF24L01_WriteRegister1Byte( NRF24L01_Handler_t* ptrHandler, NRF24L01_RegParam_e regId, uint8_t value );
void NRF24L01_ReadRegister( NRF24L01_Handler_t* ptrHandler, NRF24L01_RegParam_e regId );
void NRF24L01_WriteRegister( NRF24L01_Handler_t* ptrHandler, NRF24L01_RegParam_e regId );
void NRF24L01_ReadRxPayload( NRF24L01_Handler_t* ptrHandler, uint8_t length );
void NRF24L01_WriteTxPayload( NRF24L01_Handler_t* ptrHandler, uint8_t length );
void NRF24L01_FlushTx( NRF24L01_Handler_t* ptrHandler );
void NRF24L01_FlushRx( NRF24L01_Handler_t* ptrHandler );
void NRF24L01_WriteCE( NRF24L01_Handler_t* ptrHandler, GPIO_PinState value );

#ifdef __cplusplus
}
#endif

#endif /* INC_NRF24L01_H_ */
