#include "header.h"

void configurazione_ADC() {

  
  RCC->AHBENR |= 1<<28; //Visto che gli ADC sono collegati in coppia, abilito ADC1 e ADC2
  RCC->AHBENR |= 1<<24; //Abilito il sensore di temperatura
  
  /* Questa istruzione serve ad abilitare il sensore di temperatura e impostare il clock interno al dispositivo e sincronizzarsi con il
     bus AHB. (Manuale pag 296) serve ad abilitare anche il canale per il sensore della temperatura*/
  
  ADC1_2->CCR |= 0x00810000;
  
  /*Ora abbiamo 3 istruzioni utili per configurare l'ADC e sono necessarie e compiute prima della fase di calibrazione: queste operazioni
    prevedono prima la disabilitazione del regolatore interno di tensione, poi la sua abilitazione e successivamente aspettare un tempo pari
    nel caso peggiore a 10 micro-secondi.*/
  
  ADC1->CR &= 0xCFFFFFFF; // Pongo 00 nei bit ADVREGEN (Manuale pag 268)
  ADC1->CR |= 1<<28; // Pongo 01 nei bit ADVREGEN
  for(int i=0;i<1000;i++);// Tempo necessario per aspettare la completazione di questa fase di start-up (Potrei mettere anche 721)
  
  /*Inizia la fase di calibrazione. Tale fase serve ad ottenere quel valore necessario all'AD interno al dispositivo per correggere la 
    conversione che sta effettuando. */
  
  /*C'è un appunto da fare quando poniamo ad 1 il bit più significativo: l'ambiente di sviluppo da dopo la fase di compilazione dei warning
    dovuti al fatto che ponendo 1 nel bit più significativo potremmo modificare il segno del valore rappresentato in un determinato 
    registro. Il warning è: integer coversione resulted in a change of sign. */
  
  ADC1->CR |= 1<<31;//Ponendo ad 1 diamo inizio alla calibrazione. Al termine tale bit via HW verrà posto a 0
  while((ADC1->CR & 1<<31) == 1<<31); //Attendo che l'HW porti a 0 il bit della calibrazione (ADCAL)
  
  /*Inizia la fase durante il quale decidiamo l'ordine di conversione dei vari ingressi all'ADC. L'unico ingresso che sto prendendo in 
    considerazione è il sensore di temperatura, quindi poniamolo nel primo ingresso utile cioè il primo e sapendo che il canale del 
    sensore di temperatura è 16, pongo 16 nel primo ingresso. Successivamente decidere il clock dell'ADC per quel determinato canale. 
    Come nella fase di start-up anche qui devo aspettare per poi avviare la fase successiva. */
  
  /*Con il valore 16 indico che nel campo L pongo 0,cioè una conversione e poi seleziono il canale della temperatura (Manuale pag 282)*/
  ADC1->SQR1 |= 16<<6;
  ADC1->SMPR2 |= 7<<18; //Seleziono la frequenza sul primo canale (Manuale pag 276)
  ADC1->CR |= 1; //Pongo ad 1 il bit ADSTART per avviare la conversione dei risultati
  
  while ((ADC1->ISR & 1) == 0);//Nel registro ISR vi è il campo ADRDY che mi comunica quando è pronto l'AD alla conversione (Manuale pag 264)

  ADC1->CR |= 1<<2; //Avvio la conversione
  
  while ((ADC1->ISR & 1<<2) == 0);//Attendo la fine della conversione e devo vedere il valore del camp EOC(End of Conversion)
}


void calcolo_risultati(){
   
  calibrazione = ADC1->CALFACT; //E' il registro in cui viene salvato il valore di calibrazione (Manuale pag 290)
  tensione = ((ADC1->DR *3300)/4096);//Nel registro DR in 16 bit meno significativi hanno la tensione della conversione in mV (Manuale pag 284)
  temperatura = ((1.52 -(float)tensione/1000)/0.0046)+25;//Formula di conversione per avere la temperatura in °C;

}











