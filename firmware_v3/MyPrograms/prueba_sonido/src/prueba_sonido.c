/*=============================================================================
 * Program: prueba_sonido
 * Date: 2024/12/06
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "prueba_sonido.h"
#include "sapi.h"
#include "dados.h"

unsigned short audioData10Bit[DATA_SIZE / 2];
static uint32_t i = 0;

void playAudio(void);

int main(void) {
   bool_t buttonValue = OFF;
   bool_t ok = OFF;
   /* ------------- INICIALIZACIONES ------------- */
   boardConfig();
   uartConfig(UART_USB, 115200);   // Inicializar UART
   dacConfig(DAC_ENABLE);         // Inicializar DAC
   while(1){
      buttonValue = gpioRead( CIAA_BOARD_BUTTON );
      if (!buttonValue) {
         ok=TRUE;
         delay(200);
      }
      if (ok){
         for (int j = 0; j < DATA_SIZE / 2; j++) {
            dacWrite(DAC, rawData[j]);
            delayInaccurateUs(125);
         }
         ok=OFF;
      }
   }
   return 0;
}

void playAudio(void) {
   if (i==0) printf("Comenzando \r\n");
   if (i==(DATA_SIZE)) printf("Fin \r\n\r\n");
   dacWrite(DAC, rawData[i]);
   i = (i + 1) % DATA_SIZE;
}
