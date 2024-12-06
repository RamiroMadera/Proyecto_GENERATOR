/*=============================================================================
 * Program: prueba_sonido
 * Date: 2024/12/06
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "prueba_sonido.h"
#include "sapi.h"
#include "dados.h"

unsigned short audioData10Bit[DATA_SIZE / 2];
static uint32_t index = 0;

// Función para convertir de 16 bits a 10 bits
void convertTo10Bits(void);
void playAudio(void);


int main(void) {
   bool_t buttonValue = OFF;
   /* ------------- INICIALIZACIONES ------------- */
   boardConfig();
   uartConfig(UART_USB, 115200);   // Inicializar UART
   dacConfig(DAC_ENABLE);         // Inicializar DAC
   convertTo10Bits();
   while(1){
      buttonValue = gpioRead( CIAA_BOARD_BUTTON );
      if (!buttonValue){
         playAudio();
         delayInaccurateUs(63);
      }
   }
   return 0;
}

void convertTo10Bits(void) {
    for (int i = 0; i < DATA_SIZE / 2; i++) {
        // Convertir cada par de bytes de 16 bits a 16 bits sin signo
        unsigned short value16 = (rawData[2 * i] << 8) | rawData[2 * i + 1];
        
        // Mapear de 16 bits a 10 bits
        audioData10Bit[i] = (value16 * 1023) / 65535;  // Proporción lineal
    }
}

void playAudio(void) {
    // Reproduce el valor del arreglo audioData10Bit
    dacWrite(DAC, audioData10Bit[index]);

    // Actualiza el índice de la muestra
    index = (index + 1) % (sizeof(audioData10Bit) / sizeof(audioData10Bit[0]));
}
