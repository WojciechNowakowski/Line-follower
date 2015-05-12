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
	if (TCC0.CNT >= 3000) // gdy sygna� jest nieprawid�owy
	{
		licz_ir = 0;				// wykasuj licznik tablicy
	}
	TCC0.CNT = 0;					// wyzeruj timer
	czas = TCC0.CCAL;			// zapami�taj mniej znacz�cy bajt rejestru CCA
	if(licz_ir <= 26)			// zapami�tuj znacz�ce warto�ci
	tab[licz_ir] = czas;
	licz_ir++;						// zwi�ksz licznik
	if(licz_ir > 32)			// gdy sygna� si� zako�czy�
	{
		odebrano_kod_pilota = 1;	// ustaw flag� informuj�c� o odebraniu sygna�u
		kod_ir = 0;								// skasuj poprzedni kod
		adres_ir = 0;							// i adres
		licz_ir = 0;							// oraz licznik
	}
}

void IR_remote_init(void)	// konfiguracja input capture do pilota IR
{
	PORTC.DIRCLR = PIN4_bm;																	// Pin PC4 jako wej�cie
	PORTC.PIN4CTRL = PORT_ISC_FALLING_gc;										// opadaj�ce zbocze sygna�u na pinie PC4 wywo�a zdarzenie
	EVSYS.CH4MUX = EVSYS_CHMUX_PORTC_PIN4_gc;								// zdarzenia na pinie PC4 -> kana� o numerze 4 w systemie zdarze�
	TCC0.CTRLD = TC_EVACT_CAPT_gc | TC_EVSEL_CH4_gc;				// wyb�r kana�u zdarze� o numerze 4, dla input capture
	TCC0.INTCTRLB = TC_CCAINTLVL_LO_gc;											// niski poziom przerwania dla input capture A
	TCC0.CTRLB = TC_WGMODE_NORMAL_gc | TC0_CCAEN_bm;				// tryb normalny z input capture A
	TCC0.CTRLA = TC_CLKSEL_DIV1024_gc;											// uruchomienie timera z preskalerem 1024
}

void sprawdz_sygnal_z_pilota(void)	// sprawdzenie odbioru sygna�u i ustalenie jego kodu
{
	if(odebrano_kod_pilota)							// je�li odczytano sygna� to ustal jego kod
	{
		odebrano_kod_pilota = 0;						// zga� flag�
		for(int i=0;i<8;i++)
		{
			buf_ir = (tab[26-i] > 54)? 1 : 0;	// sprawd� d�ugo�� impuls�w
			kod_ir |= buf_ir << i;						// i ustal na ich podstawie kod
			buf_ir = (tab[10-i] > 54)? 1 : 0;
			adres_ir |= buf_ir << i;					// i adres wysy�ane przez pilot
		}
		kod_odczytany = 0;									// zga� flag� informuj�c� o odczytaniu kodu
	}
}

void tryb_oczekiwanie_na_sygnal(void)	// oczekiwanie na sygna� z pilota
{
	LED4_OFF
	LED5_OFF
	LED6_OFF
	kod_ir = 0;
	while ((kod_ir != ok)&&(kod_ir != stop)&&(kod_ir != source))	// dop�ki otrzymywany sygna� nie jest prawid�owy wykonuj:
	{
		sprawdz_sygnal_z_pilota();	// sprawdzaj czy odebrano sygna� i wyznacz jego kod
		_delay_ms(50);							// odczekaj chwil�
		wdt_reset();								// zresetuj watchdoga
	}
	switch (kod_ir)								// w zale�no�ci od kodu sygna�u z pilota
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
