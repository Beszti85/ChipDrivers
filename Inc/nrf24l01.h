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

// Register bits definitions
#define NRF24L01_CONFIG_PRIM_RX       (uint8_t)0x01 // PRIM_RX bit in CONFIG register
#define NRF24L01_CONFIG_PWR_UP        (uint8_t)0x02 // PWR_UP bit in CONFIG register
#define NRF24L01_FEATURE_EN_DYN_ACK   (uint8_t)0x01 // EN_DYN_ACK bit in FEATURE register
#define NRF24L01_FEATURE_EN_ACK_PAY   (uint8_t)0x02 // EN_ACK_PAY bit in FEATURE register
#define NRF24L01_FEATURE_EN_DPL       (uint8_t)0x04 // EN_DPL bit in FEATURE register
#define NRF24L01_FLAG_RX_DR           (uint8_t)0x40 // RX_DR bit (data ready RX FIFO interrupt)
#define NRF24L01_FLAG_TX_DS           (uint8_t)0x20 // TX_DS bit (data sent TX FIFO interrupt)
#define NRF24L01_FLAG_MAX_RT          (uint8_t)0x10 // MAX_RT bit (maximum number of TX retransmits interrupt)

// Register masks definitions
#define NRF24L01_MASK_REG_MAP         (uint8_t)0x1F // Mask bits[4:0] for CMD_RREG and CMD_WREG commands
#define NRF24L01_MASK_CRC             (uint8_t)0x0C // Mask for CRC bits [3:2] in CONFIG register
#define NRF24L01_MASK_STATUS_IRQ      (uint8_t)0x70 // Mask for all IRQ bits in STATUS register
#define NRF24L01_MASK_RF_PWR          (uint8_t)0x06 // Mask RF_PWR[2:1] bits in RF_SETUP register
#define NRF24L01_MASK_RX_P_NO         (uint8_t)0x0E // Mask RX_P_NO[3:1] bits in STATUS register
#define NRF24L01_MASK_DATARATE        (uint8_t)0x28 // Mask RD_DR_[5,3] bits in RF_SETUP register
#define NRF24L01_MASK_EN_RX           (uint8_t)0x3F // Mask ERX_P[5:0] bits in EN_RXADDR register
#define NRF24L01_MASK_RX_PW           (uint8_t)0x3F // Mask [5:0] bits in RX_PW_Px register
#define NRF24L01_MASK_RETR_ARD        (uint8_t)0xF0 // Mask for ARD[7:4] bits in SETUP_RETR register
#define NRF24L01_MASK_RETR_ARC        (uint8_t)0x0F // Mask for ARC[3:0] bits in SETUP_RETR register
#define NRF24L01_MASK_RXFIFO          (uint8_t)0x03 // Mask for RX FIFO status bits [1:0] in FIFO_STATUS register
#define NRF24L01_MASK_TXFIFO          (uint8_t)0x30 // Mask for TX FIFO status bits [5:4] in FIFO_STATUS register
#define NRF24L01_MASK_PLOS_CNT        (uint8_t)0xF0 // Mask for PLOS_CNT[7:4] bits in OBSERVE_TX register
#define NRF24L01_MASK_ARC_CNT         (uint8_t)0x0F // Mask for ARC_CNT[3:0] bits in OBSERVE_TX register

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
void NRF24L01_ClearIRQFlags( NRF24L01_Handler_t* ptrHandler );
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
