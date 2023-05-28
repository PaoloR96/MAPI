#include "header.h"


/* Questa procedura può essere trascurata perchè tale esercitazione non prevede l'uso dei LED,quindi non devo abilitare i LED,ma può
   essere una variante e la porta A non va abilitata perchè la pressione è interrupt esterna

void configurazione_dispositivo(){

  RCC->AHBENR |= 1<<21;
  RCC->AHBENR |= 1<<17;

  GPIOE->MODER = 0x55550000;
  GPIOE->OTYPER = 0;
  GPIOE->OSPEEDR = 0xFFFF0000;
  GPIOE->PUPDR = 0;
  
}*/

void configurazione_timer (){

   RCC->APB1ENR |= 1<<4;
   
   NVIC->ISER[1] |= 1<<22;
   
   TIM6->CR1 = 0;
   TIM6->CR2 = 0;
   TIM6->DIER = 1;
   TIM6->PSC = 7199;
   TIM6->ARR = 10000;

}

void configurazione_tastoUSER(){

  NVIC->ISER[0] |= 1<<6;
  
  EXTI->IMR = 1;
  EXTI->RTSR |= 1; 

}
