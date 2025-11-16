/*
 * stm32_module.c
 *
 *  Created on: Nov 14, 2025
 *      Author: mark
 */

#include "stm32_synth.h"

void init_stm32_synth(	volatile struct synth *self,
						mode init_mode,
						TIM_HandleTypeDef *htim,
						DAC_HandleTypeDef *hdac,
						wave_shape shape_in) {

	// begin constructing all the objects
	HAL_TIM_Base_Start_IT(htim);
	init_oscillator(&self->osc1, htim, shape_in);

	// fill the audio buffer
	for (int i=0; i<AUDIO_BUFFER_SIZE; i++){
		self->audio_buffer[i] = get_next_value(&self->osc1);
	}
	self->audio_buffer_position = 0;

}

void render_audio_block(volatile struct synth *self){
	uint16_t temp = 0;
	for(int i=0; i<AUDIO_BUFFER_SIZE/2; i++){
		// first half or second half
		// oscillator
		temp = get_next_value(&self->osc1);

		// mixer

		// filter

		// adsr

		self->audio_buffer[i + self->audio_buffer_position] = temp;
	}
	// change the buffer position
	self->audio_buffer_position += AUDIO_BUFFER_SIZE / 2;
	if (self->audio_buffer_position == AUDIO_BUFFER_SIZE) {
		self->audio_buffer_position = 0;
	}
}
