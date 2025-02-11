#include "sonidos.h"

void inicializarParlante() {
   dacConfig(DAC_ENABLE);
}

void reproducirDados() {
   for (int j = 0; j < DATA_DADOS; j++) {
      dacWrite(DAC, audioDados[j]);
      delayInaccurateUs(95);
   }
}

void reproducirVictoria() {
   for (int j = 0; j < DATA_VICTORIA; j++) {
      dacWrite(DAC, audioVictoria[j]);
      delayInaccurateUs(85);
   }
}

void reproducirSacudir() {
   for (int j = 4000; j < DATA_DADOS-3000; j++) {
      dacWrite(DAC, audioDados[j]);
      delayInaccurateUs(95);
   }
}