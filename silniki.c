/*
 * silniki.c
 *
 * Created: 2014-06-01 23:53:34
 *  Author: Wojciech
 */ 
 
#include "silniki.h"
#include "init.h"
#include <avr/io.h>

void silniki_init(void)	// konfiguracja timera do PWM 8 kHz
{
	PORTD.DIRSET = 0x0F;										// PD0-3 jako wyj�cia
	TCD0.CTRLB = TC_WGMODE_SINGLESLOPE_gc |	// tryb PWM - jedno zbocze
	TC0_CCAEN_bm |													// uaktywnienie kana��w: A,
	TC0_CCBEN_bm |													// B,
	TC0_CCCEN_bm |													// C i
	TC0_CCDEN_bm;														// D
	TCD0.PER = max_pwm;											// maksymalna warto��
	TCD0.CCA = 0;														// ustawienie warto�ci pocz�tkowych
	TCD0.CCB = 0;
	TCD0.CCC = 0;
	TCD0.CCD = 0;
	TCD0.CTRLA = TC_CLKSEL_DIV1_gc;					// ustawienie preskalera i uruchomienie timera
}
 
void PRAWY_przod(uint16_t wypelnienie)		// prawy silnik do przodu
{
	S1_2off
	S1_1_ch = wypelnienie;
}

void PRAWY_tyl(uint16_t wypelnienie)			// prawy silnik do ty�u
{
	S1_1off
	S1_2_ch = wypelnienie;
}

void LEWY_przod(uint16_t wypelnienie)			// lewy silnik do przodu
{
	S2_2off
	S2_1_ch = wypelnienie;
}

void LEWY_tyl(uint16_t wypelnienie)				// lewy silnik do ty�u
{
	S2_1off
	S2_2_ch = wypelnienie;
}
 
void ustawienie_silnikow(int16_t sygnal2)		// ustaw kierunek i pr�dko�� silnik�w zgodnie z sygna�em steruj�cym
{
	if(sygnal2 > max_pwm)		// ograniczenie sygna�u id�cego na silniki
		sygnal2 = max_pwm;		// odg�rne
	if(sygnal2 < -max_pwm)
		sygnal2 = -max_pwm;		// i oddolne
	if(sygnal2 == 0)				// jazda prosto
	{
		LEWY_przod(max_pwm);	// oba silniki na full speed
		PRAWY_przod(max_pwm);
	}
	if(sygnal2 > 0)					// jazda w prawo
	{
		LEWY_przod(max_pwm);
		PRAWY_przod(max_pwm - sygnal2);	// prawy silnik zwalnia
	}
	if(sygnal2 < 0)					// jazda w lewo
	{
		PRAWY_przod(max_pwm);
		LEWY_przod(max_pwm + sygnal2);	// lewy silnik zwalnia
	}
}