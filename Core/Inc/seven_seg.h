/*
 * seven_seg.h
 *
 *  Created on: Jun 18, 2025
 *      Author: mark
 */

#ifndef SEVEN_SEG_H_
#define SEVEN_SEG_H_

#include "stm32f1xx_hal.h"

extern SPI_HandleTypeDef hspi1;

extern const uint16_t digitKey[16];

struct Seven_Seg {
	uint8_t value;
	uint8_t activeDigitValue;
	uint8_t activeDigit;  				// this holds the currently active digit. Digit 1 or Digit 0
	uint32_t refresh_clock_counts;
};

// define the functions below

// initializer
void seven_seg_init(struct Seven_Seg *self);

// update function called in ISR
void seven_seg_update(struct Seven_Seg *self);

// set the seven segment display value
void seven_seg_set_value(struct Seven_Seg *self, uint8_t value);

// sends the seven segment value to the
void send_segment_value_spi(struct Seven_Seg *self);

#endif /* SEVEN_SEG_H_ */
