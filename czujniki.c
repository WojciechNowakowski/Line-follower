/*
 * czujniki.c
 *
 * Created: 2014-06-02 02:24:00
 *  Author: Wojciech
 */ 

#include "czujniki.h"

volatile uint8_t czujniki[liczba_czujnikow], num_cz = 0, ilosc_braku_sygnalu;
volatile uint8_t prog = 0xC0, adc_pomiar_napiecia, ilosc_braku_sygnalu;
volatile int8_t wagi_cz[liczba_czujnikow] = {-8, -8, -4, -6, 0, -2, 4, 2, 8, 6, 8};
volatile int16_t polozenie_linii = 0;

ISR(ADCA_CH0_vect)	// przerwanie zako�czenia konwersji ADC na kanale 0
{
	if (num_cz < liczba_czujnikow)														// wykonaj dla wszystkich czujnik�w
	{
		czujniki[num_cz] = (ADCA.CH0RESL > prog)?1:0;						// ustalenie czy przekroczono pr�g pomiaru - czarne czy bia�e
		ADCA.CH0.MUXCTRL = ((num_cz + 1) << ADC_CH_MUXPOS_gp);	// przestawienie pr�bkowanego kana�u
		ADCA.CH0.CTRL |=	ADC_CH_START_bm;											// start ADC
	}else																											// w przeciwnym razie
	{
		adc_pomiar_napiecia = ADCA.CH0RESL;											// zapisz zmierzone napi�cie zasilania
	}
	num_cz++;																									// nast�pny czujnik
}

int16_t sprawdz_stan_czujnikow_i_zasilanie(void)	// odczytuje wskazania czujnik�w i zwraca po�o�enie linii
{
	int16_t wynik = 0;															// zmienne pomocnicze
	int8_t ilosc_aktywnych_cz = 0;
	num_cz = 0;																			// pierwszy czujnik o numerze 0
	ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;				// pierwszy kana� 0
	ADCA.CH0.CTRL |=	ADC_CH_START_bm;							// start ADC
	while (num_cz <= liczba_czujnikow);							// czekaj na sprawdzenie wszystkich czujnik�w
	
	for (unsigned char i = 0; i < liczba_czujnikow; i++)
	{
		if (czujniki[i])
		{
			wynik += wagi_cz[i];												// wynik zale�ny od wagi danego czujnika
			ilosc_aktywnych_cz++;												// sprawd� ile czujnik�w wykry�o lini�
			ilosc_braku_sygnalu = 0;										// gdy kt�ry� z czujnik�w wykrywa lini� kasuj zmienn� licz�c� pomiary bez wykrycia linii
		}
	}
	if (ilosc_aktywnych_cz == 0)										// gdy �aden czujnik nie wykry� linii
	{
		wynik = polozenie_linii;											// zwr�� poprzednie po�o�enie
		ilosc_braku_sygnalu++;												// poinformuj o braku wykrycia linii
		LED1_ON;																			// zapal diod�
	}else
	{
		wynik /= ilosc_aktywnych_cz;									// w przeciwnym wypadku wynik podziel przez liczb� czujnik�w, kt�re wykry�y lini�
		LED1_OFF;																			// zga� diod� informuj�c� o braku wykrycia linii
	}
	return wynik;
}

void adc_init(void)	// konfiguracja ADC
{
	ADCA.CTRLA = ADC_ENABLE_bm;																										// w��czenie ADC
	ADCA.CTRLB = ADC_RESOLUTION_8BIT_gc;																					// pomiar unsigned, rozdzielczo�� 8 bit�w
	ADCA.CALL = ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0) );	// kalibracja ADC
	ADCA.CALH = ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1) );	// kalibracja c.d.
	ADCA.REFCTRL = ADC_REFSEL_VCC_gc;																							// odniesienie VCC/1.6 ok 2.06V przy VCC = 3.3V
	ADCA.PRESCALER = ADC_PRESCALER_DIV64_gc;																			// preskaler
	ADCA.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;															// pomiar typu Single-ended
	ADCA.CH0.INTCTRL = ADC_CH_INTLVL_HI_gc;																				// wysoki priorytet przerwania
	ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;																			// wej�cie ADC0
}
