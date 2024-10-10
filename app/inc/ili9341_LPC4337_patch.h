#ifndef ILI9341_ILI9341_LPC4337_PATCH_H_
#define ILI9341_ILI9341_LPC4337_PATCH_H_

#include "board.h"
#include "sapi.h"
//#include "lpc_open.h"
#include "ili9341.h"

/*
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
*/

void diskTickHook(void *ptr);
void ili9341_gpio_init(void);
void ili9341_spi_init(void);
void gpio_cs_pin(ili9341_gpio_pin_value_t value);
void gpio_dc_pin(ili9341_gpio_pin_value_t value);
void gpio_rst_pin(ili9341_gpio_pin_value_t value);
bool spi_tx_dma_ready(void);
int spi_tx_dma_b(const uint8_t* data, uint32_t len);


/* Cons */

#define CS_PIN      GPIO0
#define DC_PIN      GPIO2
#define RST_PIN     GPIO4

#endif // ILI9341_ILI9341_LPC4337_PATCH_H_
