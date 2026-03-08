/*
 * effects.c
 *
 *  Created on: Nov 14, 2025
 *      Author: mark
 */

#include "effects.h"

static void reset_moving_average_state(volatile struct effect_state *state)
{
	state->moving_avg_index = 0;
	state->moving_avg_count = 0;
	state->moving_avg_sum = 0;
	for (int i = 0; i < MOVING_AVG_MAX_TAPS; i++) {
		state->moving_avg_history[i] = 0;
	}
}

void effects_set_mode(volatile struct effect_state *state, effect_mode mode_in)
{
	switch (mode_in) {
		case EFFECT_MOVING_AVG:
		case EFFECT_BYPASS:
			state->active_effect = mode_in;
			break;
		default:
			state->active_effect = EFFECT_BYPASS;
			break;
	}

	reset_moving_average_state(state);
}

void effects_set_moving_avg_taps(volatile struct effect_state *state, uint8_t taps)
{
	if (taps == 0u) {
		state->moving_avg_taps = 1u;
	} else if (taps > MOVING_AVG_MAX_TAPS) {
		state->moving_avg_taps = MOVING_AVG_MAX_TAPS;
	} else {
		state->moving_avg_taps = taps;
	}

	reset_moving_average_state(state);
}

void effects_process_sample(volatile struct effect_state *state, uint32_t input_sample, uint16_t *output_sample)
{
	switch (state->active_effect) {
		case EFFECT_MOVING_AVG: {
			/*
			 * N-tap moving average: smooths the signal by outputting the mean of the
			 * last N samples. Uses a ring buffer (moving_avg_history) and a running
			 * sum so each sample is O(1). Output = sum / count (count ramps 1..N at startup).
			 */
			if (state->moving_avg_count < state->moving_avg_taps) {
				state->moving_avg_count++;
			} else {
				state->moving_avg_sum -= state->moving_avg_history[state->moving_avg_index];
			}
			state->moving_avg_history[state->moving_avg_index] = (uint16_t)input_sample;
			state->moving_avg_sum += input_sample;

			state->moving_avg_index++;
			if (state->moving_avg_index >= state->moving_avg_taps) {
				state->moving_avg_index = 0;
			}

			*output_sample = (uint16_t)(state->moving_avg_sum / state->moving_avg_count);
			break;
		}
		case EFFECT_BYPASS:
		default:
			*output_sample = (uint16_t)input_sample;
			break;
	}
}
