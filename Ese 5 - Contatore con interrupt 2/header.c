#include "header.h"


void configurazione_dispositivo(){
  
  RCC->AHBENR |= 1<<21;
  RCC->APB1ENR |= 1<<4;
  
  GPIOE->MODER = 0x55550000;
  GPIOE->OTYPER = 0;
  GPIOE->OSPEEDR = 0xFFFF0000;
  GPIOE->PUPDR = 0;
  
}

void configurazione_timer (){
  
  /* Utilizziamo questa istruzione per abiliatare l'interrupt corrispondente al TIMER 6. In particolare nella tabella delle interrutpt,
     appunto NVIC vi sono 81 interrupt possibili, che possono essere gestite da 3 registri a 32 bit l'uno che sono ISER[0],ISER[1] e 
     ISER[2]. Sapendo che ogni registro ha 32 bit,e vedendo che l'interrupt corrispondente a TIMER 6 si trova nella posizione 54, è 
     facile capire che devo utilizzare ISER[1] che si occupa delle interrupt dalla 32 alla 63 (ISER[0] si occupa dalla interrapt 0 a 31,
     mentre ISER[2] le rimanenti). Poniamo 22 perchè la posizone 0 del registro ISER[1] è 32, di conseguenza per sapere che posizione
     occupa l'interrupt di TIMER 6 devo fare 54-32 = 22. (Vale in tutti i casi)

  */

  NVIC->ISER[1] = 1<<22;
  
  TIM6->CR1 = 0;
  TIM6->CR2 = 0;
  TIM6->DIER = 1;
  TIM6->PSC = 7199;
  /* Vi è un problema sul valore dell'ARR: in una esercitazione precedente per avere un conteggio ogni secondo ho dovuto mettere 1000, qui
     invece con le interrupt ho dovuto mettere 10000. */
  TIM6->ARR = 10000;
  TIM6->CR1 = 1;
  
   
}