#include "header.h"

/* Esercitazione 5 - Realizzare un contatore binario sfruttando le interrupt a basso livello, quelle in cui devo scrivere la ISR necessaria
  a gestire l'interrupt che in questo caso sarà del timer 6 (soluzione adottata per eliminare il problema del polling) */


int main(){

        configurazione_dispositivo();
        configurazione_timer();
        while(1);


}