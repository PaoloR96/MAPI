#ifndef HEADER_H_
#define HEADER_H_

#include "stm32f30x.h"



void configurazione_dispositivo();
void configurazione_SPI();
void configurazione_giroscopio();
uint8_t inviaSPI(uint8_t);
uint8_t WHO_AM_I();
uint8_t valore_XL();
uint8_t valore_XH();
uint8_t valore_YL();
uint8_t valore_YH();
uint8_t valore_ZL();
uint8_t valore_ZH();
uint8_t valore_temperatura();







#endif