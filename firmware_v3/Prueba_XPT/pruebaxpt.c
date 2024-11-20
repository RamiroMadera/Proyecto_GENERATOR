#include "sapi.h"
#include "XPT2046_Touchscreen.h"

int main(void){
   boardConfig();
   printf("Inicializando Táctil...\r\n" );
   XPT2046_Touchscreen pantallita;
   bool_t status;
   status = XPT2046_Touchscreen_begin(&pantallita);

   if( status ){
      printf( "Touch no inicializado, chequee las conexiones\r\n\r\n" );
      while(1);
   }
   printf("Touch inicializado correctamente.\r\n\r\n" );
   //float AccelX, AccelY, AccelZ, GyroX, GyroY, GyroZ;
   //float umbralGiroscopio = 5000.0; // Umbral para aceleraci�n radial
   //float umbralAceleracion = 10000.0; // Umbral para aceleraci�n lineal
   uint16_t posx=-1,posy=-1,posz=-1;
   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(TRUE){

      //Leer el sensor y guardar en estructura de control
      XPT2046_Touchscreen_readData(pantallita, posx, posy, posz);

      printf( "Coordenada X:  (%d)\r\n",posx);
      printf( "Coordenada Y:  (%d)\r\n",posy);
      printf( "Presion:  (%d)\r\n",posz);

      delay(1000);
   }
   return 0 ;
}