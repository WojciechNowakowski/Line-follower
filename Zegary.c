/*
 * Zegary.c
 *
 * Created: 2014-06-02 14:11:07
 *  Author: Wojciech
 * Literature: http://www.leon-instruments.pl/2013/04/poznaj-atxmega.html
 */ 

#include "Zegary.h"

void Osc32MHz(void) // zmiana zegara na 32 MHz
{
	OSC.CTRL				=		OSC_RC32MEN_bm;       // w³¹czenie oscylatora 32MHz
	while(!(OSC.STATUS & OSC_RC32MRDY_bm));		// czekanie na ustabilizowanie siê generatora
	DFLLRC32M.CTRL	=		DFLL_ENABLE_bm;				// w³¹czenie uk³adu kontroli czêstotliwoœci
	CPU_CCP					=		CCP_IOREG_gc;         // odblokowanie zmiany Ÿród³a sygna³u
	CLK.CTRL				=		CLK_SCLKSEL_RC32M_gc; // zmiana Ÿród³a sygna³u na RC 32MHz
}

void Osc2MHz(void) // zmiana zegara na 2 MHz
{
	OSC.CTRL				=		OSC_RC2MEN_bm;				// w³¹czenie oscylatora 2MHz
	while(!(OSC.STATUS & OSC_RC2MRDY_bm));		// czekanie na ustabilizowanie siê generatora
	DFLLRC2M.CTRL	=		DFLL_ENABLE_bm;					// w³¹czenie uk³adu kontroli czêstotliwoœci
	CPU_CCP					=		CCP_IOREG_gc;         // odblokowanie zmiany Ÿród³a sygna³u
	CLK.CTRL				=		CLK_SCLKSEL_RC2M_gc;	// zmiana Ÿród³a sygna³u na RC 2MHz
}

void OscXtal(void) 
{
	// konfiguracja generatora kwarcowego		
	OSC.XOSCCTRL	=	OSC_FRQRANGE_12TO16_gc |	// wybór kwarcu od 12 do 16 MHZ
						OSC_XOSCSEL_XTAL_16KCLK_gc;			// czas na uruchomienie generatora
	OSC.CTRL		=	OSC_XOSCEN_bm;							// uruchomienie generatora kwarcowego
	
	// czekanie na ustabilizowanie siê generatora
	for(uint8_t i=0; i<255; i++) 
	{						
		if(OSC.STATUS & OSC_XOSCRDY_bm) 
		{
			CPU_CCP			=	CCP_IOREG_gc;					// odblokowanie zmiany Ÿród³a sygna³u zegarowego
			CLK.CTRL		=	CLK_SCLKSEL_XOSC_gc;	// wybór Ÿród³a sygna³u zegarowego na XTAL 16MHz
			// uk³ad nadzoruj¹cy kwarc
			CPU_CCP			=	CCP_IOREG_gc;					// odblokowanie modyfikacji wa¿nych rejestrów 
			OSC.XOSCFAIL	=	OSC_XOSCFDEN_bm;		// w³¹czenie uk³adu detekcji b³êdu sygna³u zegarowego
			return;															// wyjœcie z funkcji jeœli generator siê uruchomi³
		}
		_delay_us(10);
	} 	
}

void OscPLL(uint8_t pllfactor) 
{
	// uruchomienie generatora 2MHz i ustawienie go jako Ÿród³a zegara
	OSC.CTRL		=	OSC_RC2MEN_bm;						// w³¹czenie oscylatora 2MHz
	while(!(OSC.STATUS & OSC_RC2MRDY_bm));	// czekanie na ustabilizowanie siê generatora
	CPU_CCP			=	CCP_IOREG_gc;							// odblokowanie zmiany Ÿród³a sygna³u zegarowego
	CLK.CTRL		=	CLK_SCLKSEL_RC2M_gc;			// zmiana Ÿród³a sygna³u zegarowego na RC 2MHz
	
	// wy³¹czenie PLL
	OSC.CTRL		&= ~OSC_PLLEN_bm;
			
	// konfiguracja PLL
	OSC.PLLCTRL		=	OSC_PLLSRC_RC2M_gc |		// wybór RC 2MHz jako Ÿród³o sygna³u dla PLL
						pllfactor;										// mno¿nik czêstotliwoœci (od 1 do 31)
						
	// uruchomienie PLL
	OSC.CTRL		=	OSC_PLLEN_bm;							// w³¹czenie uk³adu PLL
	
	// czekanie na ustabilizowanie siê generatora			
	while(!(OSC.STATUS & OSC_PLLRDY_bm));

	// prze³¹czenie Ÿród³a sygna³u zegarowego
	CPU_CCP			=	CCP_IOREG_gc;							// odblokowanie zmiany Ÿród³a sygna³u zegarowego
	CLK.CTRL		=	CLK_SCLKSEL_PLL_gc;				// wybór Ÿród³a sygna³u zegarowego PLL
			
	// uk³ad nadzoruj¹cy PLL
	CPU_CCP			=	CCP_IOREG_gc;							// odblokowanie modyfikacji wa¿nych rejestrów 
	OSC.XOSCFAIL	=	OSC_PLLFDEN_bm;					// w³¹czenie uk³adu detekcji b³êdu sygna³u zegarowego
}
