#include "XPT2046_Touchscreen.h"
#include "PruebaTactil.h"
#include "ili9341.h"
#include "sapi.h"
#include <math.h>

TS_Point puntoaux;
uint16_t posx=-1,posy=-1,posz=-1;

void my_touch_callback(int param) {
    printf("Touch detected with param: %d\n", param);
    //Leer el sensor y guardar en estructura de control
    XPT2046_Touchscreen_readData(&puntoaux);
    posx=round((float)(puntoaux.x-2185)*(320)/(3977-2185));
    posy=round((float)(puntoaux.y-2185)*(240)/(3977-2185));
    printf( "Coordenada X:  (%d) ",posx);
    printf( "Coordenada Y:  (%d)\r\n",posy);
}

int main(void) {

    boardConfig();
    ili9341_gpio_init();
    ili9341_spi_init();
    tickConfig(1);
    tickCallbackSet(diskTickHook, NULL);

    // Inicializaci n y configuraci n del display ILI9341
    ili9341_desc_ptr_t display;
    const ili9341_cfg_t display_cfg = {
        .cs_pin = gpio_cs_pin,
        .dc_pin = gpio_dc_pin,
        .rst_pin = gpio_rst_pin,
        .spi_tx_dma = spi_tx_dma_b,
        .spi_tx_ready = spi_tx_dma_ready,
        .orientation = ILI9341_ORIENTATION_HORIZONTAL,
        .width = 320,
        .height = 240,
        .timeout_ms = 10000,
        .wup_delay_ms = 20,
        .restart_delay_ms = 20
    };

    const ili9341_hw_cfg_t hw_cfg = ili9341_get_default_hw_cfg();

    display = ili9341_init(&display_cfg, &hw_cfg);
    if (display == NULL) {
        //Error_Handler();

        //ojal� no entre ac� xd
    }

    // C digo de aplicaci n

    //AC� VA TU CODIGO

   //boardConfig();
   printf("Inicializando T�ctil...\r\n" );
   bool_t status;
   status = XPT2046_Touchscreen_begin();

   if( !status ){
      printf( "Touch no inicializado, chequee las conexiones\r\n\r\n" );
      while(1);
   }
   printf("Touch inicializado correctamente.\r\n\r\n" );
   
   touch_set_callback(my_touch_callback);
   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(TRUE){
      //printf("Loop %d\r\n",gpioRead(GPIO1));
      delay(500);
   }

    /*while (1) {
        // C digo de la aplicaci n
    }*/
}

/* 1MS Timer callback */

void diskTickHook(void *ptr)
{
    /* Update Display driver timers. */
    ili9341_1ms_timer_cb();
}


/*
#include "sapi.h"

/* Definimos el pin de la interrupci�n y la configuraci�n del borde de la interrupci�n */
#define GPIO_INTERRUPT_PIN 1    // Pin 1 (GPIO1)
#define FALLING_EDGE          0  // Interrupci�n en el borde de bajada

/* Variable global */
uint32_t interruptCount = 0;

/* Funci�n para manejar la interrupci�n del GPIO */
void GPIO1_IRQHandler(void)
{
   // Si la interrupci�n es por borde descendente
   if ( Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) & PININTCH(1) ) {
      interruptCount++;
      printf("Interrupci�n en GPIO1 detectada, cuenta: %d\n", interruptCount);
   }

   // Limpiar la interrupci�n
   Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));
}

/* Funci�n de inicializaci�n del GPIO y las interrupciones */
static void initGPIOIrqs()
{
   // Inicializar el m�dulo de interrupciones GPIO
   Chip_PININT_Init(LPC_GPIO_PIN_INT);

   // Configurar la interrupci�n para GPIO1
   Chip_SCU_GPIOIntPinSel(1, 0, 1); // Seleccionamos el canal de interrupci�n 1 para el GPIO1
   Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(1)); // Configurar modo de borde
   Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(1)); 

   // Limpiar cualquier interrupci�n pendiente
   NVIC_ClearPendingIRQ(PIN_INT1_IRQn);
   // Habilitar la interrupci�n en el NVIC
   NVIC_EnableIRQ(PIN_INT1_IRQn);
}

/* Funci�n principal */
int main(void)
{
   // Inicializar la placa
   boardConfig();

   // Inicializar la interrupci�n GPIO para el GPIO1
   initGPIOIrqs();

   // Mensaje inicial
   printf("Esperando interrupciones en GPIO1...\n");

   // Bucle principal
   while (1) {
      // El programa puede seguir ejecutando otras tareas mientras espera la interrupci�n
   }

���return�0;
}
*/