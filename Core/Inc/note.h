/*
 * note.h
 *
 *  Created on: Nov 10, 2025
 *      Author: mark
 */

#ifndef INC_NOTE_H_
#define INC_NOTE_H_

#include <stdint.h>

#define OCTAVE_MIN 0
#define OCTAVE_MAX 8
#define NOTE_COUNT 12

// Enumerate the 12 semitones in an octave
typedef enum {
    NOTE_C,
    NOTE_CS,
    NOTE_D,
    NOTE_DS,
    NOTE_E,
    NOTE_F,
    NOTE_FS,
    NOTE_G,
    NOTE_GS,
    NOTE_A,
    NOTE_AS,
    NOTE_B
} NoteName;

// make a function to play a note


// 2D array: [note][octave]
extern const float NOTES[NOTE_COUNT][OCTAVE_MAX + 1];

#endif /* INC_NOTE_H_ */
