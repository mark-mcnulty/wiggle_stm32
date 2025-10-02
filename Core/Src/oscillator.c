/*
 * oscillator.c
 *
 *  Created on: Oct 2, 2025
 *      Author: mark
 */
#include "oscillator.h"

/*
 * things that need to be checked
 * 1) what is the LUT_bit_num value? is it 10? Should be.
 */


/**
 * @brief  	Function will initialize the "oscillator" object
 * @param  	struct Oscillator *self
 * @retval 	None
 */
void init_oscillator(struct Oscillator *self) {
	// set the constants from the clock
	self->fclock = 1000000;		// 1Mhz is the input clock
	self->PSC = 6;				// this is the pre-scaler set on tim6
	self->ARR = 2;				// this is the auto reload register
	self->fstep = (self->fclock / ((1 + self->PSC) * (1 + self->ARR)));

	// initialize the lookup table as empty
	for (int i = 0; i < LUT_SIZE; i++) {
		self->wav_LUT[i] = 0;
	}

	// start the phase accumulator at 0
	self->phase_acc = 0x00000000;

	// start the phase_inc
	self->phase_inc = 0x0;

	// set the frequency of the the output wave
	self->fout = 333;

	// set the amount of bits are used in the phase acc used for the LUT
	self->LUT_bit_num = log(LUT_SIZE) / log(2) ;
}

/**
 * @brief  	This function will initialize the LUT with a waveform. It might
 *
 * @param  	struct Oscillator *self:
 * 			uint32_t freq: this is the frequency in Hz
 * @retval 	None
 */
void init_signal(struct Oscillator *self){
	// set each index of the wav_LUT to a value in a sin wave.
	// The DAC is 12 bits wide to multiply that by (2^12 - 1)
	for (int i = 0; i < LUT_SIZE; i++){
        double theta = (2.0 * 3.14 / LUT_SIZE) * i ;
		self->wav_LUT[i] = (uint16_t)((sinf(theta) * 0.5 + 0.5) * 4095);
	}
}

/**
 * @brief  	Function is called by user to set the oscillators output
 * 			frequency, it does this by calculating and setting the
 * 			structs phase increment through the phase accumulator.
 * @param  	struct Oscillator *self:
 * 			uint32_t freq: this is the frequency in Hz
 * @retval 	None
 */
void set_oscillator_freq(struct Oscillator *self, uint32_t freq) {
	self->phase_inc = (uint32_t)( (freq * 2^32) / self->fstep );
}


// get the next value in the table
uint16_t get_next_value(struct Oscillator *self) {
	self->phase_acc += self->phase_inc;
	return self->wav_LUT[self->phase_acc >> (32 - self->LUT_bit_num)];
}
