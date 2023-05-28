#include "header.h"


void configurazione_dispositivo(){
  
  /*Quello che devo abilitare sul bus AHB sono l'ADC1_2 e la porta A che in questa Esercitazione svolgerà il ruolo di output */
  
  RCC->AHBENR |= 1<<28; //Abilito ADC1_2
  RCC->AHBENR |= 1<<17; //Abilito porta A
  
  /* Come avveniva per la porta E collegata ai LED devo impostare i medesimi registri per porta A per fornire l'output */
  
  /* Con questa operazione sto dicendo che nel registro MODER nel campo MODER4 pongo il valore 11, perchè l'uscita dovrà essere di tipo
     analogico. Sappiamo che PA4 è l'uscita del nostro DAC sul canale 1 */
  GPIOA->MODER |= 3<<8;
  GPIOA->OSPEEDR |= 0xFFFF0000;
  
  /*Nelle precedenti Esercitazioni vi erano anche altre due operazioni da compiere in cui le impostavo a 0, ma vedendo nel Manuale queste
    all'accensione del dispositivo sono gia settate a 0 (Vedere il campo RESET) */
  
}


/*La seguente procedura contiene una singola istruzione, ma la suddivisione chiarisce meglio i passaggi operativi da compiere */
void configurazione_DAC(){

  RCC->APB1ENR |= 1<<29; //Abilito il DAC
  DAC->CR |= 1;//Serve ad abilitare il DA, cioè pongo ad 1 il bit EN1 il quale abilita il DAC sul canale 1 (Manuale pag 316)

}

void configurazione_ADC(){
  
  NVIC->ISER[0] |= 1<<18;
  
  ADC1->IER |= 1<<2; //Abilito interrupt ADC
  ADC1_2->CCR |= 1<<16;
  ADC1->CR &= 0xCFFFFFFF;
  ADC1->CR |=1<<28;
  for (int i = 0;i<1000;i++);
  ADC1->CR |= 1<<31; //Avvio calibrazione
  while((ADC1->CR & 1<<31)== 1<<31); //Attendo fine calibrazione
  
  /*Ora dobbiamo selezionare il canale per la conversione. Bisogna fare alcuni appunti. Inanzitutto bisogna analizzare a pag 36 del 
    Manuale 23353_STM32F30yxx i piedini del microcontrollore che funzioni di input e/o output possono avere. Vediamo che il pin PA4 
    impostato in precendenza funge da DAC1_OUT1 cioè uscita del DAC sul canale 1. Ora bisogna scegliere l'ingresso da porre che fisicamente
    deve essere vicino al pin PA4, scegliamo PA2 che sarà l'ADC1_IN3,cioè valore di ingresso sul canale 3. 
    Di conseguenza devo mettere il canale 3 nella prima sequenza del registro SQ1.*/
  
  ADC1->SQR1 |= 3<<6;
  ADC1->SMPR1 |= 7<<9; //Visto che sto considerando il canale 3, devo impostare la sua frequenza (Manuale pag 275)
  ADC1->CR |=1;
  while((ADC1->ISR & 1) == 0);
 
}

/* Il timer 6 è il timer che regolerà il DAC. Quando avvieremo i timer però vogliamo che il timer 6 parta in ritardo rispetto a timer 7
   che regola l'ADC*/

void configurazione_timer6(){
  
  RCC->APB1ENR |= 1<<4;
  
  NVIC->ISER[1] |= 1<<22;
  //TIM6->CR1 = 0;
  //TIM6->CR2 = 0;
  TIM6->DIER = 1;
  TIM6->PSC = 71;
  TIM6->ARR = 100;
  TIM6->CR1 |=1;
  
}


void configurazione_timer7(){
  
  RCC->APB1ENR |= 1<<5;
  
  NVIC->ISER[1] |= 1<<23;
  //TIM7->CR1 = 0;
  //TIM7->CR2 = 0;
  TIM7->DIER = 1;
  TIM7->PSC = 71;
  TIM7->ARR = 100;
  TIM7->CR1 |= 1;
  
}