#ifndef HEADER_H_
#define HEADER_H_

#include "stm32f30x.h"

/*A quanto pare le variabili che poi devono essere visualizzati attraverso il Live Wacth devono essere dichiarate come globali per 
  poter vedere i loro risultati. Altra accortenza deve essere che le variabili devono essere dichiarate esterne nel file .h, mentre
  nel main.c devono essere dichiarate inizializzandole.*/

extern int calibrazione,tensione;
extern float temperatura;

void configurazione_ADC();
void calcolo_risultati();

#endif