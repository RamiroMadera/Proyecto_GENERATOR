/*=============================================================================
 * Program: prueba_sonido
 * Date: 2024/12/06
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "prueba_sonido.h"
#include "sapi.h"
#include <math.h> // Para usar la función sin()

/*=====[Definition macros of private constants]==============================*/

#define SAMPLE_RATE 50000      // Frecuencia de muestreo en Hz
#define SINE_TABLE_SIZE 100   // Número de muestras de la tabla
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
/*=====[Definitions of private global variables]=============================*/

// Tabla de onda senoidal
uint16_t sineTable[SINE_TABLE_SIZE];

/*=====[Function declarations]===============================================*/

// Generar tabla de onda senoidal
void generateSineTable(void);

/*=====[Main function, program entry point after power on or reset]==========*/

int main(void) {
    /* ------------- INICIALIZACIONES ------------- */
    boardConfig();
    uartConfig(UART_USB, 115200);   // Inicializar UART
    dacConfig(DAC_ENABLE);         // Inicializar DAC

    // Generar tabla de onda senoidal
    generateSineTable();

    // Inicializar retardo no bloqueante
    delay_t delay1;
    delayConfig(&delay1, 1000 / SAMPLE_RATE); // Retardo según la frecuencia de muestreo

    uint16_t index = 0; // Índice de la tabla

    /* ------------- REPETIR POR SIEMPRE ------------- */
    while (1) {
        if (delayRead(&delay1)) {
            // Escribir valor de la tabla al DAC
            dacWrite(DAC, sineTable[index]);

            // Avanzar al siguiente índice de la tabla
            index = (index + 1) % SINE_TABLE_SIZE;
        }
    }

    // Nunca debe llegar aquí
    return 0;
}

/*=====[Function definitions]===============================================*/

// Generar una tabla de onda senoidal
void generateSineTable(void) {
    for (int i = 0; i < SINE_TABLE_SIZE; i++) {
        sineTable[i] = 512 + 511 * sin(2 * M_PI * i / SINE_TABLE_SIZE); // Rango 0-1023
    }
}
