#include "../inc/app.h"

#include "ili9341.h"
#include "sapi.h"


int main(void) {

    boardConfig();
    ili9341_gpio_init();
    ili9341_spi_init(100000000);
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
        .orientation = ILI9341_ORIENTATION_HORIZONTAL_UD,
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
    }

    // C digo de aplicaci n

    ili9341_paintBackground(display, RED);
    // Imprimir un cuadrado
    coord_2d_t square_top_left = {100, 100}; // (320-100)/2, (240-100)/2
    coord_2d_t square_bottom_right = {319, 239};
    ili9341_set_region(display, square_top_left, square_bottom_right);
    ili9341_fill_region(display, BLACK);

    //imprimir dados
    ili9341_setDadoFondo(MAGENTA);

    for (int i = 1; i < 3; i++)
    {
        ili9341_drawDadoBase(display, i);
    }
    for (int i = 1; i < 3; i++)
    {
       ili9341_drawDadoNumero(display, i, i);
    }


    ili9341_spi_init(400000);
    ili9341_setDadoFondo(MAROON);
    for (int i = 3; i < 6; i++)
    {
        ili9341_drawDadoBase(display, i);
    }
    for (int i = 3; i < 6; i++)
    {
        ili9341_drawDadoNumero(display, i, i);
    }
    //Imprimir carácteres
    uint16_t x = 55;
    uint16_t y = 190;
    uint8_t size = 2;
    uint16_t color = GREEN;
    uint16_t bg = BLACK;
   /*
    ili9341_setCursor(x, y);
    ili9341_setTextSize(size);
    ili9341_setTextColor(color, bg);
    //ili9341_printStr(display, "Imprimiendo:");
    ili9341_printStr(display, numeros);
    */

   
}

/* 1MS Timer callback */

void diskTickHook(void *ptr)
{
    /* Update Display driver timers. */
    ili9341_1ms_timer_cb();
}