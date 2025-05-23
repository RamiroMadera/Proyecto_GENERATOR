#include "../inc/Generator.h"

#include <time.h>
#include "ili9341.h"
#include "sapi.h"
#include "XPT2046_Touchscreen.h"
#include "mpu6050.h"
#include "sonidos.h"

typedef enum {inicio,reposo,sacudiendo,apagado} State;
typedef enum {emayor,emenor,full,poker,generala,nada,j1,j2,j3,j4,j5,j6} Juego;

ili9341_desc_ptr_t display;
TS_Point valPantalla;
State estado,estadoAnt;
int16_t veces=0;
uint8_t dados[5]={0,0,0,0,0};
bool seleccion[5]={false,false,false,false,false};
uint8_t suma;
int16_t seedinicio,seedreposo;
char puntos[12];
Juego tirada;

void inicializacion(void);

void LeerPantalla(void);

Juego CalcularJuego(uint8_t *dados, uint8_t *puntaje);

int main(void) {
    inicializacion();
    // C digo de aplicacion
   delay(150);
   printf("Comenzando ... \r\n");
    while(1){
        //Lecturas de perifericos
        //Aca tiene que ir la lectura del MPU para ver si se esta moviendo

        //MEF de control del dibujito
        switch (estado){
            case inicio:
                if ((estadoAnt!= inicio)){
                    //dibujo de menu principal con nombre de juego y boton de "iniciar"
                    ili9341_spi_init(100000000);

                    ili9341_paintBackground(display, 0);
                    coord_2d_t square_top_left = {110, 70}; // (320-100)/2, (240-100)/2
                    coord_2d_t square_bottom_right = {210, 170};

                    ili9341_set_region(display, square_top_left, square_bottom_right);
                    ili9341_fill_region(display, RED);
                    ili9341_setTextSize(1);
                    ili9341_setTextColor(BLACK, RED);
                    ili9341_setCursor(115, 137);
                    ili9341_printStr(display, "RED");
                    ili9341_setCursor(115, 147);
                    ili9341_printStr(display, "SQUARE");
                    ili9341_setCursor(115, 157);
                    ili9341_printStr(display, "PRODUCTIONS_");

                    ili9341_setTextColor(WHITE, BLACK);
                    ili9341_setCursor(5, 206);
                    ili9341_printStr(display, "GARCIA");
                    ili9341_setCursor(5, 216);
                    ili9341_printStr(display, "MADERA");
                    ili9341_setCursor(5, 226);
                    ili9341_printStr(display, "PEREIRA_");

                    delay_ms(3000);
                    ili9341_paintBackground(display, 0);

                    uint16_t col = YELLOW;
                    ili9341_drawHLine(display, 141, 1, 179 - 141, col);
                    ili9341_drawHLine(display, 137, 2, 183 - 137, col);
                    ili9341_drawHLine(display, 134, 3, 186 - 134, col);
                    ili9341_drawHLine(display, 132, 4, 188 - 132, col);
                    
                    ili9341_drawHLine(display, 124, 8, 196 - 124, col);
                    ili9341_drawHLine(display, 122, 9, 198 - 122, col);
                    ili9341_drawHLine(display, 120, 10, 200 - 120, col);
                    ili9341_drawHLine(display, 119, 11, 201 - 119, col);

                    ili9341_drawHLine(display, 113, 15, 207 - 113, col);
                    ili9341_drawHLine(display, 112, 16, 208 - 112, col);
                    ili9341_drawHLine(display, 111, 17, 209 - 111, col);
                    ili9341_drawHLine(display, 110, 18, 210 - 110, col);

                    ili9341_drawHLine(display, 106, 22, 214 - 106, col);
                    ili9341_drawHLine(display, 105, 23, 215 - 105, col);
                    ili9341_drawHLine(display, 104, 24, 216 - 104, col);
                    ili9341_drawHLine(display, 103, 25, 217 - 103, col);

                    ili9341_drawHLine(display, 100, 29, 220 - 100, col);
                    ili9341_fillRect(display, 99, 30, 221 - 99, 2, col);
                    ili9341_drawHLine(display, 98, 32, 222 - 98, col);

                    col = ORANGE;
                    ili9341_fillRect(display, 95, 36, 225 - 95, 2, col);
                    ili9341_fillRect(display, 94, 38, 226 - 94, 2, col);

                    ili9341_drawHLine(display, 92, 43, 228 - 92, col);
                    ili9341_fillRect(display, 91, 44, 229 - 91, 3, col);

                    ili9341_setTextSize(5);
                    ili9341_setCursor(28, 50);
                    ili9341_setTextColor(0xdaa0, 0);
                    ili9341_printStr(display, "GENERATOR");

                    ili9341_fillRect(display, 89, 93, 231 - 89, 2, col);
                    ili9341_fillRect(display, 90, 95, 230 - 90, 2, col);

                    ili9341_fillRect(display, 92, 100, 228 - 92, 3, col);
                    ili9341_fillRect(display, 93, 103, 227 - 93, 1, col);
                    
                    col = RED;
                    ili9341_fillRect(display, 95, 107, 225 - 95, 2, col);
                    ili9341_fillRect(display, 96, 109, 224 - 96, 2, col);

                    ili9341_fillRect(display, 99, 114, 221 - 99, 1, col);
                    ili9341_fillRect(display, 100, 115, 220 - 100, 2, col);
                    ili9341_fillRect(display, 101, 117, 219 - 101, 1, col);

                    ili9341_fillRect(display, 104, 121, 216 - 104, 1, col);
                    ili9341_fillRect(display, 105, 122, 215 - 105, 1, col);
                    ili9341_fillRect(display, 106, 123, 214 - 106, 1, col);
                    ili9341_fillRect(display, 107, 124, 213 - 107, 1, col);

                    ili9341_fillRect(display, 111, 128, 209 - 111, 1, col);
                    ili9341_fillRect(display, 112, 129, 208 - 112, 1, col);
                    ili9341_fillRect(display, 113, 130, 207 - 113, 1, col);
                    ili9341_fillRect(display, 115, 131, 205 - 115, 1, col);

                    ili9341_fillRect(display, 120, 135, 200 - 120, 1, col);
                    ili9341_fillRect(display, 122, 136, 198 - 122, 2, col);
                    ili9341_fillRect(display, 124, 137, 196 - 124, 2, col);
                    ili9341_fillRect(display, 126, 138, 194 - 126, 2, col);   



                    ili9341_drawRectangle(display, 57, 142, 206, 46, 0xdaa0);
                    ili9341_setTextSize(3);
                    ili9341_setCursor(100, 153);
                    ili9341_setTextColor(0, 0xdaa0);
                    ili9341_printStr(display, "Iniciar");

                    //detalles botón
                    ili9341_drawRectangle(display, 56, 140, 208, 2, 0x8180);
                    ili9341_drawRectangle(display, 55, 141, 2, 48, 0x8180);
                    ili9341_drawRectangle(display, 56, 188, 208, 2, 0x8180);
                    ili9341_drawRectangle(display, 263, 141, 2, 48, 0x8180);

                    
                    ili9341_drawRectangle(display, 64, 147, 30, 2, 0x9820);
                    ili9341_drawRectangle(display, 62, 149, 2, 15, 0x9820);

                    ili9341_drawRectangle(display, 226, 181, 30, 2, 0x9820); //esquina 256, 181
                    ili9341_drawRectangle(display, 256, 166, 2, 15, 0x9820);
                    //

                    estadoAnt = inicio;
                }
                LeerPantalla();
                if (InStart(&valPantalla)){
                    estado = reposo;
                    printf("Cambio de estado \r\n");
                }
                seedinicio++;
            break;
            case reposo:
               if ((estadoAnt != reposo)&&(estadoAnt != sacudiendo)){
                  //dibujar SOLO el fondo y borde de dados
                  ili9341_spi_init(100000000);
                  ili9341_paintBackground(display, 0xc0e6);
                  ili9341_setDadoFondo(0xFFFF);
                  ili9341_setDadoBorde(0);
                  for (int i = 1; i < 6; i++){
                     ili9341_drawDadoBase(display, i);
                  }
                  seedreposo=0;
               }

               seedreposo++;
               
               if (estadoAnt != reposo)
               {
                  if (estadoAnt == sacudiendo)
                  {
                     //Calculo los nuevos valores
                     for (int i = 1; i < 6; i++) if(!seleccion[i-1]){
                        dados[i-1]=(rand()%6+1);
                        
                     }
                  }
                  // Seleccionar todos los dados
                  ili9341_spi_init(100000000);
                  for (int i = 1; i < 6; i++){ 
                     if (dados[i-1]>0) {
                        ili9341_seleccionarDado(display, i, 0);
                        ili9341_drawDadoNumero(display, i, dados[i-1]);
                        seleccion[i-1]=true;
                     } else {
                        ili9341_drawDadoNumero(display, i, dados[i-1]);
                     }
                  }
                  if (dados[0]>0) reproducirDados();

                  tirada=CalcularJuego(dados,&suma);
                  
                  //for (int i = 0; i < 5; i++) suma+=dados[i];
                  //Antes de todo esto para mi vamos a tener que limpiar el fondo porque (sin probarlo)(Uli) para mi reescribe
                  //Imprimir nombre del juego
                  ili9341_setTextSize(3);
                  ili9341_setCursor(135, 30);
                  ili9341_setTextColor(0xFFFF, 0xc0e6);
                  switch (tirada) {
                     case generala:
                        ili9341_printStr(display, "GEN");
                        reproducirVictoria();
                     break;
                     case poker:
                        ili9341_printStr(display, "POK");
                     break;
                     case full:
                        ili9341_printStr(display, "FUL");
                     break;
                     case emayor:
                        ili9341_printStr(display, "EMA");
                     break;
                     case emenor:
                        ili9341_printStr(display, "EME");
                     break;
                     case j1:
                        ili9341_printStr(display, "UNO");
                     break;
                     case j2:
                        ili9341_printStr(display, "DOS");
                     break;
                     case j3:
                        ili9341_printStr(display, "TRE");
                     break;
                     case j4:
                        ili9341_printStr(display, "CUA");
                     break;
                     case j5:
                        ili9341_printStr(display, "CIN");
                     break;
                     case j6:
                        ili9341_printStr(display, "SEI");
                     break;
                     case nada:
                        ili9341_printStr(display, "G0!");
                     break;
                  }
                  //Imprimir puntaje
                  ili9341_setTextSize(5);;
                  ili9341_setCursor(135, 180);
                  //ili9341_printStr(display, "50");
                  sprintf(puntos, "%d", suma);
                  if(suma>0) {
                     if(suma<10) ili9341_printStr(display, "0");
                     ili9341_printStr(display, puntos);
                  }
               }
               
               estadoAnt = reposo;
               LeerPantalla();
               int16_t dado=SelectDado(&valPantalla);
               if(dado && dados[dado-1]>0){
                  //Selecciono/deselecciono el dado
                  //Y dibujo la seleccion
                  if (seleccion[dado-1]) {
                     ili9341_seleccionarDado(display, dado, 0xc0e6);
                     seleccion[dado-1]=false;
                  } else {
                     ili9341_seleccionarDado(display, dado, 0);
                     seleccion[dado-1]=true;
                  }
                  printf("Dado Numero (%d) \r\n");
               }
               if (agitando(8) && (!seleccion[0] || !seleccion[1] || !seleccion[2] || !seleccion[3] || !seleccion[4]) ) {      //Falta agregar la condicion de dados no seleccionados
                  if (!seleccion[0]) printf("1 ");
                  if (!seleccion[1]) printf("2 ");
                  if (!seleccion[2]) printf("3 ");
                  if (!seleccion[3]) printf("4 ");
                  if (!seleccion[4]) printf("5 \r\n");

                  estado = sacudiendo;
               }

            break;

            case sacudiendo:
               if(dados[0]==0) srand (seedinicio*seedreposo);
                estadoAnt = sacudiendo;
                //Randomizar dados no seleccinados y dibujarlos.
                //falta implementar la fora de randomizar y escribir el arreglo que va a tener el numero de los dados
               ili9341_spi_init(100000000);
               while (agitando(0.1)){
                for (int i = 1; i < 6; i++){
                    if (!seleccion[i-1]){
                       ili9341_drawDadoNumero(display, i, (rand())%6+1);
                     }
                }
                printf("Sacudiendooooo \r\n");
                veces++;
                reproducirSacudir();
               }
                if(!agitando(0.1) && veces >= 3){
                        estado = reposo;
                        veces = 0;
                }
                break;

            default:
                break;
            }
        delay_ms(25);
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
    valPantalla.amount=0;
    valPantalla.tolerancia=0;
    valPantalla.firsttouch=false;

    //Aca va la inicializacion del MPU
    mpu6050Init();

    //Aca va la inicializacion del AmpOp de sonido
    inicializarParlante();
    tirada=nada;
    suma=0;
    estado=inicio;
    estadoAnt=apagado;
    seedinicio=0;
    seedreposo=0;
    
    return;
}

void LeerPantalla(void){
   ili9341_spi_init(400000);
   XPT2046_Touchscreen_readData(&valPantalla);  //Esto puede quedar aca o dentro de los cases
   ili9341_spi_init(100000000);
}

Juego CalcularJuego(uint8_t *dados,uint8_t *puntaje){
   uint8_t histograma[6]={0,0,0,0,0,0};
   uint8_t nmax=0,puntmax=0;
   uint8_t histohisto[6]={0,0,0,0,0,0};
   bool seis=false,uno=false;
   for (int i = 0; i < 5; i++){
      histograma[dados[i]-1]++;
      if(dados[i]==6) seis=true;
      if(dados[i]==1) uno=true;
   }
   for (int i = 0; i < 6; i++){
      if(puntmax<(histograma[i])*(i+1)){
         puntmax=(histograma[i])*(i+1);
         nmax=(i+1);
      }
      histohisto[histograma[i]]++;
   }
   //Si es generala
   if(histohisto[5]==1) {
      *puntaje=50;
      return generala;
   }
   //Si es poker
   if(histohisto[4]==1){
      *puntaje=40;
      return poker;
   }
   //Si es full
   if(histohisto[3]==1 && histohisto[2]==1){
      *puntaje=30;
      return full;
   }
   //Si es escalera mayor o menor
   if(histohisto[1]==5){
      if(seis && !uno){
         *puntaje=25;
         return emayor;
      }
      if(!seis && uno){
         *puntaje=20;
         return emenor;
      }
   }
   *puntaje=puntmax;
   switch(nmax){
      case 1:
         return j1;
         break;
      case 2:
         return j2;
         break;
      case 3:
         return j3;
         break;
      case 4:
         return j4;
         break;
      case 5:
         return j5;
         break;
      case 6:
         return j6;
         break;
   }
   *puntaje=0;
   return nada;
}

void diskTickHook(void *ptr){
    /* Update Display driver timers. */
    ili9341_1ms_timer_cb();
}