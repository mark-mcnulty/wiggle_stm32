/*
 * seven_seg.c
 *
 *  Created on: Jun 18, 2025
 *      Author: mark
 */
#include "seven_seg.h"
#include "main.h"
#include "stm32f1xx_hal.h"

const uint16_t digitKey[16] = {
	0b1011111100000000, // 0 -> segments a,b,c,d,e,f
	0b1000011000000000, // 1
	0b1101101100000000, // 2
	0b1100111100000000, // 3
	0b1110011000000000, // 4
	0b1110110100000000, // 5
	0b1111110100000000, // 6
	0b1000011100000000, // 7
	0b1111111100000000, // 8
	0b1110111100000000, // 9
	0b0000000000000000, // None
	0b0000000000000000, // None
	0b0000000000000000, // None
	0b0000000000000000, // None
	0b0000000000000000, // None
	0b0000000000000000, // None
};

void seven_seg_init(struct Seven_Seg *self) {
	// initalize the struct to have values blank right now I'm having that be a 0xF
	self->value = 0xFF;

	// set digit 0 to being the one on right now
	self->activeDigit = 0x0;
	self->activeDigitValue = self->value & 0xF;

	// set the clock count refresh
	self->refresh_clock_counts = 333;

	// toggle one of the pins on
	HAL_GPIO_TogglePin (SEVEN_SEG_DIG_0_GPIO_Port, SEVEN_SEG_DIG_0_Pin);
}
/**
 * @brief  Function called during timer event. It will set
 * 		   the seven segment display to the next active
 * 		   digits value and swap what digit is on. This
 * 		   should happen about every 60 Hz per digit.
 * @param  None
 * @retval None
 */
void seven_seg_update(struct Seven_Seg *self) {
	/* toggle digit gpio pin */
	HAL_GPIO_TogglePin (SEVEN_SEG_DIG_0_GPIO_Port, SEVEN_SEG_DIG_0_Pin);
	HAL_GPIO_TogglePin (SEVEN_SEG_DIG_1_GPIO_Port, SEVEN_SEG_DIG_1_Pin);

	if (self->activeDigit == 0x1){
		// switch the active digit to 0 and set digit 0s value
		self->activeDigitValue = self->value & 0xF;
		send_segment_value_spi(self);

		// change what the struct says the active digit is
		self->activeDigit = 0x0;

	} else if (self->activeDigit == 0x0) {
		// spi write to set digit
		self->activeDigitValue = (self->value >> 4) & 0xF;
		send_segment_value_spi(self);

		// change what the struct says the active digit is
		self->activeDigit = 0x1;
	}
}

/**
 * @brief  This will set the seven segment displays current
 * 		   value.
 * @param  self: The struct holding the "classes" attributes
 * @param  digit: The digit to be set. either 0 or 1.
 * @param  value: The value to be set
 * @retval None
 */
void seven_seg_set_value(struct Seven_Seg *self, uint8_t value) {
	self->value = value;
	if (self->activeDigit == 0x0) {
		self->activeDigitValue = self->value & 0xF;
	} else if (self->activeDigit == 0x1){
		self->activeDigitValue = (self->value >> 4) & 0xF;
	}

	return;
}
uint8_t seven_seg_get_value(struct Seven_Seg *self){
	return self->value;
}

void send_segment_value_spi(struct Seven_Seg *self){
	// send the data
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&digitKey[self->activeDigitValue], 1, HAL_MAX_DELAY);

	// latch the data
	HAL_GPIO_TogglePin (SPI_LAT_GPIO_Port, SPI_LAT_Pin);
	HAL_GPIO_TogglePin (SPI_LAT_GPIO_Port, SPI_LAT_Pin);

	return;
}

