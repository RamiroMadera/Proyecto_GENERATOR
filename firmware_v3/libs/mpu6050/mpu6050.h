#ifndef _MPU6050_H_
#define _MPU6050_H_

#include "sapi.h"  // Incluye sapi para el uso de funciones de I2C

// Direcci�n I2C del MPU6050 cuando AD0 est� a GND (0x68)
#define MPU6050_ADDRESS       0x68

// Definici�n de registros clave en el MPU6050
#define MPU6050_PWR_MGMT_1    0x6B  // Registro de gesti�n de energ�a
#define MPU6050_ACCEL_XOUT_H  0x3B  // Registro de salida de aceleraci�n eje X (alto)

// Funci�n para inicializar el MPU6050
bool_t mpu6050Init(i2cMap_t i2cNumber);

// Funci�n para leer datos de aceleraci�n del MPU6050
bool_t mpu6050ReadAccel(int16_t *ax, int16_t *ay, int16_t *az);

#endif /* _MPU6050_H_ */
