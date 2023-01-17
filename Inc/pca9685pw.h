/*
 * pca9685pw.h
 *
 *  Created on: 2022. jún. 2.
 *      Author: drCsabesz
 */

#ifndef INC_PCA9685PW_H_
#define INC_PCA9685PW_H_

typedef struct
{
  I2C_HandleTypeDef* ptrHI2c;
  GPIO_TypeDef* portOE;
  uint16_t pinOE;
  uint8_t Address;
  uint8_t Available;
  uint8_t TxBuffer[24u];
  uint8_t RxBuffer[24u];
} PCA9685_Handler_t;

#ifdef __cplusplus
extern "C" {
#endif

void PCA9685_ReadModeRegs( PCA9685_Handler_t* ptrHandler );

#ifdef __cplusplus
}
#endif

#endif /* INC_PCA9685PW_H_ */
