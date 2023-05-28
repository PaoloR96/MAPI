#include "header.h"

/*Esercitazione 3 - Vogliamo risolvere il problema che affliggeva l'Esercitazione 1, cioè attendere un tempo aleatorio per effettuare 
  il conteggio. Per questo motivo realizziamo un contatore binario sfruttando i timer */


int main(){
  
  configurazione_dispositivo();
  configurazione_timer();
  contatore();
  return 0;
  
}