#include "mpu6050.h"
#include "sapi.h"

int main(void) {
// Inicializar la placa y el sensor
mpu6050Init();
printf("MPU6050 inicializado correctamente.\r\n\r\n");

// Bucle principal
while (TRUE) {
   // Verificar si hay movimiento
      if (agitando()) {
         gpioWrite(LED2, HIGH); // Movimiento detectado
      } else {
         gpioWrite(LED2, LOW);  // Sin movimiento
      }                           
      delay(100); // Esperar
   }                  
   return 0;
}
