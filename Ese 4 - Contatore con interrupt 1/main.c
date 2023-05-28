#include "header.h"

/* Esercitazione 4 - Il problema fondamentale dell'esercitazione 3 era il continuo polling sul campo SR del TIM6, che può 
   portare a ritardi nel conteggio. In questa soluzione vogliamo realizzare sempre un contatore binario, però andando ad 
   utilizzare le interrupt che eliminano il problema del polling. Utilizzeremo un interrupt di alto livello, dove per alto
   livello si intende interrupt già predisposte nella libreria e che non devo essere realizate ex-novo. */


int main (){
  
  configurazione_dispositivo();
  contatore();
  return 0;
  
}