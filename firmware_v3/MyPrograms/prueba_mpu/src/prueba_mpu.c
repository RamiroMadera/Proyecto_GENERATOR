#include "sapi.h"
#include <math.h> // Para sqrt, pow, fabs

MPU60X0_address_t addr = MPU60X0_ADDRESS_0;

int main(void) {
    // Inicializar la placa
    boardConfig();
    printf("Inicializando MPU6050...\r\n");
    
    // Inicializar el sensor MPU6050
    int8_t status = mpu60X0Init(addr);
    if (status < 0) {
        printf("MPU6050 no inicializado, chequee las conexiones\r\n\r\n");
        while (1);
    }
    printf("MPU6050 inicializado correctamente.\r\n\r\n");

    // Variables para las lecturas del acelerómetro
    float AccelX, AccelY, AccelZ;
    float prevMagnitude = 0, currentMagnitude;
    float umbralCambioMagnitud = 1.6; // Nivel de sensibilidad

    // Leer los valores iniciales para calibrar
    mpu60X0Read();
    AccelX = mpu60X0GetAccelX_mss();
    AccelY = mpu60X0GetAccelY_mss();
    AccelZ = mpu60X0GetAccelZ_mss();
    prevMagnitude = sqrt(pow(AccelX, 2) + pow(AccelY, 2) + pow(AccelZ, 2));

    // Bucle principal
    while (TRUE) {
        // Leer el sensor
        mpu60X0Read();

        // Lecturas del acelerómetro
        AccelX = mpu60X0GetAccelX_mss();
        AccelY = mpu60X0GetAccelY_mss();
        AccelZ = mpu60X0GetAccelZ_mss();

        // Calcular la magnitud actual
        currentMagnitude = sqrt(pow(AccelX, 2) + pow(AccelY, 2) + pow(AccelZ, 2));

        // Detectar cambios en la magnitud
        if (fabs(currentMagnitude - prevMagnitude) > umbralCambioMagnitud) {
            gpioWrite(LED2, HIGH); // Movimiento detectado
        } else {
            gpioWrite(LED2, LOW);  // Sin movimiento
        }

        // Actualizar la magnitud previa
        prevMagnitude = currentMagnitude;

        // Imprimir las lecturas
        //printf("Aceleración: (%f, %f, %f) [m/s2], Magnitud: %f\r\n", AccelX, AccelY, AccelZ, currentMagnitude);

        delay(400); // Esperar 
    }
    return 0;
}
