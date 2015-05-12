/*
 * Zegary.h
 *
 * Created: 2014-06-02 14:11:57
 *  Author: Wojciech
 */ 

#include <avr/io.h>
#include <util/delay.h>

#ifndef ZEGARY_H_
#define ZEGARY_H_

void Osc32MHz(void) /* zmiana zegara na 32 MHz */;
void Osc2MHz(void) /* zmiana zegara na 2 MHz */;
void OscXtal(void);
void OscPLL(uint8_t pllfactor);

#endif /* ZEGARY_H_ */