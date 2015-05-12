//#define F_CPU 32000000UL
#include <avr/io.h>
#include "Zegary.h"
#include "czujniki.h"
#include "init.h"
#include "silniki.h"
#include "HC-SR04.h"
#include "usart.h"
#include "pilot.h"
#include "watchdog.h"
#include "PID.h"

// ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE
extern volatile uint8_t czujniki[liczba_czujnikow];
extern volatile uint8_t tryb;
extern volatile uint8_t odebrano_kod_pilota, kod_odczytany, kod_ir, adres_ir;
extern volatile uint8_t usart_flaga, odb_byte;
extern char		 usart_bufor[32];
extern volatile int16_t polozenie_linii;
extern volatile uint8_t ilosc_braku_sygnalu, adc_pomiar_napiecia;

volatile int16_t sygnal = 0;
volatile uint8_t flaga_timer;
// ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE ZMIENNE

ISR(TCE0_OVF_vect)	// przerwanie od przepe³nienia g³ównego timera 10 ms
{
	flaga_timer = 1;	// ustaw flagê informuj¹c¹ o up³ywie wyznaczonego czasu
}

void tryb_sledzenie_linii(void)	// pod¹¿anie za lini¹ z pomoc¹ regulatora PID
{
	if(flaga_timer)
	{
		flaga_timer = 0;
		polozenie_linii = sprawdz_stan_czujnikow_i_zasilanie();
		sygnal = PID_wyznacz_sygnal(polozenie_linii);
		ustawienie_silnikow(sygnal);		// ustaw kierunek i prêdkoœæ silników zgodnie z sygna³em steruj¹cym
		
		if(ilosc_braku_sygnalu > 99)		// jeœli nie powróci na liniê w ci¹gu 1s to zatrzymaj
		{
			LEDS_ON;											// zapal wszystkie diody
			LEWY_stop;										// zatrzymaj silniki
			PRAWY_stop;										// -||-
			tryb = oczekiwanie_na_sygnal;	// oczekuj na sygna³ z pilota
		}else
		{
			LED4_ON
			LED5_OFF
			LED6_OFF
		}

		if (adc_pomiar_napiecia < 85)	// informowanie o stanie baterii
		{
			LED3_ON
		} else
		{
			if (adc_pomiar_napiecia < 90)
			{
				LED2_ON
			} else
			{
				LED2_OFF
				LED3_OFF
			}
		}
	}
}

void tryb_testy_silnika_czujnikow(void)	// polecenia testowe silników i czujników______________________________________________________________________
{
	LED4_OFF
	LED5_OFF
	LED6_ON
	switch (kod_ir)
	{
		case time:
			sprawdz_stan_czujnikow_i_zasilanie();
			for (int8_t i = 0; i < liczba_czujnikow; i++)
			{
				if (czujniki[i])
				{
					LED_PORT.OUTCLR = i + 1;
					_delay_ms(200);
					LED_PORT.OUTSET = i + 1;
					_delay_ms(200);
				}
			}
		break;
		case przod:
			LEWY_przod(max_pwm);
			PRAWY_przod(max_pwm);
		break;
		case tyl:
		  LEWY_tyl(max_pwm);
		  PRAWY_tyl(max_pwm);
		break;
		case prawo:
		  PRAWY_przod(max_pwm);
		  LEWY_stop;
		break;
		case lewo:
		  LEWY_przod(max_pwm);
		  PRAWY_stop;
		break;
		case ok:
		  LEWY_stop;
		  PRAWY_stop;
		break;
		case stop:
			tryb = oczekiwanie_na_sygnal;
		break;
		case source:
			tryb = testy_silnika;
		break;
		case zero:
		break;
		case jeden:
		break;
		case dwa:
		break;
		case trzy:
		break;
		case cztery:
		break;
		case piec:
		break;
		case siedem:
		break;
		case osiem:
		break;
		case dziewiec:
		break;
	}
}

void init(void)	// funkcja inicjalizuj¹ca robota
{
	led_init();							// konfiguracja ledów
	#if 1
		usart_init();					// konfiguracja USART
	#else
		HC_SR04_init();				// konfiguracja czujnika HC-SR04
	#endif
	adc_init();							// konfiguracja ADC
	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm; // W³¹czamy wszystkie poziomy przerwañ
	sei();
	odczyt_konfiguracji();	// odczyt nastaw z EEPROM
	IR_remote_init();				// konfiguracja input capture do pilota IR
	wdt_init();							// konfiguracja watchdoga 1s
	silniki_init();					// konfiguracja timera do PWM 16 kHz
	main_timer_init();			// konfiguracja timera g³ównego 10ms
}

int main(void)
{
	Osc32MHz();		// zmiana zegara na 32 MHz
	init();				// inicjalizacja robota

	while (1)
	{
		if (!(kod_odczytany))
		{
			switch (kod_ir)
			{
				case stop:
					LEDS_ON;											// zapal wszystkie diody
					LEWY_stop;										// zatrzymaj silniki
					PRAWY_stop;										// -||-
					tryb = oczekiwanie_na_sygnal;	// oczekuj na sygna³ z pilota
				break;
				case source:
					wyslij_nastawy_PID();
				break;
			}
			kod_odczytany = 1;
		}
		if (tryb == oczekiwanie_na_sygnal)
		{
			tryb_oczekiwanie_na_sygnal();
		}
		if (tryb == sledzenie_linii)
		{
			tryb_sledzenie_linii();
		}
		if (tryb == testy_silnika)
		{
			tryb_testy_silnika_czujnikow();
		}
		if (tryb == ust_wsp_reg_PID)
		{
			tryb_ust_wsp_reg_PID();
		}
		sprawdz_sygnal_z_pilota();
		wdt_reset();
	}
}