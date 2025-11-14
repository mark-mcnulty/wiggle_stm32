/*
 * oscillator.c
 *
 *  Created on: Oct 2, 2025
 *      Author: mark
 */
#include "oscillator.h"

/**
 * @brief  	Function will initialize the "oscillator" object
 * @param  	struct Oscillator *self
 * @retval 	None
 */
void init_oscillator(volatile struct Oscillator *self, TIM_HandleTypeDef *htim, wave_shape shape_in) {
	// set the constants from the clock
	self->PSC = htim->Instance->PSC;													// this is the pre-scaler set on tim6
	self->ARR = htim->Instance->ARR;													// this is the auto reload register
	self->fclock = HAL_RCC_GetPCLK1Freq();												// 24Mhz is the input clock
	self->f_out = 333;																	// arbitrary value for now.
	self->f_inter = ((float)self->fclock / ((1.0f + self->PSC) * (1.0f + self->ARR)));

	// start the phase accumulator at 0
	self->shape = shape_in;
	self->phase_acc = 0x00000000;
	self->phase_step = 0x0;
	self->LUT_bit_num = 10;  				   //log(LUT_SIZE) / log(2) ;
	init_signal_LUT(self);
	set_oscillator_freq(self, self->f_out);
}

/**
 * @brief  	This function will initialize the LUT with a waveform. It inits
 * 			with a sin wave to the table.
 *
 * @param  	struct Oscillator *self:
 * 			uint32_t freq: this is the frequency in Hz
 * @retval 	None
 */
void init_signal_LUT(volatile struct Oscillator *self){
	float step_size = 0;
	float sum = 0;
	switch (self->shape){
		case SINE:
			// set each index of the wav_LUT to a value in a sin wave.
			// The DAC is 12 bits wide to multiply that by (2^12 - 1)
			for (int i=0; i<LUT_SIZE; i++){
				float theta = (2.0f * (float)M_PI / (float)LUT_SIZE) * i ;
				self->wav_LUT[i] = (uint16_t)((sinf(theta) * 0.5f + 0.5f) * 4095.0f);
			}
			break;

		case SQUARE:
			for (int i=0; i<LUT_SIZE; i++){
				// first half of wave gets value of 0
				if (i <= ((LUT_SIZE/2) - 1)) {
					self->wav_LUT[i] = (uint16_t)(0.0f);
				} else if (i >= LUT_SIZE/2) {
					self->wav_LUT[i] = (uint16_t)(4095.0f);
				}

			}
			break;

		case TRIANGLE:
			step_size = (4095.0f) / (LUT_SIZE / 2);
			sum = 0;
			for (int i=0; i<LUT_SIZE; i++){
				if (i <= ((LUT_SIZE/2) - 1)) {
					self->wav_LUT[i] = sum;
					sum = sum + step_size;
				} else if (i >= LUT_SIZE/2) {
					self->wav_LUT[i] = sum;
					sum = sum - step_size;
				}
			}
			break;

		case SAW:
			step_size = 4095.0f / LUT_SIZE;
			for (int i=0; i<LUT_SIZE; i++){
				self->wav_LUT[i] = (uint16_t)(i * step_size);
			}
			break;

		case NOISE:
			for (int i=0; i<LUT_SIZE; i++){
				self->wav_LUT[i] = (uint16_t)((((float)rand() / RAND_MAX) * 4095.0f));
			}
			break;

	} // end switch statement
} // end init_signal

/**
 * @brief  	Function is called by user to set the oscillators output
 * 			frequency, it does this by calculating and setting the
 * 			structs phase increment through the phase accumulator.
 * @param  	struct Oscillator *self:
 * 			uint32_t freq: this is the frequency in Hz
 * @retval 	None
 */
void set_oscillator_freq(volatile struct Oscillator *self, uint32_t freq_out) {
	self->f_out = freq_out;
	uint64_t full = ((uint64_t)freq_out << 32);
	//self->phase_step = (uint32_t)( (freq_out << 32) / self->f_inter );
	self->phase_step = (uint32_t)(full/(uint64_t)self->f_inter);
}


/**
 * @brief  	This function is called every time there is an interrupt.
 * 			It will take the phase accumulator and add the phase step.
 * 			then it will take the 10 top bits from the phase accumulator
 * 			and use those as what position we are in the LUT / wave.
 *
 * @param  	struct Oscillator *self:
 * @retval 	returns uint16_t value for the DAC.
 */
uint16_t get_next_value(volatile struct Oscillator *self) {
	// accumulate your phase step
	self->phase_acc += self->phase_step;

	// pull the top 10 bits off of the phase accumulator. Using a right bit shift.
	return self->wav_LUT[self->phase_acc >> (32 - self->LUT_bit_num)];
}
