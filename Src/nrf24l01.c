/*
 * nrf24l01.c
 *
 *  Created on: 2022. júl. 6.
 *      Author: drCsabesz
 */

#include "main.h"
#include "nrf24l01.h"

static void SpiWrite( NRF24L01_Handler_t* ptrHandler, uint8_t length );
static void SpiRead( NRF24L01_Handler_t* ptrHandler, uint8_t length );

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

typedef struct
{
  uint8_t Addr;     // Command address
  uint8_t Size;     // Command size
} RegParam_t ;

static const RegParam_t CmdParam[NRF24L01_REG_NUM] =
{
  { REG_CONFIG,      2u },
  { REG_EN_AA,       2u },
  { REG_EN_RXADDR,   2u },
  { REG_SETUP_AW,    2u },
  { REG_SETUP_RETR,  2u },
  { REG_RF_CH,       2u },
  { REG_RF_SETUP,    2u },
  { REG_STATUS,      2u },
  { REG_OBSERVE_TX,  2u },
  { REG_CD,          2u },
  { REG_RX_ADDR_P0,  6u },
  { REG_RX_ADDR_P1,  6u },
  { REG_RX_ADDR_P2,  2u },
  { REG_RX_ADDR_P3,  2u },
  { REG_RX_ADDR_P4,  2u },
  { REG_RX_ADDR_P5,  2u },
  { REG_TX_ADDR,     6u },
  { REG_RX_PW_P0,    2u },
  { REG_RX_PW_P1,    2u },
  { REG_RX_PW_P2,    2u },
  { REG_RX_PW_P3,    2u },
  { REG_RX_PW_P4,    2u },
  { REG_RX_PW_P5,    2u },
  { REG_FIFO_STATUS, 2u },
  { REG_DYNPD,       2u },
  { REG_FEATURE,     2u },
};

void NRF24L01_Init( NRF24L01_Handler_t* ptrHandler )
{
  uint8_t mapIndex = 0u;
  for(int i = 0u; i < NRF24L01_REG_NUM; i++ )
  {
    ptrHandler->TxBuffer[0u] = CMD_READ_REGISTER | CmdParam[i].Addr;
    SpiRead(ptrHandler, CmdParam[i].Size);
    ptrHandler->RegisterMap[mapIndex] = i;
    for(int j = 1u; j < CmdParam[i].Size; j++)
    {
      ptrHandler->RegisterMap[mapIndex+j] = ptrHandler->RxBuffer[j];
    }
    mapIndex += CmdParam[i].Size;
  }
  // Write registers
  // Config register: 0 - configure later
  NRF24L01_WriteRegister1Byte(ptrHandler, NRF24L01_REG_CONFIG, 0);
  // Acknoledge Enable - no auto acknoledge
  NRF24L01_WriteRegister1Byte(ptrHandler, NRF24L01_REG_EN_AA, 0);
  // Do not enable any data pipe
  NRF24L01_WriteRegister1Byte(ptrHandler, NRF24L01_REG_EN_RXADDR, 0);
  // 5 bytes for TX/RX address
  NRF24L01_WriteRegister1Byte(ptrHandler, NRF24L01_REG_SETUP_AW, 0x03u);
  // No retransmission
  NRF24L01_WriteRegister1Byte(ptrHandler, NRF24L01_REG_SETUP_RETR, 0);
  // Channel setup during RX or TX
  NRF24L01_WriteRegister1Byte(ptrHandler, NRF24L01_REG_RF_CH, 0);
  // RF setup: 0dB power, data rate = 2Mbps
  NRF24L01_WriteRegister1Byte(ptrHandler, NRF24L01_REG_RF_SETUP, 0);

  // Clear the FIFO's
  NRF24L01_FlushTx(ptrHandler);
  NRF24L01_FlushRx(ptrHandler);

  // Clear any pending interrupt flags
  NRF24L01_ClearIRQFlags(ptrHandler);

  // Enable the chip after config
  NRF24L01_WriteCE(ptrHandler, 1u);
}

uint8_t NRF24L01_ReadRegister1Byte( NRF24L01_Handler_t* ptrHandler, NRF24L01_RegParam_e regId )
{
  uint8_t retval = 0xFFu;
  if( CmdParam[regId].Size == 0x02u )
  {
    ptrHandler->TxBuffer[0u] = CMD_READ_REGISTER | CmdParam[regId].Addr;
    SpiRead(ptrHandler, CmdParam[regId].Size);
    retval = ptrHandler->RxBuffer[1u];
  }
  return retval;
}

