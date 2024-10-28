#include "mpu6050.h"

// Inicializa el MPU6050 configurando su registro de gestión de energía
bool_t mpu6050Init(i2cMap_t i2cNumber) {
   uint8_t data[2];
   data[0] = MPU6050_PWR_MGMT_1; // Dirección del registro de gestión de energía
   data[1] = 0x00;               // Valor a escribir en el registro para activar el sensor

   if (!i2cWrite(i2cNumber, MPU6050_ADDRESS, data, 2, TRUE)) {
      return FALSE;  // Error en la comunicación I2C
   }
    
    return TRUE;  // Sensor inicializado correctamente
}

bool_t mpu6050ReadAccel(int16_t* accelX, int16_t* accelY, int16_t* accelZ) {
    uint8_t rawData[6]; // Buffer para almacenar los datos de aceleración
    bool_t result;

    // Lee los datos de aceleración desde el registro de salida del MPU6050
    result = i2cRead(I2C0, MPU6050_ADDRESS, rawData, 6, TRUE);

    if (result) {
        // Convierte los datos recibidos a valores de aceleración en bruto (16 bits)
        *accelX = (int16_t)((rawData[0] << 8) | rawData[1]);
        *accelY = (int16_t)((rawData[2] << 8) | rawData[3]);
        *accelZ = (int16_t)((rawData[4] << 8) | rawData[5]);
    }

    return result; // Devuelve el estado de la lectura (TRUE si tuvo éxito)
}

