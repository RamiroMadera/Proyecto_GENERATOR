#include "XPT2046_Touchscreen.h"



//Inicializa el t�ctil
bool_t XPT2046_Touchscreen_begin(void) {
    //touchscreen->csPin = csPin;
    //touchscreen->tirqPin = tirqPin;
    //touchscreen->rotation = 1;
    //touchscreen->isrWake = true;
    //touchscreen->xraw = 0;
    //touchscreen->yraw = 0;
    //touchscreen->zraw = 0;
    //touchscreen->msraw = 0x80000000;
    //Inicializaci�n de SPI
    
   
    gpioConfig(CST_PIN, GPIO_OUTPUT);        //esto lo hice bas�ndome en el ejemplo de examples>c>sapi>gpio>switches_leds
    gpioConfig(IRQT_PIN, GPIO_INPUT);
    
   
    bool ans=spiInit(SPI0); //Ya lo hace el display
    return ans;
}



//Actualiza la info de la pantalla
void XPT2046_Touchscreen_readData(TS_Point *punto) {
    // This function will implement the same data update logic found in the .cpp file
    // SPI transactions and data handling should be added here
    uint8_t read[2];
    uint8_t write;
    uint16_t res;

    gpioWrite(CST_PIN, 0);

    write=CMD_READ_X;
    spiWrite(SPI0, &write, 1);
    spiRead(SPI0, &read[0], 2);
    res=(read[0] << 8) | (read[1] & 0xF0);
    res>>=4;
    res&=0x0FFF;
    punto->x=res;

    write=CMD_READ_Y;
    spiWrite(SPI0, &write, 1);
    spiRead(SPI0, &read[0], 2);
    res=(read[0] << 8) | (read[1] & 0xF0);
    res>>=4;
    res&=0x0FFF;
    punto->y=res;

    uint16_t z1,z2,z;
    write=CMD_READ_Z1;
    spiWrite(SPI0, &write, 1);
    spiRead(SPI0, &read[0], 2);
    res=(read[0] << 8) | (read[1] & 0xF0);
    res>>=4;
    res&=0x0FFF;
    z1=res;

   write=CMD_READ_Z2;
   spiWrite(SPI0, &write, 1);
   spiRead(SPI0, &read[0], 2);
   res=(read[0] << 8) | (read[1] & 0xF0);
   res>>=4;
   res&=0x0FFF;
   z2=res;

    z=z1/z2;
    punto->z=z;

    gpioWrite(CST_PIN, 1);

}

/* NO SIRVE
// Auxiliares
bool_t TS_Point_equal(TS_Point *p1, TS_Point *p2) {
    return (p1->x == p2->x && p1->y == p2->y && p1->z == p2->z);
}

bool_t TS_Point_notEqual(TS_Point *p1, TS_Point *p2) {
    return (p1->x != p2->x || p1->y != p2->y || p1->z != p2->z);
}

//Devuelve el punto tocado
TS_Point XPT2046_Touchscreen_getPoint(XPT2046_Touchscreen *touchscreen) {
    XPT2046_Touchscreen_update(touchscreen);
    TS_Point point = { touchscreen->xraw, touchscreen->yraw, touchscreen->zraw };
    return point;
}

//Se fija si la pantalla est� siendo tocada
bool_t XPT2046_Touchscreen_touched(XPT2046_Touchscreen *touchscreen) {
    XPT2046_Touchscreen_update(touchscreen);
    return (touchscreen->zraw >= Z_THRESHOLD);
}

//Lee datos de la pantalla
void XPT2046_Touchscreen_readData(XPT2046_Touchscreen *touchscreen, uint16_t *x, uint16_t *y, uint16_t *z) {
    XPT2046_Touchscreen_update(touchscreen);
    *x = touchscreen->xraw;
    *y = touchscreen->yraw;
    *z = touchscreen->zraw;
}
*/