/*=============================================================================
 * Program: prueba_sonido
 * Date: 2024/12/06
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "prueba_sonido.h"
#include "sapi.h"
#include "dados.h"
#include "victoria.h"

void reproducirDados(void);
void reproducirVictoria(void);

int main(void) {
   uint8_t button1 = 0,button2 = 0, dados = 0, victoria = 0;
   /* ------------- INICIALIZACIONES ------------- */
   boardConfig();
   dacConfig(DAC_ENABLE);         // Inicializar DAC
   while(1){
      button1 = gpioRead( TEC1);
      button2 = gpioRead( TEC2);
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

void reproducirDados(void){
   for (int j = 0; j < DATA_DADOS; j++) {
            dacWrite(DAC, audioDados[j]);
            delayInaccurateUs(95);
         }
}

void reproducirVictoria(void){
   for (int j = 0; j < DATA_VICTORIA; j++) {
            dacWrite(DAC, audioVictoria[j]);
            delayInaccurateUs(85);
         }
} 