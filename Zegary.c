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
	OSC.CTRL				=		OSC_RC32MEN_bm;       // w��czenie oscylatora 32MHz
	while(!(OSC.STATUS & OSC_RC32MRDY_bm));		// czekanie na ustabilizowanie si� generatora
	DFLLRC32M.CTRL	=		DFLL_ENABLE_bm;				// w��czenie uk�adu kontroli cz�stotliwo�ci
	CPU_CCP					=		CCP_IOREG_gc;         // odblokowanie zmiany �r�d�a sygna�u
	CLK.CTRL				=		CLK_SCLKSEL_RC32M_gc; // zmiana �r�d�a sygna�u na RC 32MHz
}

void Osc2MHz(void) // zmiana zegara na 2 MHz
{
	OSC.CTRL				=		OSC_RC2MEN_bm;				// w��czenie oscylatora 2MHz
	while(!(OSC.STATUS & OSC_RC2MRDY_bm));		// czekanie na ustabilizowanie si� generatora
	DFLLRC2M.CTRL	=		DFLL_ENABLE_bm;					// w��czenie uk�adu kontroli cz�stotliwo�ci
	CPU_CCP					=		CCP_IOREG_gc;         // odblokowanie zmiany �r�d�a sygna�u
	CLK.CTRL				=		CLK_SCLKSEL_RC2M_gc;	// zmiana �r�d�a sygna�u na RC 2MHz
}

void OscXtal(void) 
{
	// konfiguracja generatora kwarcowego		
	OSC.XOSCCTRL	=	OSC_FRQRANGE_12TO16_gc |	// wyb�r kwarcu od 12 do 16 MHZ
						OSC_XOSCSEL_XTAL_16KCLK_gc;			// czas na uruchomienie generatora
	OSC.CTRL		=	OSC_XOSCEN_bm;							// uruchomienie generatora kwarcowego
	
	// czekanie na ustabilizowanie si� generatora
	for(uint8_t i=0; i<255; i++) 
	{						
		if(OSC.STATUS & OSC_XOSCRDY_bm) 
		{
			CPU_CCP			=	CCP_IOREG_gc;					// odblokowanie zmiany �r�d�a sygna�u zegarowego
			CLK.CTRL		=	CLK_SCLKSEL_XOSC_gc;	// wyb�r �r�d�a sygna�u zegarowego na XTAL 16MHz
			// uk�ad nadzoruj�cy kwarc
			CPU_CCP			=	CCP_IOREG_gc;					// odblokowanie modyfikacji wa�nych rejestr�w 
			OSC.XOSCFAIL	=	OSC_XOSCFDEN_bm;		// w��czenie uk�adu detekcji b��du sygna�u zegarowego
			return;															// wyj�cie z funkcji je�li generator si� uruchomi�
		}
		_delay_us(10);
	} 	
}

void OscPLL(uint8_t pllfactor) 
{
	// uruchomienie generatora 2MHz i ustawienie go jako �r�d�a zegara
	OSC.CTRL		=	OSC_RC2MEN_bm;						// w��czenie oscylatora 2MHz
	while(!(OSC.STATUS & OSC_RC2MRDY_bm));	// czekanie na ustabilizowanie si� generatora
	CPU_CCP			=	CCP_IOREG_gc;							// odblokowanie zmiany �r�d�a sygna�u zegarowego
	CLK.CTRL		=	CLK_SCLKSEL_RC2M_gc;			// zmiana �r�d�a sygna�u zegarowego na RC 2MHz
	
	// wy��czenie PLL
	OSC.CTRL		&= ~OSC_PLLEN_bm;
			
	// konfiguracja PLL
	OSC.PLLCTRL		=	OSC_PLLSRC_RC2M_gc |		// wyb�r RC 2MHz jako �r�d�o sygna�u dla PLL
						pllfactor;										// mno�nik cz�stotliwo�ci (od 1 do 31)
						
	// uruchomienie PLL
	OSC.CTRL		=	OSC_PLLEN_bm;							// w��czenie uk�adu PLL
	
	// czekanie na ustabilizowanie si� generatora			
	while(!(OSC.STATUS & OSC_PLLRDY_bm));

	// prze��czenie �r�d�a sygna�u zegarowego
	CPU_CCP			=	CCP_IOREG_gc;							// odblokowanie zmiany �r�d�a sygna�u zegarowego
	CLK.CTRL		=	CLK_SCLKSEL_PLL_gc;				// wyb�r �r�d�a sygna�u zegarowego PLL
			
	// uk�ad nadzoruj�cy PLL
	CPU_CCP			=	CCP_IOREG_gc;							// odblokowanie modyfikacji wa�nych rejestr�w 
	OSC.XOSCFAIL	=	OSC_PLLFDEN_bm;					// w��czenie uk�adu detekcji b��du sygna�u zegarowego
}
