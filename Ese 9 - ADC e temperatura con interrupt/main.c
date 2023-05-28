#include "header.h"

/*Esercitazione 9 - Voglio realizzare 100 misurazioni di temperatura, con intervalli di ogni secondo tra una misurazione e l'altra. Il tutto 
  utilizzando interrupt. */

int main(){
  configurazione_ADC();
  configurazione_timer();
  while(1);
}
