/*
 * adsr.h
 *
 *  Created on: Nov 14, 2025
 *      Author: mark
 */

#ifndef INC_ADSR_H_
#define INC_ADSR_H_

typedef enum {
	ADSR_IDLE,
	ADSR_ATTACK,
	ADSR_DECAY,
	ADSR_SUSTAIN,
	ADSR_RELEASE
} adsr_state_t;

struct ADSR {
	adsr_state_t state;

	float attack_time;
	float decay_time;
	float sustain_level;
	float release_time;

	float current_value;
	float attack_step;
	float decay_step;
	float release_step;
};

void adsr_init(volatile struct ADSR *self);

#endif /* INC_ADSR_H_ */
