/*
 * init.c
 *
 * Created: 2014-06-01 23:37:07
 *  Author: Wojciech
 */ 

#include "init.h"

void led_init(void)	// konfiguracja i mryganie diodami
{
	LED_DDR_init							// ustawienie odpowiednich pinów jako wyjœcia
	LEDS_ON										// zamruganie wszystkimi diodami
	_delay_ms(300);
	LEDS_OFF
	_delay_ms(300);
	LED1_ON	_delay_ms(100);		// liniowe zaœwiecenie diod
	LED2_ON	_delay_ms(100);
	LED3_ON	_delay_ms(100);
	LED4_ON	_delay_ms(100);
	LED5_ON	_delay_ms(100);
	LED6_ON	_delay_ms(100);
	LED7_ON	_delay_ms(100);
	LED6_OFF	_delay_ms(100);	// i zgaszenie ich oprócz led7
	LED5_OFF	_delay_ms(100);
	LED4_OFF	_delay_ms(100);
	LED3_OFF	_delay_ms(100);
	LED2_OFF	_delay_ms(100);
	LED1_OFF	_delay_ms(100);
}

void main_timer_init(void)	// konfiguracja timera g³ównego 10ms
{
	TCE0.CTRLA = TC_CLKSEL_DIV256_gc;				// preskaler 256
	TCE0.PER = 1250;												// maksymalna wartoœæ
	TCE0.INTCTRLA = TC_OVFINTLVL_HI_gc;			// wysoki poziom przerwania od przepe³nienia
}
