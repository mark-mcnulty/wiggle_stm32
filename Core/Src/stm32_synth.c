/*
 * stm32_module.c
 *
 *  Created on: Nov 14, 2025
 *      Author: mark
 */

#include "stm32_synth.h"


/**
 * @brief   This will initialize the class of stm32 "class"
 * @param   struct synth
 * @retval  None.
 */
void init_stm32_synth(	volatile struct synth *self,
						mode init_mode,
						TIM_HandleTypeDef *htim,
						DAC_HandleTypeDef *hdac,
						wave_shape shape_in) {

	self->synth_mode = init_mode;

	// Initialize the synth as a source
	if (init_mode == SYNTH) {
		HAL_TIM_Base_Start_IT(htim);
		init_oscillator(&self->osc, htim, shape_in);

		// fill the audio buffer
		for (int i=0; i<AUDIO_BUFFER_SIZE; i++){
			self->audio_buffer[i] = get_next_value(&self->osc);
		}
		self->audio_buffer_position = 0;
	}

	// initialize the synth as a effects unit
	if (init_mode == EFFECTS){

	}

}

/**
 * @brief   This function will be called when the DAC is half way audio buffer (DMA) and when its
 * 			at the end of the audio buffer..
 *  		HAL_DACEx_ConvHalfCpltCallbackCh2: in main.c these functions call render audio block
 *  		HAL_DACEx_ConvCpltCallbackCh2: in main.c these functions call render audio block
 * @param   struct synth
 * @retval  None.
 */
void render_audio_block(volatile struct synth *self){
	if (self->synth_mode == SYNTH){
		uint16_t temp = 0;
		for(int i=0; i<AUDIO_BUFFER_SIZE/2; i++){
			// first half or second half
			// oscillator
			temp = get_next_value(&self->osc);

			// mixer

			// filters

			// adsr

			self->audio_buffer[i + self->audio_buffer_position] = temp;
		}
		// change the buffer position
		self->audio_buffer_position += AUDIO_BUFFER_SIZE / 2;
		if (self->audio_buffer_position == AUDIO_BUFFER_SIZE) {
			self->audio_buffer_position = 0;
		}
	} else if (self->synth_mode == EFFECTS) {

	}
}

void marry_had_a_little_lamb(volatile struct synth *self){

	  self->osc.f_out = NOTES[NOTE_E][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_D][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_C][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_D][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_E][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_E][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_E][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(800);



	  self->osc.f_out = NOTES[NOTE_D][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_D][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_D][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(800);



	  self->osc.f_out = NOTES[NOTE_E][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_G][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_G][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(800);



	  self->osc.f_out = NOTES[NOTE_E][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_D][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_C][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_D][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_E][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_E][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_E][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(800);



	  self->osc.f_out = NOTES[NOTE_E][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_D][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_D][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_E][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_D][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

	  self->osc.f_out = NOTES[NOTE_C][4];
	  set_oscillator_freq(&self->osc, self->osc.f_out);
	  HAL_Delay(400);

}
