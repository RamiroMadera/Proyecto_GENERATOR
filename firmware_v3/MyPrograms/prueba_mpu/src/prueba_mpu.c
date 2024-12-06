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
   float AccelX, AccelY, AccelZ, GyroX, GyroY, GyroZ, AccelX1, AccelY1, AccelZ1, GyroX1, GyroY1, GyroZ1;
   float umbralGiroscopio = 4; // Umbral para aceleración radial
   float umbralAceleracion = 4; // Umbral para aceleración lineal
   mpu60X0Read();
   GyroX1 = mpu60X0GetGyroX_rads();
   GyroY1 = mpu60X0GetGyroY_rads();
   GyroZ1 = mpu60X0GetGyroZ_rads();
   AccelX1 = mpu60X0GetAccelX_mss();
   AccelY1 = mpu60X0GetAccelY_mss();
   AccelZ1 = mpu60X0GetAccelZ_mss();
   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(TRUE){

      //Leer el sensor y guardar en estructura de control
      mpu60X0Read();

      GyroX = mpu60X0GetGyroX_rads() - GyroX1;
      GyroY = mpu60X0GetGyroY_rads() - GyroY1;
      GyroZ = mpu60X0GetGyroZ_rads() - GyroZ1;
      printf( "Giroscopo:  (%f, %f, %f)   [rad/s]\r\n",GyroX, GyroY, GyroZ);
      // Prender led si se esta girando
      if ((GyroX > umbralGiroscopio || GyroX < -umbralGiroscopio) ||
         (GyroY > umbralGiroscopio || GyroY < -umbralGiroscopio) ||
         (GyroZ > umbralGiroscopio || GyroZ < -umbralGiroscopio)) {
         gpioWrite(LED1,HIGH);
      } else {
         gpioWrite(LED1,LOW);
      }
      AccelX = mpu60X0GetAccelX_mss() - AccelX1;
      AccelY = mpu60X0GetAccelY_mss() - AccelY1;
      AccelZ = mpu60X0GetAccelZ_mss() - AccelZ1;
      printf( "Acelerometro:  (%f, %f, %f)   [m/s2]\r\n",AccelX, AccelY, AccelZ);
      // Prender led si se esta agitando
      if ((AccelX > umbralAceleracion || AccelX < -umbralAceleracion) ||
         (AccelY > umbralAceleracion || AccelY < -umbralAceleracion) ||
         (AccelZ > umbralAceleracion || AccelZ < -umbralAceleracion)) {
         gpioWrite(LED2,HIGH);
      } else {
         gpioWrite(LED2,LOW);
      }
		//printf( "Temperatura:   %f [C]\r\n\r\n",  mpu60X0GetTemperature_C() );

      delay(500);
   }
   return 0 ;
}
