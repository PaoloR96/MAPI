#include "header.h"


void configurazione_dispositivo (){

RCC->AHBENR |= 1<<21;
RCC->AHBENR |= 1<<17;//Serve ad abilitare la porta A

GPIOE->MODER = 0x55550000;
GPIOE->OTYPER = 0;
GPIOE->OSPEEDR = 0xFFFF0000;
GPIOE->PUPDR = 0;


}


void contatore(){
  int cont = 0;
  while (1){
  //Questo IF serve a capire se il tasto è stato premuto. Nel registro IDR il bit 0 è collegato al pin PA0 che è collegato al tasto USER  
  if(GPIOA->IDR & 0x1 == 0x1){
    while (GPIOA->IDR & 0x1 == 0x1); //Questo ciclo while serve ad aspettare che il tasto venga rilasciato per far avvenire il conteggio
    GPIOE->ODR = (++cont)<<8;
  }
}
}