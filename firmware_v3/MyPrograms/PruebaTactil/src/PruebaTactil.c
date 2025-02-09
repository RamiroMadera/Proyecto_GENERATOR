#include "XPT2046_Touchscreen.h"
#include "PruebaTactil.h"
#include "ili9341.h"
#include "sapi.h"
#include <math.h>

volatile TS_Point puntoaux;
volatile uint16_t posx=-1,posy=-1,posz=-1;

/*
// PENIRQ interrupt handler
void GPIO1_IRQHandler(void) {
    // Check if PENIRQ is LOW (screen touched)
    if (Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) & PININTCH(1)) {
          //Leer el sensor y guardar en estructura de control
          XPT2046_Touchscreen_readData(&puntoaux);
          posx=round((float)(puntoaux.x-2185)*(320)/(3977-2185));
          posy=round((float)(puntoaux.y-2185)*(240)/(3977-2185));
          printf( "Coordenada X:  (%d) ",posx);
          printf( "Coordenada Y:  (%d)\r\n",posy);
          printf("Detecta flag\r\n");
    }
    printf("Entra rutina\r\n");
    // Clear interrupt flag
    
     if (Chip_GPIO_GetPinState(LPC_GPIO_PORT, 3, 3) == 0) {  // Check if the pin is still low
        // The interrupt condition is still active; wait or handle accordingly
        while (Chip_GPIO_GetPinState(LPC_GPIO_PORT, 3, 3) == 0) {
            // Optional: Add a timeout here to avoid an infinite loop
           delay(10);
        }
    }
    Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));
    printf("Sale flag clear \r\n");
}
*/

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

        //ojalá no entre acá xd
    }

    // C digo de aplicaci n

    //ACÁ VA TU CODIGO

   //boardConfig();
   printf("Inicializando Táctil...\r\n" );
   bool_t status;
   status = XPT2046_Touchscreen_begin();

   if( !status ){
      printf( "Touch no inicializado, chequee las conexiones\r\n\r\n" );
      while(1);
   }
   printf("Touch inicializado correctamente.\r\n\r\n" );
   
   //touch_set_callback(my_touch_callback);
   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(TRUE){
      //printf("Loop %d\r\n",gpioRead(GPIO1));
      
          XPT2046_Touchscreen_readData(&puntoaux);
          posx=puntoaux.x;
          posy=puntoaux.y;
          posz=puntoaux.z;
          //posx=round((float)(puntoaux.x-2186)*(15)/(3978-2186));
          //posy=round((float)(puntoaux.y-2186)*(14)/(3850-2186));
          printf( "Coordenada X:  (%d) ",posx);
          printf( "Coordenada Y:  (%d)\r\n",posy);
          //printf( "Presion:  (%d)\r\n",posz);
          //printf("Pussi\n");
          //printf("Detecta flag\r\n");
      
      delay(1000);
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


