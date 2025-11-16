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

#define AUDIO_BUFFER_SIZE 256

typedef enum {
	SYNTH,
	EFFECTS
} mode;


struct synth {
	struct Oscillator osc1;
	struct Mixer mix1;
	struct Filter filt1;
	struct ADSR adsr1;

	struct Seven_Seg seven_seg1;

	DAC_HandleTypeDef synth_hdac;
	mode synth_mode;

	uint16_t audio_buffer[AUDIO_BUFFER_SIZE];
	uint16_t audio_buffer_position;
};

void init_stm32_synth(	volatile struct synth *self,
						mode init_mode,
						TIM_HandleTypeDef *htim,
						DAC_HandleTypeDef *hdac,
						wave_shape shape_in);

void render_audio_block(volatile struct synth *self);

void marry_had_a_little_lamb(volatile struct synth *self);

#endif /* INC_STM32_SYNTH_H_ */
