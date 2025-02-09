#include "../inc/app.h"

#include "ili9341.h"
#include "sapi.h"

void inicializacion(void);

int main(void) {

    inicializacion();
    inicializar todo

    // C digo de aplicaci n
    while(1){
        switch (estado){

            case inicio:
                if (estadoAnt!= inicio)
                    //dibujo de menu principal con nombre de juego y boton de "iniciar"
                    ili9341_spi_init(100000000);
                    ili9341_paintBackground(display, 0);
                    ili9341_setTextSize(5);
                    ili9341_setCursor(0, 10);
                    ili9341_setTextColor(0xdaa0, 0);
                    ili9341_printStr(display, "GENERATOR");

                    ili9341_drawRectangle(display, 60, 140, 200, 40, 0xdaa0);
                    ili9341_setTextSize(3);
                    ili9341_setCursor(110, 150);
                    ili9341_setTextColor(0, 0xdaa0);
                    ili9341_printStr(display, "Iniciar");

                    estadoAnt = inicio;
                    leerTouch();

                    if (boton apretado)
                        estado = reposo;
                    break;
            case reposo:
                if ((estadoAnt != reposo)&&(estadoAnt != sacudiendo)){
                    //dibujar el fondo y borde de dados

                    ili9341_spi_init(100000000);
                    ili9341_paintBackground(display, 0xfd35);
                    ili9341_setDadoFondo(0xFFFF);
                    ili9341_setDadoBorde(0);
                    for (int i = 1; i < 6; i++){
                        ili9341_drawDadoBase(display, i);
                    }
                }
                estadoAnt = reposo;

                if (no es la primera vez)
                {
                    // Seleccionar todos los dados
                    ili9341_spi_init(100000000);
                    for (int i = 1; i < 6; i++){
                        ili9341_seleccionarDado(display, i, 0);
                    }
                    //Imprimir nombre del juego
                    //acá falta implementar la funcion que calcule estas dos cosas
                    ili9341_setTextSize(3);
                    ili9341_setCursor(110, 30);
                    ili9341_setTextColor(0xFFFF, 0xfd35);
                    ili9341_printStr(display, "NombreJuego");

                    //Imprimir puntaje

                    ili9341_setTextSize(5);;
                    ili9341_setCursor(130, 180);
                    ili9341_printStr(display, "50");

                    if(leerTouch() == algunDado){
                        Selecciono/deselecciono el dado
                        Y dibujo la seleccion
                    }

                }

                
                if(Se sacude && algun dado sin seleccionar){
                    veces++;
                    if veces>5 {
                        estado = sacudiendo;
                        veces = 0;
                    }
                }

                break;

            case sacudiendo:
                estadoAnt = sacudiendo;

                //Randomizar dados no seleccinados y dibujarlos.
                //falta implementar la fora de randomizar y escribir el arreglo que va a tener el numero de los dados

                ili9341_spi_init(100000000);
                for (int i = 1; i < 6; i++){
                    ili9341_drawDadoNumero(display, i, 6);
                }
                cada(x veces){
                    Reproducir sonido
                }

                if(NO Se sacude){
                    veces++;
                    if veces>5 {
                        estado = reposo;
                        veces = 0;
                    }
                }

                break;

            default:
                break;
            }
        wait(0.25s);
    }




    
   
}

/* 1MS Timer callback */

void inicializacion(void){
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
        .restart_delay_ms = 20};

    const ili9341_hw_cfg_t hw_cfg = ili9341_get_default_hw_cfg();

    display = ili9341_init(&display_cfg, &hw_cfg);
    if (display == NULL)
    {
        // Error_Handler();
    }
}

void diskTickHook(void *ptr){
    /* Update Display driver timers. */
    ili9341_1ms_timer_cb();
}