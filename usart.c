/*
 * usart.c
 *
 * Created: 2014-06-02 00:38:02
 *  Author: Wojciech
 */ 

#include "usart.h"

volatile uint8_t usart_flaga, usart_indeks, usart_ile;
char		 usart_bufor[32];
volatile int8_t odb_byte;

ISR(USARTC0_RXC_vect)	// przerwanie przy odbiorze danych przez usart
{
	usart_flaga = 1;						// ustaw odpowiedni¹ flagê
	odb_byte = USARTC0_DATA;	// przepisz odebrany bajt/znak
}

ISR(USARTC0_DRE_vect)	// przerwanie pustego buforu nadawania
{
	USARTC0.DATA = usart_bufor[usart_indeks];		// przepisz znak do odpowieniego rejestru
	usart_indeks++;															// ustaw indeks na nastêpny znak
	if (usart_indeks == usart_ile)							// gdy wys³ano odpowiedni¹ iloœæ znaków
	{
		USARTC0.CTRLA &= ~USART_DREINTLVL_gm;			// wy³¹cz przerwanie pustego buforu nadawania
	}
}

void wyslij_usart(unsigned char ilosc_znakow)	// wysy³a wskazan¹ iloœæ znaków z buforu przez usart
{
	usart_indeks = 0;												// zacznij od znaku z indeksem 0
	usart_ile = ilosc_znakow;
	USARTC0.CTRLA |= USART_DREINTLVL_LO_gc;	// odblokuj przerwanie pustego bufora nadawania
}
 
void usart_init(void)	// konfiguracja USART
{
	PORTC.DIRSET = PIN3_bm;													// PC3 (TX) jako wyjœcie
	USARTC0.BAUDCTRLA = 7;													// prescaler na 250 kbps dla 32 MHz
	USARTC0.CTRLA = USART_RXCINTLVL_LO_gc;					// niski poziom przerwania odbiornika
	USARTC0.CTRLB = USART_TXEN_bm | USART_RXEN_bm;	// w³¹czenie nadajnika i odbiornika
}
