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
#include <stdlib.h>

#define LUT_SIZE 1024
#define M_PI 3.14159265358979323846

//
typedef enum {
	SINE,
	SQUARE,
	TRIANGLE,
	SAW,
	NOISE
} wave_shape;


struct Oscillator {
	float fclock;
	float f_inter;
	float f_out;
	uint16_t PSC;
	uint16_t ARR;

	uint16_t wav_LUT[LUT_SIZE];
	uint32_t phase_acc;
	uint32_t phase_step;
	uint8_t LUT_bit_num;

	wave_shape shape;
};


// constructor of the class
void init_oscillator(volatile struct Oscillator *self, wave_shape shape);

// set the waveform
void init_signal(volatile struct Oscillator *self);

// set oscillator frequency
void set_oscillator_freq(volatile struct Oscillator *self, uint32_t freq);

// get next the next value of the lut
uint16_t get_next_value(volatile struct Oscillator *self);


#endif /* INC_OSCILLATOR_H_ */
