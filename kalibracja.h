#ifndef KALIBRACJA_H_
#define KALIBRACJA_H_

#include <avr/io.h>
#include <stddef.h>
#include <avr/pgmspace.h>

int8_t ReadCalibrationByte( uint8_t index ) /* odczytaj dane kalibracyjne */;

#endif /* KALIBRACJA_H_ */