/*
 * pilot.h
 *
 * Created: 2014-06-02 00:39:52
 *  Author: Wojciech
 */ 

#ifndef PILOT_H_
#define PILOT_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "init.h"
#include "watchdog.h"

#define ok				64
#define stop			145
#define source		177
#define time			128
#define prawo			193
#define lewo			65
#define przod			129
#define tyl				33
#define zero			192
#define jeden			161
#define dwa				176
#define trzy			240
#define cztery		97
#define piec			96
#define szesc			225
#define siedem		1
#define osiem			224
#define dziewiec	48

void IR_remote_init(void) /* konfiguracja input capture do pilota IR */;
void sprawdz_sygnal_z_pilota(void) /* sprawdzenie odbioru sygna³u i ustalenie jego kodu */;
void tryb_oczekiwanie_na_sygnal(void) /* oczekiwanie na sygna³ z pilota */;

#endif /* PILOT_H_ */