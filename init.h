/*
 * init.h
 *
 * Created: 2014-06-01 23:36:29
 *  Author: Wojciech
 */ 

#ifndef INIT_H_
#define INIT_H_ 

#include <avr/io.h>
#include <util/delay.h>

//definicje
#define LED_DDR_init	PORTE.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm;\
											PORTD.DIRSET = PIN4_bm | PIN5_bm | PIN6_bm;
#define LED_PORT		PORTE
#define LED1_ON 		PORTD.OUTCLR = PIN4_bm;
#define LED2_ON 		PORTD.OUTCLR = PIN5_bm;
#define LED3_ON 		PORTD.OUTCLR = PIN6_bm;
#define LED4_ON 		PORTE.OUTCLR = PIN0_bm;
#define LED5_ON 		PORTE.OUTCLR = PIN1_bm;
#define LED6_ON 		PORTE.OUTCLR = PIN2_bm;
#define LED7_ON 		PORTE.OUTCLR = PIN3_bm;

#define LED1_TGL 		PORTD.OUTTGL = PIN4_bm;
#define LED2_TGL 		PORTD.OUTTGL = PIN5_bm;
#define LED3_TGL 		PORTD.OUTTGL = PIN6_bm;
#define LED4_TGL		PORTE.OUTTGL = PIN0_bm;
#define LED5_TGL		PORTE.OUTTGL = PIN1_bm;
#define LED6_TGL		PORTE.OUTTGL = PIN2_bm;
#define LED7_TGL		PORTE.OUTTGL = PIN3_bm;

#define LED1_OFF 		PORTD.OUTSET = PIN4_bm;
#define LED2_OFF 		PORTD.OUTSET = PIN5_bm;
#define LED3_OFF 		PORTD.OUTSET = PIN6_bm;
#define LED4_OFF 		PORTE.OUTSET = PIN0_bm;
#define LED5_OFF 		PORTE.OUTSET = PIN1_bm;
#define LED6_OFF 		PORTE.OUTSET = PIN2_bm;
#define LED7_OFF 		PORTE.OUTSET = PIN3_bm;
#define LEDS_OFF		LED1_OFF LED2_OFF LED3_OFF LED4_OFF LED5_OFF LED6_OFF LED7_OFF
#define LEDS_ON			LED1_ON LED2_ON LED3_ON LED4_ON LED5_ON LED6_ON LED7_ON

#define S1_1_ch			TCD0.CCA
#define S1_1off 		S1_1_ch = 0; PORTD.OUTCLR = PIN0_bm;
#define S1_2_ch			TCD0.CCB
#define S1_2off 		S1_2_ch = 0; PORTD.OUTCLR = PIN1_bm;
#define S2_1_ch			TCD0.CCC
#define S2_1off 		S2_1_ch = 0; PORTD.OUTCLR = PIN2_bm;
#define S2_2_ch			TCD0.CCD
#define S2_2off 		S2_2_ch = 0; PORTD.OUTCLR = PIN3_bm;

#define PRAWY_stop				S1_1off S1_2off
#define LEWY_stop					S2_1off S2_2off

#define max_pwm									4000
#define calka_max								3200
#define oczekiwanie_na_sygnal		0
#define ust_wsp_reg_PID					1
#define testy_silnika						2
#define sledzenie_linii					3

void main_timer_init(void) /* konfiguracja timera g³ównego 10ms */;
void led_init(void) /* konfiguracja i mryganie diodami */;

#endif /* INIT_H_ */