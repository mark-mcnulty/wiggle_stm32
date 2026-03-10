/*
 * effects.h
 *
 *  Created on: Nov 14, 2025
 *      Author: mark
 */

#ifndef INC_EFFECTS_H_
#define INC_EFFECTS_H_

#include <stdint.h>

#define MOVING_AVG_MAX_TAPS  32

typedef enum {
	EFFECT_BYPASS,
	EFFECT_MOVING_AVG,
	EFFECT_BIT_CRUSHER
} effect_mode;

struct effect_state {
	effect_mode active_effect;
	uint8_t moving_avg_taps;
	uint8_t moving_avg_index;
	uint8_t moving_avg_count;
	uint32_t moving_avg_sum;
	uint16_t moving_avg_history[MOVING_AVG_MAX_TAPS];
};

void effects_set_mode(volatile struct effect_state *state, effect_mode mode_in);
void effects_set_moving_avg_taps(volatile struct effect_state *state, uint8_t taps);
void effects_process_sample(volatile struct effect_state *state, uint32_t input_sample, uint16_t *output_sample);

#endif /* INC_EFFECTS_H_ */
