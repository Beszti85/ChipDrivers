/*
 * pca9685pw.h
 *
 *  Created on: 2022. jún. 2.
 *      Author: drCsabesz
 */

#ifndef INC_PCA9685PW_H_
#define INC_PCA9685PW_H_

extern I2C_HandleTypeDef hi2c1;

void PCA9685_ReadModeRegs( void );

#endif /* INC_PCA9685PW_H_ */
