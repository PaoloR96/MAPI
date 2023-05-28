#include "header.h"


void configurazione_dispositivo(){

RCC->AHBENR |= 1<<21; //Istruzione utilizzata per abilitare la porta E del dispositivo a cui sono collegati i LED (Manuale pag 117)

//Sequenza di istruzioni utili alla configurazione della porta E
GPIOE->MODER = 0x55550000; //Serve ad impostare la modalità di I/O della porta E, in questo caso General Purpose (Manuale pag 145)
GPIOE->OTYPER = 0; //Serve ad impostare l'output della porta E, in questo caso l'ouput è PUSH-PULL ( Manuale pag 145)
GPIOE->OSPEEDR = 0xFFFF0000;//Serve ad impostare la velocità del dispositivo, in questo caso 50 Mhz (Manuale pag 146)
GPIOE->PUPDR = 0;//Serve a configurare l'I/O pull-up e pull-down, in questo caso mettiamo 0 (Manuale pag 146)

}


void contatore (){
 int cont = 0;
 while (1){
   GPIOE->ODR = cont; //Il registro ODR, in particolare i primi 8 bit di quelli disponibili sono collegati ai LED (Manuale pag 147)
   cont +=0x00000001;
   for (int i = 0;i<900;i++);//Ciclo For utile al conteggio
   if (GPIOE->ODR == 0x0000FF00)//Quando il conteggio arriva alla fine, questo IF serve per ricominciare dall'inizio
     cont = 0;
 }
}
