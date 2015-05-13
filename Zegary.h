/*
 * Zegary.h
 *
 * Created: 2014-06-02 14:11:57
 *  Author: Wojciech
 * Literature: http://www.leon-instruments.pl/2013/04/poznaj-atxmega.html
 */ 

#ifndef ZEGARY_H_
#define ZEGARY_H_

#include <avr/io.h>
#include <util/delay.h>

void Osc32MHz(void) /* zmiana zegara na 32 MHz */;
void Osc2MHz(void) /* zmiana zegara na 2 MHz */;
void OscXtal(void);
void OscPLL(uint8_t pllfactor);

#endif /* ZEGARY_H_ */