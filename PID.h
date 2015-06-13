/*
 * PID.h
 *
 * Created: 2014-06-02 13:03:06
 *  Author: Wojciech
 */ 

#ifndef PID_H_
#define PID_H_

#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include "init.h"
#include "usart.h"
#include "watchdog.h"

int16_t PID_wyznacz_sygnal(int16_t uchyb) /* wyznaczenie sygna³u id¹cego na silniki na podstawie po³o¿enia linii */;
void odczyt_konfiguracji(void) /* odczyt nastaw z EEPROM */;
void tryb_ust_wsp_reg_PID(void) /* ustawianie przez usart wspó³czynników regulatora PID */;
void wyslij_nastawy_PID(void);

#endif /* PID_H_ */