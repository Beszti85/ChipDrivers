/*
 * leddrv_pca9685pw.h
 *
 *  Created on: 2022. jún. 2.
 *      Author: drCsabesz
 */

#ifndef INC_LEDDRV_PCA9685PW_H_
#define INC_LEDDRV_PCA9685PW_H_

extern I2C_HandleTypeDef hi2c1;

void LEDDRV_PCA9685_ReadModeRegs( void );

#endif /* INC_LEDDRV_PCA9685PW_H_ */
