#include "header.h"

/*Esercitazione 6 - Voglio utilizzare il tasto USER come tasto di RESET del contatore, cioè premuto il tasto USER il conteggio riparte
  da 0 */


int main(){
  
  configurazione_dispositivo();
  configurazione_timer();
  configurazione_tastoUSER();
  
  while(1);
  
}