#include "prueba_pantalla.h"
#include "ili9341.h"
#include "sapi.h"
#include "ili9341_LPC4337_patch.h"

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
    ili9341_fill_region(display, IBLACK);

    //Imprimir carácteres
    uint16_t x = 55;
    uint16_t y = 190;
    uint8_t size = 2;
    uint16_t color = IGREEN;
    uint16_t bg = IBLACK;

    ili9341_setCursor(display, x, y);
    ili9341_setTextSize(display, size);
    ili9341_setTextColor(display, color, bg);
    ili9341_print_str(display, "HOLA FUCKING MUNDO!!");
    ili9341_print_str(display, "");



    //imprimir imagen
    int WIDTH = 100;
    int HEIGHT = 100;

    uint8_t image_data[WIDTH * HEIGHT * 2]; // Array para la imagen 100x100
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            // Generar un color basado en las coordenadas (x, y)
            uint16_t red = (x * 31) / WIDTH;                   // Rojo: 5 bits (0-31)
            uint16_t green = (y * 63) / HEIGHT;                // Verde: 6 bits (0-63)
            uint16_t blue = ((x + y) * 31) / (WIDTH + HEIGHT); // Azul: 5 bits (0-31)

            uint16_t color = (red << 11) | (green << 5) | blue; // RGB565
            int index = (y * WIDTH + x) * 2;

            image_data[index] = (color >> 8) & 0xFF; // Byte alto
            image_data[index + 1] = color & 0xFF;    // Byte bajo
        }
    }

    coord_2d_t top_left = {1, 1};         // Esquina superior izquierda (x, y)
    coord_2d_t bottom_right = {1 + WIDTH, 1+ HEIGHT}; // Esquina inferior derecha (x, y) para una pantalla completa 320x240
    uint32_t image_size = 100 * 100 * 2;  // Cada píxel ocupa 2 bytes en RGB565.

    int result = ili9341_set_region(display, top_left, bottom_right);

    result = ili9341_draw_RGB565_dma(display, image_data, image_size);

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