/*
 * mixer.c
 *
 *  Created on: Nov 14, 2025
 *      Author: mark
 */

#include "mixer.h"

uint16_t mix_audio(const uint16_t *sources, uint8_t num_sources) {
	if (num_sources == 0) return 0;

	// can really only mix 4 sources with this size
	uint64_t summer = 0;

	// sum the input audio
	for (int i=0; i<num_sources; i++) summer += sources[i];

	return (uint16_t)(summer / num_sources);
}
