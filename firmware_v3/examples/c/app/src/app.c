#include "../inc/app.h"

#include "ili9341.h"
#include "sapi.h"


int main(void) {

    boardConfig();
    ili9341_gpio_init();
    ili9341_spi_init();
    tickConfig(1);
    tickCallbackSet(diskTickHook, NULL);

    // Inicializaci�n y configuraci�n del display ILI9341
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

    // C�digo de aplicaci�n

    // Imprimir un cuadrado
    coord_2d_t square_top_left = {110, 70}; // (320-100)/2, (240-100)/2
    coord_2d_t square_bottom_right = {210, 170};
    ili9341_set_region(display, square_top_left, square_bottom_right);
    ili9341_fill_region(display, BLACK);

    //Imprimir algunas líneas
    uint16_t start_X = 110;
    uint16_t start_Y = 70;
    ili9341_drawLine(display, start_X, start_Y, start_X + 40, start_Y, RED);
    ili9341_drawLine(display, start_X, start_Y, start_X + 37, start_Y + 16, BLUE);
    ili9341_drawLine(display, start_X, start_Y, start_X + 29, start_Y + 29, YELLOW);
    ili9341_drawLine(display, start_X, start_Y, start_X + 16, start_Y + 37, GREEN);
    ili9341_drawLine(display, start_X, start_Y, start_X + 0 , start_Y + 40, BLUE);


    //Imprimir píxeles sobre región cuadrada
    uint16_t startX = 110;    // X-coordinate of the top-left corner of the square
    uint16_t startY = 70;     // Y-coordinate of the top-left corner of the square
    uint16_t width = 100;     // Width of the square
    uint16_t height = 100;    // Height of the square
    int numPixels = 500;      // Number of random pixels to draw
    // Loop to draw random pixels
    for (int i = 0; i < numPixels; i++)
    {
        // Generate random (x, y) within the square
        uint16_t x = startX + (rand() % width);
        uint16_t y = startY + (rand() % height);

        // Generate a random color (RGB565 format)
        uint16_t color = (rand() % 0xFFFF); // RGB565 uses 16 bits for color

        // Draw the pixel (assuming ili9341_drawPixel is already defined)
        ili9341_drawPixel(display, x, y, color);
    }

    //Imprimir carácteres
    uint16_t x = 110;
    uint16_t y = 190;
    uint8_t size = 1;
    uint16_t color = BLACK;
    uint16_t bg = PINK;

    ili9341_setCursor(display, x, y);
    ili9341_setTextSize(display, size);
    ili9341_setTextColor(display, color, bg);
    print_message(desc, "Hola fucking mundo!!");
    ili9341_print(display, ":");
    ili9341_print(display, ")");

    while (1) {
        // C�digo de la aplicaci�n
    }
}

/* 1MS Timer callback */

void diskTickHook(void *ptr)
{
    /* Update Display driver timers. */
    ili9341_1ms_timer_cb();
}