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
void init_stm32(	volatile struct synth *self,
						mode init_mode,
						TIM_HandleTypeDef *htimDAC,
						DAC_HandleTypeDef *hdac,
						wave_shape shape_in,
						TIM_HandleTypeDef *htimADC,		// timer for adc
						ADC_HandleTypeDef *hadc1			// pointer to the adc
						) {

	self->synth_mode = init_mode;
	self->adc_half_ready[0] = 0;
	self->adc_half_ready[1] = 0;
	self->adc_underruns[0] = 0;
	self->adc_underruns[1] = 0;
	self->adc_overruns[0] = 0;
	self->adc_overruns[1] = 0;
	self->effect_prev_sample = 0;
	set_effect_mode(self, EFFECT_BYPASS);

	// init the DAC
	HAL_TIM_Base_Start(htimDAC);
	HAL_DAC_Start_DMA(hdac, DAC_CHANNEL_2, (uint32_t)self->audio_buffer_dac, AUDIO_BUFFER_SIZE, DAC_ALIGN_12B_R);
	self->audio_buffer_position = 0;

	// fill the DAC buffer
	for (int i=0; i<AUDIO_BUFFER_SIZE; i++){
		self->audio_buffer_dac[i] = 0;
	}

	// Initialize the synth as a source
	if (init_mode == SYNTH) {
		init_oscillator(&self->osc, htimDAC, shape_in);
	}

	// initialize the synth as a effects unit
	else if (init_mode == EFFECTS){
		// get the ADC setup
		HAL_TIM_Base_Start_IT(htimADC);
		HAL_ADCEx_Calibration_Start(hadc1);
		HAL_ADC_Start_DMA(hadc1, (uint32_t)self->audio_buffer_adc, AUDIO_BUFFER_SIZE);
	}
}

/**
 * @brief   This function will be called when the DAC is half way audio buffer (DMA) and when its
 * 			at the end of the audio buffer..
 *  		HAL_DACEx_ConvHalfCpltCallbackCh2: in main.c these functions call render audio block
 *  		HAL_DACEx_ConvCpltCallbackCh2: in main.c these functions call render audio block
 * @param   struct synth
 * @param   uint8_t buffer_half; this is the dac buffer half that sent us to this function.
 * @retval  None.
 */
void render_audio_block(volatile struct synth *self, uint8_t buffer_half){
	uint16_t temp = 0;
	self->audio_buffer_position = buffer_half * AUDIO_BUFFER_HALF_SIZE;

	if (self->synth_mode == SYNTH){
		for(int i=0; i<AUDIO_BUFFER_HALF_SIZE; i++){
			// first half or second half
			// oscillator
			temp = get_next_value(&self->osc);

			// mixer

			// filters

			// adsr

			// fill the buffer
			self->audio_buffer_dac[self->audio_buffer_position] = temp;

			// update index
			self->audio_buffer_position += 1;
		}
	}


	else if (self->synth_mode == EFFECTS) {
		// loop through the buffer
		for(int i=0; i<AUDIO_BUFFER_HALF_SIZE; i++){
			uint16_t current_index = self->audio_buffer_position;
			uint32_t current_sample = self->audio_buffer_sig_path[current_index];

			switch (self->active_effect) {
				case EFFECT_MOVING_AVG:
					self->audio_buffer_dac[current_index] =
							(uint16_t)((current_sample + self->effect_prev_sample) / 2u);
					self->effect_prev_sample = (uint16_t)current_sample;
					break;

				case EFFECT_BYPASS:
				default:
					self->audio_buffer_dac[current_index] = (uint16_t)current_sample;
					self->effect_prev_sample = (uint16_t)current_sample;
					break;
			}

			self->audio_buffer_position += 1;
		}
	}
}

void set_effect_mode(volatile struct synth *self, effect_mode mode_in)
{
	switch (mode_in) {
		case EFFECT_MOVING_AVG:
		case EFFECT_BYPASS:
			self->active_effect = mode_in;
			break;
		default:
			self->active_effect = EFFECT_BYPASS;
			break;
	}

	self->effect_prev_sample = 0;
}

void buffer_adc_input(volatile struct synth *self, uint8_t buffer_half){
	self->audio_buffer_position = buffer_half * AUDIO_BUFFER_HALF_SIZE;

	// Buffer the ADC input
	for (int i=0; i<AUDIO_BUFFER_HALF_SIZE; i++) {

		self->audio_buffer_sig_path[self->audio_buffer_position] = self->audio_buffer_adc[self->audio_buffer_position];

		self->audio_buffer_position += 1;
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
