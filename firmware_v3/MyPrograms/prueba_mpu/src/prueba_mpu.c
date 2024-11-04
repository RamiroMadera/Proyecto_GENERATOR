#include "sapi.h"

MPU60X0_address_t addr = MPU60X0_ADDRESS_0;

int main(void){
   boardConfig();
   printf("Inicializando MPU6050...\r\n" );
   int8_t status;
   status = mpu60X0Init( addr );

   if( status < 0 ){
      printf( "MPU6050 no inicializado, chequee las conexiones\r\n\r\n" );
      while(1);
   }
   printf("MPU6050 inicializado correctamente.\r\n\r\n" );
   float AccelX, AccelY, AccelZ, GyroX, GyroY, GyroZ;
   float umbralGiroscopio = 5000.0; // Umbral para aceleración radial
   float umbralAceleracion = 10000.0; // Umbral para aceleración lineal
   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(TRUE){

      //Leer el sensor y guardar en estructura de control
      mpu60X0Read();

      GyroX = mpu60X0GetGyroX_rads();
      GyroY = mpu60X0GetGyroY_rads();
      GyroZ = mpu60X0GetGyroZ_rads();
      printf( "Giroscopo:  (%f, %f, %f)   [rad/s]\r\n",GyroX, GyroY, GyroZ);
      // Prender led si se esta girando
      if ((GyroX > umbralAceleracion || GyroX < -umbralAceleracion) ||
         (GyroY > umbralAceleracion || GyroY < -umbralAceleracion) ||
         (GyroZ > umbralAceleracion || GyroZ < -umbralAceleracion)) {
         gpioWrite(LED1,HIGH);
      } else {
         gpioWrite(LED1,LOW);
      }
      AccelX = mpu60X0GetAccelX_mss();
      AccelY = mpu60X0GetAccelY_mss();
      AccelZ = mpu60X0GetAccelZ_mss();
      printf( "Acelerometro:  (%f, %f, %f)   [m/s2]\r\n",AccelX, AccelY, AccelZ);
      // Prender led si se esta agitando
      if ((AccelX > umbralAceleracion || AccelX < -umbralAceleracion) ||
         (AccelY > umbralAceleracion || AccelY < -umbralAceleracion) ||
         (AccelZ > umbralAceleracion || AccelZ < -umbralAceleracion)) {
         gpioWrite(LED2,HIGH);
      } else {
         gpioWrite(LED2,LOW);
      }
		printf( "Temperatura:   %f [C]\r\n\r\n",
              mpu60X0GetTemperature_C()
            );

      delay(1000);
   }
   return 0 ;
}
