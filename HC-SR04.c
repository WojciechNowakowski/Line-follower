#include <avr/io.h>
#include <util/delay.h>
#include "HC-SR04.h"

volatile uint16_t	HC_SR04_dystans;
volatile uint8_t		HC_SR04_flaga;

ISR(HC_SR04_timer_int_vect)	// przerwanie przechwycenia A timera - czujnik HC-SR04
{
	HC_SR04_dystans = HC_SR04_timer.CCA / 3;	// dystans w mm, czas w [2us]
	HC_SR04_flaga = 1;												// ustaw flagê informuj¹c¹ o zmierzeniu dystansu
}

void HC_SR04_init(void)		// konfiguracja czujnika HC-SR04
{
	HC_SR04_trig_PORT.DIRSET = HC_SR04_trig_Pin;													// PC4 (Trigger) jako wyjœcie
	HC_SR04_echo_PORT.DIRCLR = HC_SR04_echo_Pin;													// Pin PC5 jako wejœcie
	HC_SR04_echo_PORT.HC_SR04_echo_PinCTRL = PORT_ISC_BOTHEDGES_gc;				// oba zbocza sygna³u na pinie PC5 wywo³aj¹ zdarzenie
	EVSYS.HC_SR04_echo_Event_CH = HC_SR04_echo_Event_src;									// zdarzenia na pinie PC5 -> kana³ o numerze 5 w systemie zdarzeñ
	HC_SR04_timer.CTRLD = TC_EVACT_PW_gc | HC_SR04_timer_event;						// wybór kana³u zdarzeñ o numerze 4, dla pulse width capture
	HC_SR04_timer.INTCTRLB = HC_SR04_timer_int_lvl;												// œredni poziom przerwania dla input capture A
	HC_SR04_timer.CTRLB = TC_WGMODE_NORMAL_gc | HC_SR04_timer_cmp_ch_bm;	// tryb normalny z input capture A
	HC_SR04_timer.CTRLA = HC_SR04_timer_prescaler;												// uruchomienie timera z preskalerem 64
}

void HC_SR04_trigger(void)	// wyzwolenie pomiaru przez czujnik HC-SR04
{
	HC_SR04_trig_PORT.OUTSET = HC_SR04_trig_Pin;	// ustaw pin trig w stan wysoki
	_delay_us(11);																// czekaj 11 us
	HC_SR04_trig_PORT.OUTCLR = HC_SR04_trig_Pin;	// ustaw pin trig w stan niski
}