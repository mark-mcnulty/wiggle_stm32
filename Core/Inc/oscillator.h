/*
 * oscillator.h
 *
 *  Created on: Oct 2, 2025
 *      Author: mark
 */

#ifndef INC_OSCILLATOR_H_
#define INC_OSCILLATOR_H_

#include "stm32f1xx_hal.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include <math.h>

#define LUT_SIZE 1024

// make the objects struct
struct Oscillator {
	float fclock;
	float fstep;
	float fout;
	uint8_t PSC;
	uint8_t ARR;

	uint16_t wav_LUT[LUT_SIZE];
	uint32_t phase_acc;
	uint32_t phase_inc;
	uint8_t LUT_bit_num;
};

// constructor of the class
void init_oscillator(struct Oscillator *self);

// set the waveform
void init_signal(struct Oscillator *self);

// set oscillator frequency
void set_oscillator_freq(struct Oscillator *self, uint32_t freq);

// get next the next value of the lut
uint16_t get_next_value(struct Oscillator *self);


#endif /* INC_OSCILLATOR_H_ */
