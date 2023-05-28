#include "header.h"

/*Esercitazione 10 - Utilizzare il DAC e ADC per verificare la conversione dei due elementi. Ricordiamo che il TIM7 è legato all'ADC 
  mentre il TIM6 al DAC. */


int main(){
  
  configurazione_dispositivo();
  configurazione_ADC();
  configurazione_DAC();
  configurazione_timer7();
  for (int i = 0;i<34;i++); //Tempo di attesa tra i due timer
  configurazione_timer6();
  while(1);
  
}

/*N.B. Dalle spiegazioni fatte dal prof. in Laboratorio sembra che il dispositivo ADC fornisca un input al dispositivo DAC e tale 
  supposizione è confermata dal fatto che ADC sia sul pin PA2 di input, mentre DAC sul pin PA4 che è di output. In realtà in questa 
  Esercitazione avvine il perfetto opposto.*/

/*N.B.1 Il risultato come sempre può essere visto attraverso il Live Wacth e si discosterà dal codice inserito di almeno 2 o 3 codici. */