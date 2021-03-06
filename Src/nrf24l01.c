/*
 * nrf24l01.c
 *
 *  Created on: 2022. júl. 6.
 *      Author: drCsabesz
 */

#include "main.h"
#include "nrf24l01.h"

static void SpiWrite( uint8_t length );
static void SpiRead( uint8_t length );

uint8_t NRF24L01_TxBuffer[32u];
uint8_t NRF24L01_RxBuffer[128u];

extern SPI_HandleTypeDef hspi1;

#define CMD_READ_REGISTER     0x00u
#define CMD_WRITE_REGISTER    0x20u
#define CMD_READ_RX_PAYLOAD   0x61u
#define CMD_WRITE_TX_PAYLOAD  0xA0u
#define CMD_FLUSH_TX          0xE1u
#define CMD_FLUSH_RX          0xE2u
#define CMD_REUSE_TX_PAYLOAD  0xE3u
#define CMD_ACTIVATE          0x50u
#define CMD_READ_RX_PLWIDTH   0x60u
#define CMD_WRITE_ACK_PAYLOAD 0xA8u
#define CMD_WRITE_TX_PL_NOACK 0xB0u
#define CMD_NOP               0xFFu

#define REG_CONFIG            0x00u
#define REG_EN_AA             0x01u
#define REG_EN_RXADDR         0x02u
#define REG_SETUP_AW          0x03u
#define REG_SETUP_RETR        0x04u
#define REG_RF_CH             0x05u
#define REG_RF_SETUP          0x06u
#define REG_STATUS            0x07u
#define REG_OBSERVE_TX        0x08u
#define REG_CD                0x09u
#define REG_RX_ADDR_P0        0x0Au
#define REG_RX_ADDR_P1        0x0Bu
#define REG_RX_ADDR_P2        0x0Cu
#define REG_RX_ADDR_P3        0x0Du
#define REG_RX_ADDR_P4        0x0Eu
#define REG_RX_ADDR_P5        0x0Fu
#define REG_TX_ADDR           0x10u
#define REG_RX_PW_P0          0x11u
#define REG_RX_PW_P1          0x12u
#define REG_RX_PW_P2          0x13u
#define REG_RX_PW_P3          0x14u
#define REG_RX_PW_P4          0x15u
#define REG_RX_PW_P5          0x16u
#define REG_FIFO_STATUS       0x17u
#define REG_DYNPD             0x1Cu
#define REG_FEATURE           0x1Du

void NRF24L01_Init( void )
{
  NRF24L01_TxBuffer[0u] = CMD_READ_REGISTER | REG_CONFIG;
  SpiRead(128u);
}

static void SpiWrite( uint8_t length )
{
  HAL_GPIO_WritePin(CS_NRF24L01_GPIO_Port, CS_NRF24L01_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, NRF24L01_TxBuffer, length, 100u);
  HAL_GPIO_WritePin(CS_NRF24L01_GPIO_Port, CS_NRF24L01_Pin, GPIO_PIN_SET);
}

static void SpiRead( uint8_t length )
{
  HAL_GPIO_WritePin(CS_NRF24L01_GPIO_Port, CS_NRF24L01_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi1, NRF24L01_TxBuffer,NRF24L01_RxBuffer, length, 100u);
  HAL_GPIO_WritePin(CS_NRF24L01_GPIO_Port, CS_NRF24L01_Pin, GPIO_PIN_SET);
}
