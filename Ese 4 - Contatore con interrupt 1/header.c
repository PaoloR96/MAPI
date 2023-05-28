#include "header.h"



void configurazione_dispositivo(){
  
  RCC->AHBENR |= 1<<21;
  
  GPIOE->MODER = 0x55550000;
  GPIOE->OTYPER = 0;
  GPIOE->OSPEEDR = 0xFFFF0000;
  GPIOE->PUPDR = 0;
  
}

void contatore (){
  
  /*Utilizzando l'interrupt di alto livello non ho bisogno di utilizzare alcun timer del dispositivo, ma viene prelevata direttamente 
  la frequenza del cortex. Necessarie al riconoscimento degli elementi presenti nell'IF sono i file: startup_stm32f30x.s,
  system_stm32f30x.c e system_stm32f30x.h .
  */
  
  if (SysTick_Config(SystemCoreClock/1000)){
      while (1);
  }
  RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
  GPIOE->MODER = 0x55550000;
  while(1);
  
 /* N.B. Le istruzioni sopra riportate non sono molto chiare. La condizione nell'if se verificate fa scattare l'interrupt, ma andando a 
    verificare andando a fare il debug, questa non si verifichererà mai. Inoltre le tre istruzioni successive ( molto simili alla 
    configurazione del dispositivo non sono molto chiare).
    Altro appunto vede di eliminare le ultime tre istruzioni e mettere un NOT nella condizione dell'IF e l'interrupt avviene correttemente.
  */ 
  
}