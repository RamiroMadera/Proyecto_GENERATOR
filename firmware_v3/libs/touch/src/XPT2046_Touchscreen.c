#include "XPT2046_Touchscreen.h"

spiMap_T spi;

// Auxiliares
bool_t TS_Point_equal(TS_Point *p1, TS_Point *p2) {
    return (p1->x == p2->x && p1->y == p2->y && p1->z == p2->z);
}

bool_t TS_Point_notEqual(TS_Point *p1, TS_Point *p2) {
    return (p1->x != p2->x || p1->y != p2->y || p1->z != p2->z);
}

//Inicializa el táctil
bool_t XPT2046_Touchscreen_begin(XPT2046_Touchscreen *touchscreen, uint8_t csPin, uint8_t tirqPin) {
    touchscreen->csPin = csPin;
    touchscreen->tirqPin = tirqPin;
    touchscreen->rotation = 1;
    touchscreen->isrWake = true;
    touchscreen->xraw = 0;
    touchscreen->yraw = 0;
    touchscreen->zraw = 0;
    touchscreen->msraw = 0x80000000;
    //Inicialización de SPI
    bool ans=spiInit(spi);
    return true;
}

//Devuelve el punto tocado
TS_Point XPT2046_Touchscreen_getPoint(XPT2046_Touchscreen *touchscreen) {
    XPT2046_Touchscreen_update(touchscreen);
    TS_Point point = { touchscreen->xraw, touchscreen->yraw, touchscreen->zraw };
    return point;
}

//Se fija si la pantalla está siendo tocada
bool_t XPT2046_Touchscreen_touched(XPT2046_Touchscreen *touchscreen) {
    XPT2046_Touchscreen_update(touchscreen);
    return (touchscreen->zraw >= Z_THRESHOLD);
}

//Actualiza la info de la pantalla
void XPT2046_Touchscreen_update(XPT2046_Touchscreen *touchscreen) {
    // This function will implement the same data update logic found in the .cpp file
    // SPI transactions and data handling should be added here
    bool_t spiRead(spi, uint8_t* buffer, 8);    //Cambiar el buffer
    bool_t spiRead(spi, uint8_t* buffer, 8);
}

//Lee datos de la pantalla
void XPT2046_Touchscreen_readData(XPT2046_Touchscreen *touchscreen, uint16_t *x, uint16_t *y, uint8_t *z) {
    XPT2046_Touchscreen_update(touchscreen);
    *x = touchscreen->xraw;
    *y = touchscreen->yraw;
    *z = touchscreen->zraw;
}