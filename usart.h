/*
 * usart.h
 *
 * Created: 2014-06-02 00:30:28
 *  Author: Wojciech
 */ 

#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void wyslij_usart(unsigned char ilosc_znakow) /* wysy³a wskazan¹ iloœæ znaków z buforu przez usart */;
void usart_init(void) /* konfiguracja USART */;

#endif /* USART_H_ */