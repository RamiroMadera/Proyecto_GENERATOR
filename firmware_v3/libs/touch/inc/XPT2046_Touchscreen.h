#ifndef XPT2046_TOUCHSCREEN_H
#define XPT2046_TOUCHSCREEN_H

#include <stdint.h>
#include <stdbool.h>

#define Z_THRESHOLD 300
#define Z_THRESHOLD_INT 75
#define MSEC_THRESHOLD 3

//Estrctura de punto
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} TS_Point;

bool TS_Point_equal(TS_Point *p1, TS_Point *p2);
bool TS_Point_notEqual(TS_Point *p1, TS_Point *p2);

//Estructura del táctil
typedef struct {
    uint8_t csPin;
    uint8_t tirqPin;
    uint8_t rotation;
    int16_t xraw;
    int16_t yraw;
    int16_t zraw;
    uint32_t msraw;
    bool isrWake;
} XPT2046_Touchscreen;

//Declaración de funciones
bool XPT2046_Touchscreen_begin(XPT2046_Touchscreen *touchscreen, uint8_t csPin, uint8_t tirqPin);
TS_Point XPT2046_Touchscreen_getPoint(XPT2046_Touchscreen *touchscreen);
bool XPT2046_Touchscreen_touched(XPT2046_Touchscreen *touchscreen);
void XPT2046_Touchscreen_update(XPT2046_Touchscreen *touchscreen);
void XPT2046_Touchscreen_readData(XPT2046_Touchscreen *touchscreen, uint16_t *x, uint16_t *y, uint8_t *z);
void XPT2046_Touchscreen_setRotation(XPT2046_Touchscreen *touchscreen, uint8_t rotation);

#endif // XPT2046_TOUCHSCREEN_H
