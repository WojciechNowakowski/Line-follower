/*
 * silniki.h
 *
 * Created: 2014-06-01 23:55:34
 *  Author: Wojciech
 */ 

#ifndef SILNIKI_H_
#define SILNIKI_H_

#include <avr/io.h>

void silniki_init(void) /* konfiguracja timera do PWM 8 kHz */;
void PRAWY_przod(uint16_t wypelnienie) /* prawy silnik do przodu */;
void PRAWY_tyl(uint16_t wypelnienie) /* prawy silnik do ty³u */;
void LEWY_przod(uint16_t wypelnienie) /* lewy silnik do przodu */;
void LEWY_tyl(uint16_t wypelnienie) /* lewy silnik do ty³u */;
void ustawienie_silnikow(int16_t sygnal2) /* ustaw kierunek i prêdkoœæ silników zgodnie z sygna³em steruj¹cym */;

#endif /* SILNIKI_H_ */