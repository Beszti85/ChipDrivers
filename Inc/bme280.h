/*
 * bme280.h
 *
 *  Created on: Jun 3, 2022
 *      Author: drCsabesz
 */

#ifndef INC_BME280_H_
#define INC_BME280_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
  uint32_t RawTemp;
  uint32_t RawHum;
  uint32_t RawPres;
} BME280_RawMeasVal_t;

typedef struct
{
  double Temperature;
  double Humidity;
  double Pressure;
} BME280_PhysValues_t;

typedef enum
{
  MeasOff = 0u,
  Oversampling1,
  Oversampling2,
  Oversampling4,
  Oversampling8,
  Oversampling16
} BME280_Oversampling_e;

void BME280_Detect( void );
void BME280_ReadMeasResult( void );
void BME280_StartMeasurement( BME280_Oversampling_e ovsTemp, BME280_Oversampling_e ovsHum, BME280_Oversampling_e ovsPres );

#endif /* INC_BME280_H_ */
