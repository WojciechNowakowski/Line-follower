/*
 * pilot.c
 *
 * Created: 2014-06-02 00:40:05
 *  Author: Wojciech
 */ 

#include "pilot.h"

volatile uint8_t odebrano_kod_pilota, kod_odczytany = 1, licz_ir, kod_ir, adres_ir, tab[27], buf_ir;
volatile uint8_t tryb;

ISR(TCC0_CCA_vect)  // przerwanie przechwycenia A timera C0 - pilot IR
{
	unsigned char czas;		// zmienna pomocnicza
	if (TCC0.CNT >= 3000) // gdy sygna³ jest nieprawid³owy
	{
		licz_ir = 0;				// wykasuj licznik tablicy
	}
	TCC0.CNT = 0;					// wyzeruj timer
	czas = TCC0.CCAL;			// zapamiêtaj mniej znacz¹cy bajt rejestru CCA
	if(licz_ir <= 26)			// zapamiêtuj znacz¹ce wartoœci
	tab[licz_ir] = czas;
	licz_ir++;						// zwiêksz licznik
	if(licz_ir > 32)			// gdy sygna³ siê zakoñczy³
	{
		odebrano_kod_pilota = 1;	// ustaw flagê informuj¹c¹ o odebraniu sygna³u
		kod_ir = 0;								// skasuj poprzedni kod
		adres_ir = 0;							// i adres
		licz_ir = 0;							// oraz licznik
	}
}

void IR_remote_init(void)	// konfiguracja input capture do pilota IR
{
	PORTC.DIRCLR = PIN4_bm;																	// Pin PC4 jako wejœcie
	PORTC.PIN4CTRL = PORT_ISC_FALLING_gc;										// opadaj¹ce zbocze sygna³u na pinie PC4 wywo³a zdarzenie
	EVSYS.CH4MUX = EVSYS_CHMUX_PORTC_PIN4_gc;								// zdarzenia na pinie PC4 -> kana³ o numerze 4 w systemie zdarzeñ
	TCC0.CTRLD = TC_EVACT_CAPT_gc | TC_EVSEL_CH4_gc;				// wybór kana³u zdarzeñ o numerze 4, dla input capture
	TCC0.INTCTRLB = TC_CCAINTLVL_LO_gc;											// niski poziom przerwania dla input capture A
	TCC0.CTRLB = TC_WGMODE_NORMAL_gc | TC0_CCAEN_bm;				// tryb normalny z input capture A
	TCC0.CTRLA = TC_CLKSEL_DIV1024_gc;											// uruchomienie timera z preskalerem 1024
}

void sprawdz_sygnal_z_pilota(void)	// sprawdzenie odbioru sygna³u i ustalenie jego kodu
{
	if(odebrano_kod_pilota)							// jeœli odczytano sygna³ to ustal jego kod
	{
		odebrano_kod_pilota = 0;						// zgaœ flagê
		for(int i=0;i<8;i++)
		{
			buf_ir = (tab[26-i] > 54)? 1 : 0;	// sprawdŸ d³ugoœæ impulsów
			kod_ir |= buf_ir << i;						// i ustal na ich podstawie kod
			buf_ir = (tab[10-i] > 54)? 1 : 0;
			adres_ir |= buf_ir << i;					// i adres wysy³ane przez pilot
		}
		kod_odczytany = 0;									// zgaœ flagê informuj¹c¹ o odczytaniu kodu
	}
}

void tryb_oczekiwanie_na_sygnal(void)	// oczekiwanie na sygna³ z pilota
{
	LED4_OFF
	LED5_OFF
	LED6_OFF
	kod_ir = 0;
	while ((kod_ir != ok)&&(kod_ir != stop)&&(kod_ir != source))	// dopóki otrzymywany sygna³ nie jest prawid³owy wykonuj:
	{
		sprawdz_sygnal_z_pilota();	// sprawdzaj czy odebrano sygna³ i wyznacz jego kod
		_delay_ms(50);							// odczekaj chwilê
		wdt_reset();								// zresetuj watchdoga
	}
	switch (kod_ir)								// w zale¿noœci od kodu sygna³u z pilota
	{
		case ok:										// ustaw odpowiedni tryb pracy robota
			tryb = testy_silnika;
		break;
		case stop:
			tryb = sledzenie_linii;
		break;
		case source:
			tryb = ust_wsp_reg_PID;
		break;
	}
	kod_odczytany = 1;
}
