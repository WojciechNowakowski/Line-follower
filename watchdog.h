/*
 * watchdog.h
 *
 * Created: 2014-06-02 02:46:09
 *  Author: Wojciech
 */ 

#include <avr/io.h>
#include <avr/wdt.h>

#ifndef WATCHDOG_H_
#define WATCHDOG_H_


void wdt_init(void) /* inicjalizacja watchdoga na 1 s */;



#endif /* WATCHDOG_H_ */