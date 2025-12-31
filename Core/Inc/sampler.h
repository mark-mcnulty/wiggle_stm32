/*
 * sampler.h
 *
 *  Created on: Dec 12, 2025
 *      Author: mark
 */

#ifndef INC_SAMPLER_H_
#define INC_SAMPLER_H_


#include "stm32f1xx_hal.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include <math.h>
#include <stdlib.h>


struct Sample {
	// used to synthesize the sound
	uint16_t wav_LUT_sample[LUT_SIZE];
};


#endif /* INC_SAMPLER_H_ */
