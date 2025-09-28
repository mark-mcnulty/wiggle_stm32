/*
 * note.h
 *
 *  Created on: Sep 27, 2025
 *      Author: mark
 */

#ifndef INC_NOTE_H_
#define INC_NOTE_H_

#include "stm32f1xx_hal.h"
#include <stdint.h>

#define TONE_LUT_SIZE 1024

typedef struct {
    const uint16_t *lut;     // Pointer to sine LUT
    uint32_t phase_acc;      // Phase accumulator
    uint32_t phase_step;     // Phase increment per tick
    uint32_t sample_rate;    // Timer ISR rate (Hz)
    uint32_t freq;           // Output frequency
} tone_t;

void tone_init(tone_t *t, const uint16_t *lut, uint32_t sample_rate);
void tone_set_freq(tone_t *t, uint32_t freq);
uint16_t tone_next_sample(tone_t *t);

#endif /* INC_NOTE_H_ */
