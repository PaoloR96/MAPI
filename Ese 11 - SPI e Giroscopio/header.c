#include "header.h"


void configurazione_dispositivo(){
  
  /*Con le seguenti operazioni abilito il clock di porta A e porta E visto che il giroscopio è collegato ad alcuni pin di queste due porte */
  
  RCC->AHBENR |= 1<<17; 
  RCC->AHBENR |= 1<<21;
  
  
  /*Vado ad impostare i pin di porta A PA5, PA6 e PA7 perchè sono i piedini collegati al Send Clock, al MOSI e MISO del giroscopio */
  
  GPIOA->MODER |= 1<<11; //Imposto la porta PA5 in Alternate Function e successivamente specifico come deve operare
  GPIOA->MODER |= 1<<13; //Imposto la porta PA6 in Alternate Function e successivamente specifico come deve operare
  GPIOA->MODER |= 1<<15; //Imposto la porta PA7 in Alternate Function e successivamente spesifico come deve operare
  
  /* Con la seguente istruzioni andiamo a decidere come devono operare i pin appena settati. Per vedere come devono operare basta visualizzare 
     il DATASHEET del dispositivo a pagina 42. Dopo aver visto dalla tabella quale funzione devono avere i tre pin, per impostarli utilizzo
     il registro AFRL dove pongo dove aver visto il Datasheet a 5 per i tre pin. In realtà non utilizziamo direttamente tale registro ma la 
     sua definizione all'interno della libreria stm32f30x.h dove viene definito nel seguente modo : __IO uint32_t AFR[2]. Tale registro è a 
     due puntatori uno per il registro AFRL e uno per AFRH. */
  
  GPIOA->AFR[0] |= 0x55500000; //Imposto AF5 nei tre pin (Manuale pag 150 e Datasheet pag 42)
  
  GPIOA->OSPEEDR |= 0x00005400; //Imposto la velocità delle porte visto che al massimo il giroscopio funziona a 10 MHz
  
  GPIOE->MODER |= 1<<6; //Della porta E imposto il pin PE3 che è il chip-select come uscita digitale (General Purpose)
  GPIOE->OSPEEDR |= 0x55555555;//Imposto la stessa velocità anche per porta E
  
    
}

void configurazione_SPI(){
  
  /*Ora devo configurare l'interfaccia SPI per la comunicazione e sono previsti 13 punti, però non tutti necessari ed è possibile vederli sul
    Manuale pag 730 */
 
  RCC->APB2ENR |=1<<12; //Serve ad abilitare il clock del SPI (Manuale pag 119);
  
  RCC->APB2RSTR |= 1<<12; //Istruzione fatta per essere certi che SPI sia resettato all'inizio del programma da eseguire (Manuale pag 113)
  RCC->APB2RSTR &= (~ (1<<12)); //Variante dell'AND bit a bit e serve a riabilitare SPI
  
  /*Inizio la vera e propria configurazione del SPI */
  
  /*Nel registro CR1 vi sono molte informazioni utili che in questo caso non vanno impostate perchè il valore di reset è già 0, infatti: 
  il bit BIDIMODE deve essere impostato a 0 per avere una comunicazione a due linee unidirezionale, inoltre viene impostato la polarità
  del clock avendo come valore di reset 0 per CPOL e CPHA, cosi definito per poter comunicare con il giroscopio. */
  
  SPI1->CR1 |=1<<4;//Con tale istruzione imposto il BAUND RATE che al massimo per la comunicazione con il giroscopio è 10 MHz (Manuale pag 759)
  
  
  SPI1->CR1 |=1<<9; //Imposto la SW slave management, cioè il cambiamento di slave avviene per via SW
  SPI1->CR1 |=1<<8; //Serve a collegare NSS al valore logico alto del Master
  SPI1->CR1 |=1<<2; //Do informazioni sul fatto che il dispositivo è un Master
  SPI1->CR2 |=1<<12; //Serve per selezionare la lunghezza della FIFO,in questo caso seleziono la trasmissione ad 8 bit
  SPI1->CR2 |=7<<8; //Imposto la lunghezza del messaggio che devo inviare nel campo DS (Manuale pag 760)

  /*L'istruzione successiva serve ad assicurarsi che l'interfaccia I2C non sia attiva. Tale istruzioni risulta superflua laddove resettiamo il 
    dispositivo,ma utile laddove faccio operazioni più complesse. */
  
  SPI1->I2SCFGR &= ~(1<<11);//(Manuale pag 766)
  
  SPI1->CR1 |=1<<6; //Serve ad abilitare la SPI
 
  
}

/* Sappiamo che nel registro DR è il registro comune per la lettura e la struttura , in pratica è il nostro shif-register. La comunicazione va  
   su 8 bit,ma in questo registro a 16 bit non è possibile scrivere 8 bit come visto nelle precedenti Esercitazioni, perciò viene implementata  
   la seguente funzione che mi serve a calcolare l'indirizzo del registro ed utilizzando un puntatore a puntatore posso mettere il dato nel     
   registro. */

