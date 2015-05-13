#ifndef HC_SR04_H_
#define HC_SR04_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define HC_SR04_trig_PORT					PORTC
#define HC_SR04_echo_PORT					PORTC
#define HC_SR04_trig_Pin					PIN2_bm
#define HC_SR04_echo_Pin					PIN3_bm
#define HC_SR04_echo_PinCTRL			PIN3CTRL
#define HC_SR04_echo_Event_CH			CH5MUX
#define HC_SR04_echo_Event_src		EVSYS_CHMUX_PORTC_PIN3_gc
#define HC_SR04_timer							TCC1
#define HC_SR04_timer_event				TC_EVSEL_CH5_gc
#define HC_SR04_timer_prescaler		TC_CLKSEL_DIV64_gc	// dla F_CPU = 32 MHz
#define HC_SR04_timer_int_vect		TCC1_CCA_vect
#define HC_SR04_timer_int_lvl			TC_CCAINTLVL_MED_gc
#define HC_SR04_timer_cmp_ch_bm		TC1_CCAEN_bm

ISR(HC_SR04_timer_int_vect);	// przerwanie przechwycenia A timera - czujnik HC-SR04

void HC_SR04_init(void) /* konfiguracja czujnika HC-SR04 */;

void HC_SR04_trigger(void) /* wyzwolenie pomiaru przez czujnik HC-SR04 */;

#endif /* HC_SR04_H_ */