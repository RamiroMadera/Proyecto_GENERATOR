#include "../inc/app.h"

#include "ili9341.h"
#include "sapi.h"
#include "XPT2046_Touchscreen.h"

typedef enum {inicio,reposo,sacudiendo,apagado} State;

TS_Point valPantalla;
State estado,estadoAnt;

void inicializacion(void);

int main(void) {

    inicializacion();
    

    // C digo de aplicacion
    while(1){
        //Lecturas de los perifericos
        XPT2046_Touchscreen_readData(valPantalla);  //Esto puede quedar aca o dentro de los cases
        //Aca tiene que ir la lectura del MPU para ver si se esta moviendo

        //MEF de control del dibujito
        switch (estado){
            case inicio:
                if (estadoAnt!= inicio){
                    //dibujo de menu principal con nombre de juego y boton de "iniciar" 
                    
                    //Actualizamos estado previo
                    estadoAnt = inicio;
                }

                if (InStart(valPantalla)){
                    estado = reposo;
                }
            break;
            case reposo:
                if (estadoAnt != reposo){
                    dibujar el fondo y borde de dados
                }
                estadoAnt = reposo;

                if (no es la primera vez)
                {
                    Seleccionar todos los dados
                    Imprimir nombre del juego
                    Imprimir puntaje

                    int16_t dado=SelectDado(valPantalla);
                    if(dado){
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

                Randomizar dados no seleccinados y dibujarlos.

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

    //Inicializacion del XPT2046
    if(!XPT2046_Touchscreen_begin()){
        printf( "Error al Inicializar el Touch.");
    }
    valPantalla.firsttouch=false;
    valPantalla.x=-1;
    valPantalla.y=-1;
    valPantalla.z=-1;

    //Aca va la inicializacion del MPU

    //Aca va la inicializacion del AmpOp de sonido

    estado=reposo;
    estadoAnt=apagado;
    return;
}

void diskTickHook(void *ptr){
    /* Update Display driver timers. */
    ili9341_1ms_timer_cb();
}