/*
 * PID.c
 *
 * Created: 2014-06-02 13:03:23
 *  Author: Wojciech
 */ 

#include "PID.h"

int16_t k_p = 250, k_i = 16, k_d = 140;
volatile uint16_t ee_k_p[1] EEMEM, ee_k_i[1] EEMEM, ee_k_d[1] EEMEM;   //zmienne w pamiêci EEPROM
extern volatile uint8_t usart_flaga, odb_byte;
extern char			usart_bufor[32];
extern volatile uint8_t tryb;

int16_t PID_wyznacz_sygnal(int16_t wejscie)	// wyznaczenie sygna³u id¹cego na silniki na podstawie po³o¿enia linii
{
	static int16_t pop_uchyb = 0, uchyb = 0, rozniczka = 0, calka = 0;
	int16_t wyjscie;

	uchyb = 0 - wejscie;	// lewo - ujemny
	calka += uchyb;
 
	// prosty anty wind-up
	if(calka > calka_max / k_i)
		calka = calka_max / k_i;
	if(calka < -calka_max / k_i)
		calka = -calka_max / k_i;
	if (uchyb == 0)
	{
		calka = 0;
	}
 
	rozniczka = k_d * (uchyb - pop_uchyb) + rozniczka * 5 / 6;	// ró¿niczkowanie z inercj¹
	pop_uchyb = uchyb;
 
	wyjscie = k_p * uchyb + k_i * calka + rozniczka;

	return wyjscie;
}

void odczyt_konfiguracji(void)	// odczyt nastaw z EEPROM
{
	int16_t odczyt;
	odczyt = eeprom_read_word((uint16_t*)&ee_k_p);		// odczyt wzmocnienia cz³onu proporcjonalnego regulatora z EEPROM
	if (odczyt > 0)
	{
		k_p = odczyt;
	}
	odczyt = eeprom_read_word((uint16_t*)&ee_k_i);		// odczyt wzmocnienia cz³onu ca³kuj¹cego			regulatora z EEPROM
	if (odczyt > 0)
	{
		k_i = odczyt;
	}
	odczyt = eeprom_read_word((uint16_t*)&ee_k_d);		// odczyt wzmocnienia cz³onu ró¿niczkuj¹cego	regulatora z EEPROM
	if (odczyt > 0)
	{
		k_d = odczyt;
	}

	wyslij_nastawy_PID();
}

void wyslij_nastawy_PID(void)
{
	sprintf(usart_bufor, "k_p = %4d\n\r", k_p);		// wyœwietl informacjê o wartoœci odczytanego wspó³czynnika
	wyslij_usart(12);
	_delay_ms(6);																	// odczekaj czas wysy³ania
	
	sprintf(usart_bufor, "k_i = %4d\n\r", k_i);		// wyœwietl informacjê o wartoœci odczytanego wspó³czynnika
	wyslij_usart(12);
	_delay_ms(6);																	// odczekaj czas wysy³ania

	sprintf(usart_bufor, "k_d = %4d\n\r", k_d);		// wyœwietl informacjê o wartoœci odczytanego wspó³czynnika
	wyslij_usart(12);
	_delay_ms(6);
}

void tryb_ust_wsp_reg_PID(void)	// ustawianie przez usart wspó³czynników regulatora PID
{
	LED4_OFF
	LED5_ON
	LED6_OFF
	sprintf(usart_bufor, "Podaj k_p_h:\n");
	usart_bufor[13] = 13;
	wyslij_usart(14);
	while(usart_flaga == 0)
	{
		wdt_reset();
		_delay_ms(100);
	}
	usart_flaga = 0;
	k_p = odb_byte << 8;
	
	sprintf(usart_bufor, "Podaj k_p_l:\n");
	usart_bufor[13] = 13;
	wyslij_usart(14);
	while(usart_flaga == 0)
	{
		wdt_reset();
		_delay_ms(100);
	}
	usart_flaga = 0;
	k_p += odb_byte;

	sprintf(usart_bufor, "k_p = %4d\n", k_p);
	usart_bufor[11] = 13;
	wyslij_usart(12);
	eeprom_write_word((uint16_t*)&ee_k_p, k_p);
	_delay_ms(5);
	
	
	sprintf(usart_bufor, "Podaj k_i_h:\n");
	usart_bufor[13] = 13;
	wyslij_usart(14);
	while(usart_flaga == 0)
	{
		wdt_reset();
		_delay_ms(100);
	}
	usart_flaga = 0;
	k_i = odb_byte << 8;
	
	sprintf(usart_bufor, "Podaj k_i_l:\n");
	usart_bufor[13] = 13;
	wyslij_usart(14);
	while(usart_flaga == 0)
	{
		wdt_reset();
		_delay_ms(100);
	}
	usart_flaga = 0;
	k_i += odb_byte;

	sprintf(usart_bufor, "k_i = %4d\n", k_i);
	usart_bufor[11] = 13;
	wyslij_usart(12);
	eeprom_write_word((uint16_t*)&ee_k_i, k_i);
	_delay_ms(5);
	
	sprintf(usart_bufor, "Podaj k_d_h:\n");
	usart_bufor[13] = 13;
	wyslij_usart(14);
	while(usart_flaga == 0)
	{
		wdt_reset();
		_delay_ms(100);
	}
	usart_flaga = 0;
	k_d = odb_byte << 8;
	
	sprintf(usart_bufor, "Podaj k_d_l:\n");
	usart_bufor[13] = 13;
	wyslij_usart(14);
	while(usart_flaga == 0)
	{
		wdt_reset();
		_delay_ms(100);
	}
	usart_flaga = 0;
	k_d += odb_byte;
	
	sprintf(usart_bufor, "k_d = %4d\n", k_d);
	usart_bufor[11] = 13;
	wyslij_usart(12);
	eeprom_write_word((uint16_t*)&ee_k_d, k_d);
	_delay_ms(5);
	
	tryb = oczekiwanie_na_sygnal;
}