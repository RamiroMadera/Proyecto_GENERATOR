/*=============================================================================
 * Author: Tobias <->
 * Date: 2024/10/28
 * Version: 1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "prueba_mpu.h"
#include "sapi.h"
#include "mpu6050.h"

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
   // ---------- CONFIGURACIONES ------------------------------

   // Inicializar y configurar la plataforma
   boardConfig();

   i2cInit(I2C0, 100000);
   if (!mpu6050Init(I2C0)) {
	   printf("Error al inicializar el MPU6050\n");
	   while(1);  // Detener el programa si no se pudo inicializar
   }
   printf("MPU6050 inicializado correctamente\n");

   int16_t ax, ay, az;

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE ) {
	   // Leer los valores de aceleración
	  if (mpu6050ReadAccel(&ax, &ay, &az)) {
		  // Imprimir los valores de aceleración en consola
		  printf("Ax: %d, Ay: %d, Az: %d\n", ax, ay, az);
	  } else {
		  printf("Error al leer el MPU6050\n");
	  }
	  delay(250);
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}