#include "mpu6050.h"

// Inicializa el MPU6050 configurando su registro de gestión de energía
bool_t mpu6050Init(i2cMap_t i2cNumber) {
    uint8_t data = 0;  // Valor para activar el sensor (pone a cero el bit de sueño)

    // Escribe en el registro de gestión de energía para activar el sensor
    if (!i2cWrite(i2cNumber, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, &data, 1, TRUE)) {
        return FALSE;  // Error en la comunicación I2C
    }

    return TRUE;  // Sensor inicializado correctamente
}

// Lee los valores de aceleración de los ejes X, Y, y Z
bool_t mpu6050ReadAccel(int16_t *ax, int16_t *ay, int16_t *az) {
    uint8_t rawData[6];  // Array para almacenar los datos brutos de aceleración

    // Lee 6 bytes de los registros de aceleración (X, Y, Z)
    if (!i2cRead(I2C0, MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H, rawData, 6, TRUE)) {
        return FALSE;  // Error en la lectura I2C
    }

    // Convierte los datos brutos de 8 bits a enteros de 16 bits
    *ax = (rawData[0] << 8) | rawData[1];  // Aceleración eje X
    *ay = (rawData[2] << 8) | rawData[3];  // Aceleración eje Y
    *az = (rawData[4] << 8) | rawData[5];  // Aceleración eje Z

    return TRUE;  // Lectura completada con éxito
}
