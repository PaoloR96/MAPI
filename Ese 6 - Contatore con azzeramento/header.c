#include "header.h"


void configurazione_dispositivo(){

  RCC->AHBENR |= 1<<21;
  //RCC->AHBENR |= 1<<17; Possiamo anche non attivare la porta A visto che stiamo utilizzando un interrupt esterno 
  
  GPIOE->MODER = 0x55550000;
  GPIOE->OTYPER = 0;
  GPIOE->OSPEEDR = 0xFFFF0000;
  GPIOE->PUPDR = 0;

}

void configurazione_timer(){
  
  RCC->APB1ENR |= 1<<4;
  
  NVIC->ISER[1] |= 1<<22;
  
  TIM6->CR1 = 0;
  TIM6->CR2 = 0;
  TIM6->DIER = 1;
  TIM6->PSC = 7199;
  TIM6->ARR = 10000;
  TIM6->CR1 = 1;
  
}

void configurazione_tastoUSER(){
  
  /* Ricordiamo che il tasto USER se premuto scatena un interrupt esterno, dove collegato alla porta PA0. I pin 0 di tutte le porte sono
     collegati all'interrupt esterno EXTI0 e ponendo quel valore in ISER[0] abilito tale interrupt. */
  
  NVIC->ISER[0] = 1<<6; //Mi abilita l'interrupt esterno in particolare quello legato ai pin 0 delle porte
  EXTI->IMR = 1; //Serve ad abilitare l'interrupt (Manuale pag 192)
  EXTI->RTSR |= 1;//Abilita l'interrupt sul fronte di salita, cioè quando premiamo il tasto (Manuale pag 193)
}
  