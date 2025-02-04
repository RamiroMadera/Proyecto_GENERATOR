#include "ili9341_LPC4337_patch.h"  // Incluye el archivo de cabecera correspondiente


void ili9341_gpio_init(void){
    gpioConfig(CS_PIN, GPIO_OUTPUT);        //esto lo hice basándome en el ejemplo de examples>c>sapi>gpio>switches_leds
    gpioConfig(DC_PIN, GPIO_OUTPUT);
    gpioConfig(RST_PIN, GPIO_OUTPUT);
}

void ili9341_spi_init(uint32_t bitRate){
    static uint32_t first = 1;
    if (first)
        first = 0;
    else
        Chip_SSP_DeInit(LPC_SSP1);
    spiInit(SPI0, bitRate);
}

void gpio_cs_pin(ili9341_gpio_pin_value_t value) {
    if (value == ILI9341_PIN_RESET) {
        gpioWrite(CS_PIN, 0);
    } else {
        gpioWrite(CS_PIN, 1);
    }
}

void gpio_dc_pin(ili9341_gpio_pin_value_t value) {
    if (value == ILI9341_PIN_RESET) {
        gpioWrite(DC_PIN, 0);
    } else {
        gpioWrite(DC_PIN, 1);
    }
}

void gpio_rst_pin(ili9341_gpio_pin_value_t value) {
    if (value == ILI9341_PIN_RESET) {
        gpioWrite(RST_PIN, 0);
    } else {
        gpioWrite(RST_PIN, 1);
    }
}

int spi_tx_dma_b(const uint8_t* data, uint32_t len) {

    bool_t result = spiWrite(SPI0, data, len);      //esto lo hice basándome en Libs>Sapi>Sapi_v0.5.2>Soc>Peripherals>src>sapi_spi.c

    if (result)
        return 0; // Éxito
    else return 1; //Fail
}

bool spi_tx_dma_ready(void) {
    //en página 1174 del datasheet

    // Código para verificar si el SPI está listo para transmitir
    return (Chip_SSP_GetStatus(LPC_SSP1, SSP_STAT_TNF) == SET);         // line160 -->  \libs\lpc_open\lpc_chip_43xx\src\ssp_18xx_43xx.c
    // return (LPC_SSP1->SR & SPI_SR_TNF) != 0; // Ejemplo: Verifica la bandera de no lleno
}