uint8_t inviaSPI(uint8_t dato){ //uint8_t significa che sto utilizzando un tipo intero ad 8 bit senza segno

  uint32_t spi_base = 0x00; //Dichiaro il byte dummy in cui mettero l'indirizzo del registro
  spi_base = (uint32_t)SPI1;//Faccio un casting sulla struttura SPI per predere l'indirizzo base SPI
  spi_base += 0x0C; //Sommo il valore di OFFSET per avere l'indirizzo del registro DR dove effettivamente si trova il dato (Manuale pag 764)
  
  /* Il seguente ciclo WHILE serve per effettuare il polling sul bit TXE del registro SR (Manuale pag 762). Tale bit mi dice se il buffer è
     occupato( trasmissione in corso) oppure è libero. Attendo fintanto che il buffer non si libera. */
  while ((SPI1->SR & 2<<0) != 2<<0);
  
  /** La seguente istruzione permette attraverso l'utilizzo di un puntatore a puntatore di mettere il valore ad 8 bit nel registro DR*/
  *(__IO uint8_t *)spi_base = dato;  
  
  /* Con questo ciclo WHILE attendo la fine della trasmissione, in particolare verifico il valore del bit RXNE*/
  while((SPI1->SR & 1<<0) != 1<<0);
  
  return *(__IO uint8_t *)spi_base;
  
}
/*Andando a vedere i vari registri di configurazione utili a definire le proprietà del giroscopio, vediamo che di default i bit hanno valori tali
  da avere già la configurazione desiderata in questa Esercitazione, quindi se non fatta questa procedura non cambia niente ai fini delle 
  operazioni. */
void configurazione_giroscopio(){

  uint8_t dato;
  GPIOE->ODR &= (~(1<<3));
  
  dato = inviaSPI(0x20); //Tale istruzione serve a configurare il registro CTRL_REG1 in cui comunico che voglio la scrittura singola
  dato = inviaSPI(0xFF); //In questi istruzioni i primi 4 bit definiscono la velocità ed inoltre abilito i tre assi
  
  GPIOE->ODR |= 1<<3;

}

/* La seguente procedura serve sostanzialmente a capire se effettivamente la comunicazione sta andando a buon fine, in particolare vedere se il 
   dispositivo risponde. Viene interrogato un registro di indirizzo predefinito 0F che risponderà con il valore D4 nel registro DR.
   Bisogna fare attenzione però perchè affinchè venga generato il clok dal Master bisogna trasmettere il dummy byte.*/
uint8_t WHO_AM_I (){
  
  uint8_t dato;
  
  GPIOE->ODR &= (~(1<<3)); //Istruzione che serve ad abbassare il chip-select collegato al pin PE3
  /*Voglio leggere il contenuto del registro WHO AM I che si trova all'indirizzo 0F, ma devo mettere come bit più significato 1 per indicare che
    devo leggere dal registro */
  dato = inviaSPI(0x8F);
  dato = inviaSPI(0x00); //Invio a vuoto del dummy byte per far si che il Master generi il clock
  GPIOE->ODR |= 1<<3; //Abilito il chip-select
  
  return dato;
  
} 
  
/* Le seguenti 6 procedure servono a prelevare i valori legati ai tre assi. La procedura è la medesima di quella fatta per WHO_AM_I, l'unica 
   differenza risale all'indirizzo e se si vuole scrivere o leggere il contenuto del registro. Inanziutto il valore che avrei per gli assi viene
   diviso in due registri da 8 bit, visto che 16 non possono essere gestiti e per ogni asse abbiamo LOW e HIGH. L'indirizzo di ogni registro da
   prendere si trova a pagina 29 del manuale del giroscopio (3-axis digital output gyroscope).*/

uint8_t valore_XL(){

  uint8_t dato;
  GPIOE->ODR &= (~(1<<3));
  
  dato = inviaSPI(0xA8);
  dato = inviaSPI(0x00);
  
  GPIOE->ODR |= 1<<3;
  
  return dato;
  
}


uint8_t valore_XH(){

  uint8_t dato;
  GPIOE->ODR &= (~(1<<3));
  
  dato = inviaSPI(0xA9);
  dato = inviaSPI(0x00);
  
  GPIOE->ODR |= 1<<3;
  
  return dato;
  
}


uint8_t valore_YL(){

  uint8_t dato;
  GPIOE->ODR &= (~(1<<3));
  
  dato = inviaSPI(0xAA);
  dato = inviaSPI(0x00);
  
  GPIOE->ODR |= 1<<3;
  
  return dato;
  
}

uint8_t valore_YH(){

  uint8_t dato;
  GPIOE->ODR &= (~(1<<3));
  
  dato = inviaSPI(0xAB);
  dato = inviaSPI(0x00);
  
  GPIOE->ODR |= 1<<3;
  
  return dato;
  
}

uint8_t valore_ZL(){

  uint8_t dato;
  GPIOE->ODR &= (~(1<<3));
  
  dato = inviaSPI(0xAC);
  dato = inviaSPI(0x00);
  
  GPIOE->ODR |= 1<<3;
  
  return dato;
  
}

uint8_t valore_ZH(){

  uint8_t dato;
  GPIOE->ODR &= (~(1<<3));
  
  dato = inviaSPI(0xAD);
  dato = inviaSPI(0x00);
  
  GPIOE->ODR |= 1<<3;
  
  return dato;
  
}

//Tale procedura serve a prelevare la temperatura avuta dal giroscopio
uint8_t valore_temperatura(){
  
  uint8_t dato;
  GPIOE->ODR &= (~(1<<3));
  
  dato = inviaSPI(0xA6);
  dato = inviaSPI(0x00);
  
  GPIOE->ODR |= 1<<3;
  
  return dato;
}