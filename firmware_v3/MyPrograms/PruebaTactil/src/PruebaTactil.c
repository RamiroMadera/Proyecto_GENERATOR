#include "XPT2046_Touchscreen.h"
#include "PruebaTactil.h"
#include "ili9341.h"
#include "sapi.h"


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

    // Imprimir un cuadrado
    coord_2d_t square_top_left = {110, 70}; // (320-100)/2, (240-100)/2
    coord_2d_t square_bottom_right = {210, 170};
    ili9341_set_region(display, square_top_left, square_bottom_right);
    ili9341_fill_region(display, BLACK);

    //ACÁ VA TU CODIGO

   boardConfig();
   printf("Inicializando Táctil...\r\n" );
   XPT2046_Touchscreen pantallita;
   bool_t status;
   status = XPT2046_Touchscreen_begin(&pantallita);

   if( !status ){
      printf( "Touch no inicializado, chequee las conexiones\r\n\r\n" );
      while(1);
   }
   printf("Touch inicializado correctamente.\r\n\r\n" );
   //float AccelX, AccelY, AccelZ, GyroX, GyroY, GyroZ;
   //float umbralGiroscopio = 5000.0; // Umbral para aceleraci?n radial
   //float umbralAceleracion = 10000.0; // Umbral para aceleraci?n lineal
   uint16_t posx=-1,posy=-1,posz=-1;
   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(TRUE){

      //Leer el sensor y guardar en estructura de control
      XPT2046_Touchscreen_readData(&pantallita, &posx, &posy, &posz);
      posx=round((float)(posx-2185)*(320)/(3977-2185));
      posy=round((float)(posy-2185)*(240)/(3977-2185));
      printf( "Coordenada X:  (%d) ",posx);
      printf( "Coordenada Y:  (%d)\r\n",posy);
      //printf( "Presion:  (%d)\r\n",posz);

        // Imprimir un cuadrado
        coord_2d_t square_top_left = {posx, posy}; // (320-100)/2, (240-100)/2
        coord_2d_t square_bottom_right = {posx+30, posy+30};
        ili9341_set_region(display, square_top_left, square_bottom_right);
        ili9341_fill_region(display, BLACK);

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