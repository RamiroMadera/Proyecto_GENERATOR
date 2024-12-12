#include "XPT2046_Touchscreen.h"
#include "PruebaTactil.h"
#include "ili9341.h"
#include "sapi.h"
#include <math.h>


/* Definimos el pin de la interrupción y la configuración del borde de la interrupción */
#define GPIO_INTERRUPT_PIN 1    // Pin 1 (GPIO1)
#define FALLING_EDGE          0  // Interrupción en el borde de bajada

/* Variable global */
uint32_t interruptCount = 0;

/* Función para manejar la interrupción del GPIO */
void GPIO1_IRQHandler(void)
{
   // Si la interrupción es por borde descendente
   if ( Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) & PININTCH(1) ) {
      interruptCount++;
      printf("Interrupción en GPIO1 detectada, cuenta: %d\r\n", interruptCount);
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
}

/* Función principal */
int main(void) {
   // Inicializar la placa
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

        //ojalá no entre acá xd
    }
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

   // Bucle principal
   while (1) {
      // El programa puede seguir ejecutando otras tareas mientras espera la interrupción
   }
   
   return 0;
}

void diskTickHook(void *ptr)
{
    /* Update Display driver timers. */
    ili9341_1ms_timer_cb();
}