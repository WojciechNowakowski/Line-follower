/*
 * kalibracja.c
 *
 * Created: 2014-06-02 14:04:27
 *  Author: Wojciech
 */ 

#include "kalibracja.h"

int8_t ReadCalibrationByte( uint8_t index )	// odczytaj dane kalibracyjne
{
	uint8_t result;
	// Load the NVM Command register to read the calibration row.
	NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
	result = pgm_read_byte(index);
	// Clean up NVM Command register.
	NVM_CMD = NVM_CMD_NO_OPERATION_gc;
	return( result );
}

/*	Przyk³ad u¿ycia
	ADCA.CALL = ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0) );	// kalibracja ADC
	ADCA.CALH = ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1) );	// kalibracja c.d.
*/