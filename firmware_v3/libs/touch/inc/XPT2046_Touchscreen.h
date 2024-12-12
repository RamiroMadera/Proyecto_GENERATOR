#ifndef XPT2046_TOUCHSCREEN_H
#define XPT2046_TOUCHSCREEN_H

#include <stdint.h>
#include <stdbool.h>
#include <sapi_spi.h>
#include <sapi_gpio.h>

#define Z_THRESHOLD 300
#define Z_THRESHOLD_INT 75
#define MSEC_THRESHOLD 3
#define PENIRQ_PIN 11

#define CMD_READ_X  0x90  // Command for X
#define CMD_READ_Y  0xD0  // Command for Y
#define CMD_READ_Z1 0xB0  // Command for Z1
#define CMD_READ_Z2 0xF0  // Command for Z2

#define CST_PIN     GPIO3
#define IRQT_PIN    GPIO1

#define GPIO_INTERRUPT_PIN 1    // Pin 1 (GPIO1)
#define RAISING_EDGE          0  // Interrupción en el borde de subida


//Estrctura de punto
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} TS_Point;

bool_t TS_Point_equal(TS_Point *p1, TS_Point *p2);
bool_t TS_Point_notEqual(TS_Point *p1, TS_Point *p2);

/*
//Estructura del táctil
typedef struct {
    uint8_t csPin;
    uint8_t tirqPin;
    uint8_t rotation;
    uint16_t xraw;
    uint16_t yraw;
    uint16_t zraw;
    uint32_t msraw;
    bool_t isrWake;
} XPT2046_Touchscreen;
*/

//Declaración de funciones
bool_t XPT2046_Touchscreen_begin(void);
//TS_Point XPT2046_Touchscreen_getPoint(XPT2046_Touchscreen *touchscreen);
//bool_t XPT2046_Touchscreen_touched(XPT2046_Touchscreen *touchscreen);
//void XPT2046_Touchscreen_update(XPT2046_Touchscreen *touchscreen);
void XPT2046_Touchscreen_readData(TS_Point *punto);


// Function to set a user-defined callback
//void touch_set_callback(void (*callback)(int));

#endif // XPT2046_TOUCHSCREEN_H