#include "sapi.h"
#include "sonidos.h"

int main(void) {
   uint8_t button1 = 0, button2 = 0;

   // Inicializar la placa y el parlante
   boardConfig();
   inicializarParlante();

   while(1) {
      button1 = gpioRead(TEC1);
      button2 = gpioRead(TEC2);
      if (!button1) {
         // Reproducir sonido de dados
         reproducirDados();
         button1 = 0;
      }
      if (!button2) {
         // Reproducir sonido victoria
         reproducirVictoria();
         button2 = 0;
      }
   }
   return 0;
}
