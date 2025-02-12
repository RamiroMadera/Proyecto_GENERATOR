#include "XPT2046_Touchscreen.h"

Rectangle Dado1,Dado2,Dado3,Dado4,Dado5,Push2Start;

static void InitRectangles(){
    Dado4.xi=1; Dado4.yi=1; Dado4.xs=4; Dado4.ys=5;
    Dado1.xi=1; Dado1.yi=8; Dado1.xs=4; Dado1.ys=12;
    Dado3.xi=6; Dado3.yi=4; Dado3.xs=9; Dado3.ys=8;
    Dado2.xi=10; Dado2.yi=8; Dado2.xs=13; Dado2.ys=12;
    Dado5.xi=10; Dado5.yi=1; Dado5.xs=13; Dado5.ys=5;
    Push2Start.xi=3; Push2Start.yi=3; Push2Start.xs=11; Push2Start.ys=5;
    return;
}

//Inicializa el t�ctil
bool_t XPT2046_Touchscreen_begin(void) {
    gpioConfig(CST_PIN, GPIO_OUTPUT);        //Configura canal de comunicacion activo en bajo
    //gpioConfig(IRQT_PIN, GPIO_INPUT);      //Para interrupcion
    InitRectangles();
    //bool ans=spiInit(SPI0,100000000); //Ya lo hace el display
    bool ans=true;
    return ans;
}



//Actualiza la info de la pantalla
void XPT2046_Touchscreen_readData(TS_Point *punto) {
    uint8_t read[2];
    uint8_t write;
    uint16_t res;
    uint16_t x,y;
   
    //NVIC_DisableIRQ(PIN_INT1_IRQn);
    
    gpioWrite(CST_PIN, 0);

    write=CMD_READ_X;
    spiWrite(SPI0, &write, 1);
    spiRead(SPI0, &read[0], 2);
    res=(read[0] << 8) | (read[1] & 0xF0);
    res>>=4;
    //res&=0x0FFF;
    res=round((float)(res-2185)*(14)/(3977-2185));
    x=res;
    gpioWrite(CST_PIN, 1);

    gpioWrite(CST_PIN, 0);
    write=CMD_READ_Y;
    spiWrite(SPI0, &write, 1);
    spiRead(SPI0, &read[0], 2);
    res=(read[0] << 8) | (read[1] & 0xF0);
    res>>=4;
    //res&=0x0FFF;
    res=round((float)(res-2185)*(13)/(3849-2185));
    y=res;
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
    //printf("Coord Z1: (%d)   Coord Z2: (%d)   \r\n",z1,z2);
    z=z1/z2;
    punto->z=z;


    //printf("Coord X : (%d)   Coord Y: (%d)   ",x,y);
    if(punto->x != x || punto->y !=y){
       if(punto->amount < 5) punto->amount=0;
       else{
          punto->tolerancia++;
          if(punto->tolerancia > 1){
             punto->amount=0;
             punto->tolerancia=0;
          }
       }
    }else{
       punto->amount=(punto->amount)+1;
       punto->tolerancia=0;
    }
    //printf("Cantidad: (%d)   \r\n",punto->amount);
    punto->x=x;
    punto->y=y;
    if(punto->amount >= 5){
       //printf("Estoy tocando algooo");
       if(punto->firsttouch) punto->firsttouch=false;
       else punto->firsttouch=true;
    }else punto->firsttouch=false;
    //NVIC_EnableIRQ(PIN_INT1_IRQn);

}

bool_t PointInRectangle(TS_Point *punto, Rectangle *boton){
    if((punto->firsttouch == false)) return false;
    if((punto->x)>=(boton->xi) && (punto->x)<=(boton->xs) && (punto->y)>=(boton->yi) && (punto->y)<=(boton->ys)) return true;
    else return false;
}

int16_t SelectDado(TS_Point *punto){
    if(PointInRectangle(punto,&Dado1)) return 1;
    if(PointInRectangle(punto,&Dado2)) return 2;
    if(PointInRectangle(punto,&Dado3)) return 3;
    if(PointInRectangle(punto,&Dado4)) return 4;
    if(PointInRectangle(punto,&Dado5)) return 5;
    return 0;
}

bool_t InStart(TS_Point *punto){
    if(PointInRectangle(punto,&Push2Start)) return 1;
    return 0;
}
