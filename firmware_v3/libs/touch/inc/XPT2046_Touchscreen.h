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

#define CMD_READ_X  0x95  // Command for X
#define CMD_READ_Y  0xD5  // Command for Y
#define CMD_READ_Z1 0xB5  // Command for Z1
#define CMD_READ_Z2 0xC5  // Command for Z2

#define CST_PIN     GPIO3
#define IRQT_PIN    GPIO1

#define GPIO_INTERRUPT_PIN 1    // Pin 1 (GPIO1)
#define RAISING_EDGE          0  // Interrupci�n en el borde de subida


//Estrctura de punto
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
    bool_t firsttouch;
} TS_Point;

typedef struct{
    int16_t xi;
    int16_t xs;
    int16_t yi;
    int16_t ys;
}Rectangle;

bool_t TS_Point_equal(TS_Point *p1, TS_Point *p2);
bool_t TS_Point_notEqual(TS_Point *p1, TS_Point *p2);

//Declaraci�n de funciones
bool_t XPT2046_Touchscreen_begin(void);
void XPT2046_Touchscreen_readData(TS_Point *punto);
bool_t PointInRectangle(TS_Point *punto, Rectangle *boton);
int16_t SelectDado(TS_Point *punto);
bool_t InStart(TS_Point *punto);

#endif // XPT2046_TOUCHSCREEN_H