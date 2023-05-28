#include "header.h"

void configurazione_timer(){
  
  RCC->APB1ENR |=1<<4;
  NVIC->ISER[1] |= 1<<22; 
  
  TIM6->CR1 = 0;
  TIM6->CR2 = 0;
  TIM6->DIER = 1;
  TIM6->PSC = 7199;
  TIM6->ARR = 10000;
  TIM6->CR1 |=1;
  
}

void configurazione_ADC (){

  RCC->AHBENR |= 1<<28;
  RCC->AHBENR |= 1<<24;
  
  ADC1_2->CCR = 0x00810000;
  
  ADC1->CR &= 0xCFFFFFFF;
  ADC1->CR |= 1<<28;
  for(int i=0;i<1000;i++);
  
  ADC1->CR |= 1<<31;
  while((ADC1->CR & 1<<31)== 1<<31);
  
  NVIC->ISER[0] |= 1<<18; //Abilito l'interrupt per 'ADC1_2 
  
  ADC1->SQR1 |= 16<<6;
  ADC1->SMPR2 |= 7<<18;
  
  ADC1->IER |= 1<<2; //Serve ad abilitare l'interrupt per l'ADC ad ogni fine conversione (Manuale pag 267)
  
  ADC1->CR |= 1;
  
  while((ADC1->ISR & 1 ) == 0);
    
}
