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

    
   
}

/* 1MS Timer callback */

void diskTickHook(void *ptr)
{
    /* Update Display driver timers. */
    ili9341_1ms_timer_cb();
}