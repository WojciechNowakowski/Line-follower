/*
 * czujniki.h
 *
 * Created: 2014-06-02 02:23:17
 *  Author: Wojciech
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "init.h"
#include "kalibracja.h"

#ifndef CZUJNIKI_H_
#define CZUJNIKI_H_

#define liczba_czujnikow				11


int16_t sprawdz_stan_czujnikow_i_zasilanie(void) /* odczytuje wskazania czujników i zwraca po³o¿enie linii */;
void adc_init(void) /* konfiguracja ADC */;


#endif /* CZUJNIKI_H_ */