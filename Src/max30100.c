/*
 * max30100.c
 *
 *  Created on: 2022. jún. 16.
 *      Author: drCsabesz
 */

#include "stm32f4xx_hal.h"
#include "main.h"
#include "max30100.h"

#define MAX30100_ADDR 0xAEu

#define REGADDR_IRQSTATUS    0x00u
#define REGADDR_IRQENABLE    0x01u
#define REGADDR_FIFOWRITEPTR 0x02u
#define REGADDR_OVERFLOWCTR  0x03u
#define REGADDR_FIFOREADPTR  0x04u
#define REGADDR_FIFODATA     0x05u
#define REGADDR_MODECONFIG   0x06u
#define REGADDR_SPO2CONFIG   0x07u
#define REGADDR_LEDCONFIG    0x09u
#define REGADDR_TEMPINTEGER  0x16u
#define REGADDR_TEMPFRACTION 0x17u
#define REGADDR_REVISIONID   0xFEu
#define REGADDR_PARTNUMBER   0xFFu

#define REVISION_ID 0x05u
#define PART_ID     0x11u

static uint8_t MAX30100_TxBuffer[16u];
static uint8_t MAX30100_RxBuffer[16u];

static uint8_t MAX30100_Detected = 0u;

extern I2C_HandleTypeDef hi2c1;

void MAX30100_Init( void )
{
  // Read revision and part ID
  MAX30100_TxBuffer[0u] = REGADDR_REVISIONID;
  HAL_I2C_Master_Transmit(&hi2c1, MAX30100_ADDR, MAX30100_TxBuffer, 1u, 100u);
  HAL_I2C_Master_Receive(&hi2c1, MAX30100_ADDR, MAX30100_RxBuffer, 2u, 100u);
  // Check values
  if(   (MAX30100_RxBuffer[0] == REVISION_ID)
     && (MAX30100_RxBuffer[1] == PART_ID) )
  {
    // Chip detected
    MAX30100_Detected = 1u;
  }
}
