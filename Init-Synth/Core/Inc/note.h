/*
 * note.h
 *
 *  Created on: May 13, 2024
 *      Author: Implacable
 */

#ifndef INC_NOTE_H_
#define INC_NOTE_H_

#define sample_rate 32000 // Number of samples per second
#define TAU (2.0 * M_PI)
#define NUM_NOTES 88 // Number of keys on a piano
#define A4_FREQ 440.0 // Frequency of A4 (in Hz)

extern int gate;

float Calculate_Frequency(float note_number);

int16_t Synthesize_Sine_Wave_FM(float t, float freq, float fm_intensity, float fm_harmonic);
int16_t Synthesize_Sine_Wave(float t, float freq);
int16_t Synthesize_Pulse_Wave(float t, float freq, float duty_cycle);
int16_t Synthesize_Triangle_Wave(float t, float freq);
int16_t Synthesize_Sawtooth_Wave(float t, float freq);

int Waveform_Synthesis_Handler(int midi_note, int velocity);

#endif /* INC_NOTE_H_ */
