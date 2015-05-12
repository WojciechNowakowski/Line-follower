/*
 * watchdog.c
 *
 * Created: 2014-06-02 02:46:30
 *  Author: Wojciech
 */ 

#include "watchdog.h"
  
void wdt_init(void)	// inicjalizacja watchdoga na 1 s
{
	CPU_CCP =	CCP_IOREG_gc;																		// odblokowanie zmiany
	WDT.CTRL = WDT_CEN_bm | WDT_PER_1KCLK_gc | WDT_ENABLE_bm;	// w³¹czenie watchdoga z preskalerem 1k
}