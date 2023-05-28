#include "header.h"

/*Esercitazione 8 - Utilizzando il convertitore AD ottenere una tensione misurata in mV, dove tale tensione è quella interna al dispositivo e 
  grazie a quest'ultima calcolare la temperatura a cui lavora il dispositivo stesso */

int calibrazione = 0;
int tensione = 0;
float temperatura = 0.0;

int main(){
  configurazione_ADC();
  calcolo_risultati();
  while(1);
  
}