void NRF24L01_WriteRegister1Byte( NRF24L01_Handler_t* ptrHandler, NRF24L01_RegParam_e regId, uint8_t value )
{
  if( CmdParam[regId].Size == 0x02u )
  {
    ptrHandler->TxBuffer[0u] = CMD_WRITE_REGISTER | CmdParam[regId].Addr;
    ptrHandler->TxBuffer[1u] = value;
    SpiWrite(ptrHandler, CmdParam[regId].Size);
  }
}

void NRF24L01_ReadRegister( NRF24L01_Handler_t* ptrHandler, NRF24L01_RegParam_e regId )
{
  ptrHandler->TxBuffer[0u] = CMD_READ_REGISTER | CmdParam[regId].Addr;
  SpiRead(ptrHandler, CmdParam[regId].Size);
}

void NRF24L01_WriteRegister( NRF24L01_Handler_t* ptrHandler, NRF24L01_RegParam_e regId )
{
  ptrHandler->TxBuffer[0u] = CMD_WRITE_REGISTER | CmdParam[regId].Addr;
  SpiWrite(ptrHandler, CmdParam[regId].Size);
}

void NRF24L01_ReadRxPayload( NRF24L01_Handler_t* ptrHandler, uint8_t length )
{
  ptrHandler->TxBuffer[0u] = CMD_READ_RX_PAYLOAD;
  SpiRead(ptrHandler, length+1u);
}

void NRF24L01_WriteTxPayload( NRF24L01_Handler_t* ptrHandler, uint8_t length )
{
  ptrHandler->TxBuffer[0u] = CMD_WRITE_TX_PAYLOAD;
  SpiWrite(ptrHandler, length+1u);
}

void NRF24L01_FlushTx( NRF24L01_Handler_t* ptrHandler )
{
  ptrHandler->TxBuffer[0u] = CMD_FLUSH_TX;
  SpiWrite(ptrHandler, 1u);
}

void NRF24L01_FlushRx( NRF24L01_Handler_t* ptrHandler )
{
  ptrHandler->TxBuffer[0u] = CMD_FLUSH_RX;
  SpiWrite(ptrHandler, 1u);
}

void NRF24L01_WriteCE( NRF24L01_Handler_t* ptrHandler, GPIO_PinState value )
{
  HAL_GPIO_WritePin(ptrHandler->portCE, ptrHandler->pinCE, value);
}

void NRF24L01_ClearIRQFlags( NRF24L01_Handler_t* ptrHandler )
{
  uint8_t regStatus;

  regStatus = NRF24L01_ReadRegister1Byte(ptrHandler, NRF24L01_REG_STATUS);
  regStatus |= NRF24L01_MASK_STATUS_IRQ;
  NRF24L01_WriteRegister1Byte(ptrHandler, NRF24L01_REG_STATUS, regStatus);
}

static void SpiWrite( NRF24L01_Handler_t* ptrHandler, uint8_t length )
{
  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_RESET);
#if (HAL_SPI_DMA == 1)
  HAL_SPI_Transmit_DMA(ptrHandler->ptrHSpi, ptrHandler->TxBuffer, length);
#else
  HAL_SPI_Transmit(ptrHandler->ptrHSpi, ptrHandler->TxBuffer, length, 100u);
#endif
  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_SET);
}

static void SpiRead( NRF24L01_Handler_t* ptrHandler, uint8_t length )
{
  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_RESET);
#if (HAL_SPI_DMA == 1)
  HAL_SPI_TransmitReceive_DMA(ptrHandler->ptrHSpi, ptrHandler->TxBuffer, ptrHandler->RxBuffer, length);
#else
  HAL_SPI_TransmitReceive(ptrHandler->ptrHSpi, ptrHandler->TxBuffer, ptrHandler->RxBuffer, length, 100u);
#endif
  HAL_GPIO_WritePin(ptrHandler->portCS, ptrHandler->pinCS, GPIO_PIN_SET);
  ptrHandler->StatusReg = *(ptrHandler->RxBuffer);
}
