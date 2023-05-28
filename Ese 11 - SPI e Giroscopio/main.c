#include "header.h"

/*Esercitazione 11 - Definire la comunicazione tra SPI e il Giroscopio: vogliamo valutare la velocità dei 3 assi. Valuto in oltre anche il valore
 della temperatura del giroscopio. */

float Asse_X,Asse_Y,Asse_Z;     //Valori reali dei tre assi
uint8_t Asse_XL,Asse_XH;       // Valori ad 8 bit dell'asse X
uint8_t Asse_YL,Asse_YH;      //  Valori ad 8 bit dell'asse Y
uint8_t Asse_ZL,Asse_ZH;     //   Valori ad 8 bit dell'asse Z
uint16_t AsseX,AsseY,AsseZ; //    Valori a 16 bit per il valore totale dei tre assi
int temperatura;

uint8_t dato = 0;

int main(){
  
  configurazione_dispositivo();
  configurazione_SPI();
  configurazione_giroscopio();
  dato = WHO_AM_I();
  
  /*Per ogni coppia di valori per gli assi ricaviamo il valore di 16 bit, cioè pongo in unico registro i valori dei singoli assi. Tale operazione
    puo essere fatta come di seguito o utilizziando le seguenti due operazioni :
    
    AsseX = (valore_XH()*256);                                       
    AsseX = (valore_XL() + AsseX); 
  
  Queste due istruzioni si devono fare per tutti gli assi. E' facile vedere che il tutto è equivalente a quelle che seguono perchè moltiplicare 
   per 256 o sfasare di 8 è la stessa operazione, mentre la somma o l'OR compiono la medesima operazione.
  */
  
  
  /*Faccio un ciclo while cosi attraverso il Live Wacth posso vedere che muovendo il dispositivo i valori dei tre assi variano. */
  while(1){ 
    
  AsseX =(int16_t)((valore_XH()<<8) |valore_XL());                                       
  AsseY =(int16_t)((valore_YH()<<8) | valore_YH());
  AsseZ =(int16_t)((valore_ZH()<<8) | valore_ZH());
  
  /*Questa formula serve per ottenere il valore reale sui tre assi e i valori utilizzati dipendono fortemente dal fondo scala del giroscopio.
    Nel registro CTRL_REG4 vi sono due bit utili a definire il fondo scala che sono i bit FS1-FS0. Se non opportunamente configurati il valore  
    di default sarà 250 dps. La formula generale prevede che il valore dell'asse venga moltiplicato per un valore tipico che ha il dispositivo 
    ad un determinato fondo scala e diviso per 1000. Tale valore tipico può essere visto all'interno della tabella del manuale del giroscopio
    (3-axis digital output gyroscope) a pag 9. In questo caso il valore tipioco è 8.75  */
  
  
  Asse_X = ((float)(AsseX*0.00875));                                 
  Asse_Y = ((float)(AsseY*0.00875));    
  Asse_Z = ((float)(AsseZ*0.00875));
  
  temperatura = valore_temperatura();
  
  }  
}