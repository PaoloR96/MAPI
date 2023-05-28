#include "header.h"


void configurazione_dispositivo(){
  
  RCC->AHBENR |= 1<<21;
  
  GPIOE->MODER = 0x55550000;
  GPIOE->OTYPER = 0;
  GPIOE->OSPEEDR = 0xFFFF0000;
  GPIOE->PUPDR = 0;
  
}

void configurazione_timer (){
  
  RCC->APB1ENR |= 1<<4; //Serve ad abilitare il TIMER 6 situato sul bus APB1 (Manuale pag 120)
  
  TIM6->CR1 = 0; //Registro di controllo in cui inizialmente disabilito tutte le opzioni su TIM6 (Manuale pag 593)
  TIM6->CR2 = 0; //Serve ad indicare la completa sincronizzazione con i timer interni al dispositivo in base ad un evento (Manuale pag 595)
  TIM6->DIER = 0; //Serve ad abilitare l'interrupt del TIM6. In questo caso non � presente nessuna interrupt (Manuale pag 595)
  
  /* Le seguenti due istruzioni sono molto importanti. Il dispositivo va a 72 MHz, cio� compie 72 milioni di istruzioni al secondo. In questa
     esercitazione voglio avere un conteggio al secondo (istruzione al secondo) e il metodo pi� rapido � dividere per 72 milioni, ma ci� non 
     � possibile visto che i registri sono a 16 bit. Il PRESCALER serve ad aggiornare la frequenza di conteggio, mentre con ARR indico il valore
     a cui devo arrivare con il conteggio.
  */
  TIM6->PSC = 7199; //Valore di PRESCALER che � il valore utile - 1 (se serve 7200, metto 7199) (Manuale pag 598)
  TIM6->ARR = 1000; //Valore a cui devo arrivare con il conteggio (Manuale pag 598)
  TIM6->CR1 |= 1;//Serve ad abiliatare il timer, in particolare il bit0 CEN che abilita il conteggio riferito al timer
  
}


void contatore (){
  int cont = 0;
  while (1){
    //Verifico il bit0 del registro SR se vi � stato l'evento di update (in pratica se � scattato il secondo) (Manuale pag 596)
    if (TIM6->SR & 0x1 == 0x1){
        TIM6->SR = 0; //Necessito di abbassare il bit nel registro SR, affinch� possa avvenire il prossimo evento di update
        GPIOE->ODR = (++cont) <<8;
  }
  }
}