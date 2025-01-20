#include "XPT2046_Touchscreen.h"
#include "PruebaTactil.h"
#include "ili9341.h"
#include "sapi.h"
#include <math.h>

/* Variable global */
uint32_t interruptCount = 0;
uint32_t flag=0;

/* Función para manejar la interrupción del GPIO */
void GPIO1_IRQHandler(void)
{
   // Si la interrupción es por borde descendente
   if ( Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) & PININTCH(1) ) {
      interruptCount++;
      printf("Interrupción en GPIO1 detectada, cuenta: %d\r\n", interruptCount);
      flag=1;
   }
   // Limpiar la interrupción
   Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));
}

/* Función de inicialización del GPIO y las interrupciones */
static void initGPIOIrqs()
{
   // Inicializar el módulo de interrupciones GPIO
   Chip_PININT_Init(LPC_GPIO_PIN_INT);
   // Configurar la interrupción para GPIO1
   Chip_SCU_GPIOIntPinSel(1, 3, 3); // Seleccionamos el canal de interrupción 1 para el GPIO1
   Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(1)); // Configurar modo de borde
   Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(1)); 

   // Limpiar cualquier interrupción pendiente
   NVIC_ClearPendingIRQ(PIN_INT1_IRQn);
   // Habilitar la interrupción en el NVIC
   NVIC_EnableIRQ(PIN_INT1_IRQn);
   //NVIC_DisableIRQ(PIN_INT1_IRQn);
}

/* Función principal */
int main(void) {
   // Inicializar la placa
   boardConfig();
   spiInit(SPI0);
   printf("Inicializando Táctil...\r\n" );
   bool_t status;
   status = XPT2046_Touchscreen_begin();

   if( !status ){
      printf( "Touch no inicializado, chequee las conexiones\r\n\r\n" );
      while(1);
   }
   printf("Touch inicializado correctamente.\r\n\r\n" );
   TS_Point puntoaux;
   uint16_t posx=-1,posy=-1,posz=-1;
   // Inicializar la interrupción GPIO para el GPIO1
   initGPIOIrqs();
   // Mensaje inicial
   printf("Esperando interrupciones en GPIO1...\r\n");
   int ant=-1,act=-1;
   // Bucle principal
   while (1) {
      act=gpioRead(GPIO3);
      if (ant != act) printf("GPIO3: (%d)\r\n", act);
      ant=act;
      if (flag) {
         NVIC_DisableIRQ(PIN_INT1_IRQn);
         XPT2046_Touchscreen_readData(&puntoaux);
         posx = puntoaux.x;
         posy = puntoaux.y;
         posz = puntoaux.z;
         printf("X: (%d) ", posx);
         printf("Y: (%d) ", posy);
         printf("Presion: (%d)\r\n", posz);
         flag = 0;
         NVIC_ClearPendingIRQ(PIN_INT1_IRQn);
         NVIC_EnableIRQ(PIN_INT1_IRQn);
      }
   }
   
   return 0;
}

void diskTickHook(void *ptr)
{
    /* Update Display driver timers. */
    ili9341_1ms_timer_cb();
}