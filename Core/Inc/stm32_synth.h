/*
 * stm32_module.h
 *
 *  Created on: Nov 14, 2025
 *      Author: mark
 */

#ifndef INC_STM32_SYNTH_H_
#define INC_STM32_SYNTH_H_

#include "notes.h"

#include "seven_seg.h"
#include "oscillator.h"
#include "mixer.h"
#include "filter.h"
#include "adsr.h"

#define AUDIO_BUFFER_SIZE   		256
#define AUDIO_BUFFER_HALF_SIZE 		128

typedef enum {
	SYNTH,
	EFFECTS
} mode;

typedef enum {
	REVERB
} effect_mode;


struct synth {
	struct Oscillator osc;
	struct Mixer mix1;
	struct Filter filt1;
	struct ADSR adsr1;

	struct Seven_Seg display;

	mode synth_mode;

	DAC_HandleTypeDef synth_hdac;
	uint16_t audio_buffer_dac[AUDIO_BUFFER_SIZE];
	uint8_t audio_buffer_position;

	// make a adc audio buffer
	uint16_t audio_buffer_adc[AUDIO_BUFFER_SIZE];
	volatile uint8_t adc_half_ready[2];		// [0]=first half, [1]=second half
	volatile uint32_t adc_underruns[2];		// debug counter
	volatile uint32_t adc_overruns[2];		// debug counter

	uint16_t audio_buffer_sig_path[AUDIO_BUFFER_SIZE];
};

void init_stm32(		volatile struct synth *self,
						mode init_mode,					// stm32 mode
						TIM_HandleTypeDef *htimDAC,		// pointer to timer for dac
						DAC_HandleTypeDef *hdac,		// pointer to dac
						wave_shape shape_in,			// shape for dac (SYNTH)
						TIM_HandleTypeDef *htimADC,		// timer for adc
						ADC_HandleTypeDef *hadc1		// pointer to the adc
						);

void render_audio_block(volatile struct synth *self, uint8_t buffer_half);

void buffer_adc_input(volatile struct synth *self, uint8_t buffer_half);

void marry_had_a_little_lamb(volatile struct synth *self);

#endif /* INC_STM32_SYNTH_H_ */
