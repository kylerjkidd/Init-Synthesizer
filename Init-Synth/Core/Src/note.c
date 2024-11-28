/*
 * note.c
 *
 *  Created on: May 13, 2024
 *      Author: Implacable
 */

#define ARM_MATH_CM4

#include "dma.h"
#include "stdint.h"
#include "arm_math.h"
#include "math.h"
#include "i2s.h"

#include "note.h"
#include "synth.h"

//#define TAU (2.0 * M_PI)
//#define NUM_NOTES 88 // Number of keys on a piano
//#define A4_FREQ 440.0 // Frequency of A4 (in Hz)

//extern int gate;

// Function to calculate the frequency of a given note
float Calculate_Frequency(float note_number) {
    return A4_FREQ * powf(2.0, (note_number - 69.0) / 12.0); // MIDI note number for A4 is 69
}

int16_t Synthesize_Sine_Wave_FM(float t, float freq, float fm_intensity, float fm_harmonic){

	int16_t sample;
	sample = 32767*arm_sin_f32( (freq * TAU * t) + (fm_intensity*arm_sin_f32(freq*fm_harmonic*TAU*t)) ); // sine wave formula

	return sample;
}

int16_t Synthesize_Sine_Wave(float t, float freq){

	int16_t sample;
	sample = 32767*arm_sin_f32(freq * TAU * t); // sine wave formula
	//sample = 32767*sin(freq * TAU * t); // sine wave formula

	return sample;
}

int16_t Synthesize_Pulse_Wave(float t, float freq, float duty_cycle){

	int16_t sample;
	float phase = fmodf(t * freq, 1.0); // Phase of the wave
	sample = 32767*((phase < duty_cycle) ? 1.0 : -1.0); // Square wave formula with adjustable duty cycle

	return sample;
}

int16_t Synthesize_Triangle_Wave(float t, float freq){

	int16_t sample;
    float phase = fmodf(t * freq, 1.0); // Phase of the wave

		// Triangle wave formula
		if (phase < 0.25)
			sample = 32767*(phase * 4.0);
		else if (phase < 0.75)
			sample = 32767*(2.0 - (phase * 4.0));
		else
			sample = 32767*((phase - 1.0) * 4.0);

	return sample;
}

int16_t Synthesize_Sawtooth_Wave(float t, float freq){

	int16_t sample;
	sample = 32767*fmodf(t * freq, 1.0) * 2.0 - 32767; // Sawtooth wave formula

	return sample;
}

int Waveform_Synthesis_Handler(int midi_note, int velocity){

    HAL_StatusTypeDef res;
    //int16_t signal[46876];
    int16_t signal[sample_rate];
    int nsamples = sample_rate;

    float freq1 = Calculate_Frequency(midi_note + (SynthParameters.note_offset1 - 12));
    float freq2 = Calculate_Frequency(midi_note + (SynthParameters.note_offset2 - 12));
    //int nsamples = sizeof(signal) / sizeof(signal[0]);

    // even indices (signal[i]) represent the left channel of the DAC (Oscillator 1)
    // odd indices  (signal[i+1]) represent the right channel of the DAC (Oscillator 2)
    // this allows the production of two distinctly programmable waveforms with one audio DAC

    int i = 0;
    while(i < nsamples) {
        float t = ((float)i/2.0)/((float)nsamples);

        // oscillator 1 handler
        if(SynthParameters.oscillator1 == 1){
        	signal[i] = Synthesize_Sine_Wave(t, freq1);
        	//signal[i] = Synthesize_Sine_Wave_FM(t, freq1, fm_intensity1, fm_harmonic1);
        }
        else if(SynthParameters.oscillator1 == 2){
        	signal[i] = Synthesize_Pulse_Wave(t, freq1, SynthParameters.duty_cycle1);
        }
        else if(SynthParameters.oscillator1 == 3){
        	signal[i] = Synthesize_Triangle_Wave(t, freq1);
        }
        else if(SynthParameters.oscillator1 == 4){
        	signal[i] = Synthesize_Sawtooth_Wave(t, freq1);
        	//signal[i] = Synthesize_Sawtooth_Wave(t, 440);

        }
        else return 1;

        //signal[i] = signal[i]*(velocity/127); // adjust intensity per velocity

        // oscillator 2 handler
        if(SynthParameters.oscillator2 == 1){
        	signal[i+1] = Synthesize_Sine_Wave(t, freq2);
        	//signal[i+1] = Synthesize_Sine_Wave_FM(t, freq2, fm_intensity2, fm_harmonic2);
        }
        else if(SynthParameters.oscillator2 == 2){
        	signal[i+1] = Synthesize_Pulse_Wave(t, freq2, SynthParameters.duty_cycle2);
        }
        else if(SynthParameters.oscillator2 == 3){
        	signal[i+1] = Synthesize_Triangle_Wave(t, freq2);
        }
        else if(SynthParameters.oscillator2 == 4){
        	signal[i+1] = Synthesize_Sawtooth_Wave(t, freq2);
        	//signal[i+1] = Synthesize_Sawtooth_Wave(t, 440);
        }
        else return 1;

        //signal[i+1] = signal[i+1]*(velocity/127); // adjust intensity per velocity

        i += 2;
    }

    //while(gate == 1) {
    	while(1) {
    	res = HAL_I2S_Transmit(&hi2s2, (uint16_t*)signal, nsamples, 1000);
        if(res != HAL_OK) {
        	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);

            //UART_Printf("I2S - ERROR, res = %d!\r\n", res);
            break;
        }
    }

    return 0;

}


