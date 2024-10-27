#ifndef _MPU6050_H_
#define _MPU6050_H_

#include "sapi.h"  // Incluye sapi para el uso de funciones de I2C

// Dirección I2C del MPU6050 cuando AD0 está a GND (0x68)
#define MPU6050_ADDRESS       0x68

// Definición de registros clave en el MPU6050
#define MPU6050_PWR_MGMT_1    0x6B  // Registro de gestión de energía
#define MPU6050_ACCEL_XOUT_H  0x3B  // Registro de salida de aceleración eje X (alto)

// Función para inicializar el MPU6050
bool_t mpu6050Init(i2cMap_t i2cNumber);

// Función para leer datos de aceleración del MPU6050
bool_t mpu6050ReadAccel(int16_t *ax, int16_t *ay, int16_t *az);

#endif /* _MPU6050_H_ */
