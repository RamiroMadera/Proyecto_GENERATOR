#include "XPT2046_Touchscreen.h"

//Inicializa el táctil
bool_t XPT2046_Touchscreen_begin(void) {
    gpioConfig(CST_PIN, GPIO_OUTPUT);        //Configura canal de comunicacion activo en bajo
    //gpioConfig(IRQT_PIN, GPIO_INPUT);      //Para interrupcion
    bool ans=spiInit(SPI0); //Ya lo hace el display
    return ans;
}



//Actualiza la info de la pantalla
void XPT2046_Touchscreen_readData(TS_Point *punto) {
    
    uint8_t read[2];
    uint8_t write;
    uint16_t res;
   
    //NVIC_DisableIRQ(PIN_INT1_IRQn);
    
    gpioWrite(CST_PIN, 0);

    write=CMD_READ_X;
    spiWrite(SPI0, &write, 1);
    spiRead(SPI0, &read[0], 2);
    res=(read[0] << 8) | (read[1] & 0xF0);
    res>>=4;
    //res&=0x0FFF;
    res=round((float)(res-2185)*(14)/(3977-2185));
    punto->x=res;
    gpioWrite(CST_PIN, 1);

    gpioWrite(CST_PIN, 0);
    write=CMD_READ_Y;
    spiWrite(SPI0, &write, 1);
    spiRead(SPI0, &read[0], 2);
    res=(read[0] << 8) | (read[1] & 0xF0);
    res>>=4;
    //res&=0x0FFF;
    res=round((float)(res-2185)*(13)/(3849-2185));
    punto->y=res;
    gpioWrite(CST_PIN, 1);

    uint16_t z1,z2,z;
    gpioWrite(CST_PIN, 0);
    write=CMD_READ_Z1;
    spiWrite(SPI0, &write, 1);
    spiRead(SPI0, &read[0], 2);
    res=(read[0] << 8) | (read[1] & 0xFF);
    //res>>=4;
    res&=0x0FFF;
    z1=res;
    //printf( "Zeta 1:  (%d) ",z1);
    gpioWrite(CST_PIN, 1);

    gpioWrite(CST_PIN, 0);
    write=CMD_READ_Z2;
    spiWrite(SPI0, &write, 1);
    spiRead(SPI0, &read[0], 2);
    res=(read[0] << 8) | (read[1] & 0xFF);
    //res>>=4;
    res&=0x0FFF;
    z2=res;
    //printf( "Zeta 2:  (%d) ",z2);
    gpioWrite(CST_PIN, 1);
    z=z1/z2;
    punto->z=z;

    //NVIC_EnableIRQ(PIN_INT1_IRQn);

}