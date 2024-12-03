/*
 * synth.h
 *
 *  Created on: May 21, 2024
 *      Author: Implacable
 */

#ifndef SRC_SYNTH_H_
#define SRC_SYNTH_H_

// gate signal
//#define GATE_PORT 0
//#define GATE_PIN 3

// toggle polarity of MIDI UART output
//#define MIDI_TX_POL_PORT 0
//#define MIDI_TX_POL_PIN 3

// enable transmit out of MIDI port
//#define MIDI_TX_EN_PORT 0
//#define MIDI_TX_EN_PIN 4

// gate signal
//#define GATE_PORT GPIOA
//#define GATE_PIN GPIO_PIN_8

// chip select for mixer 1
#define CS0_PORT GPIOA
#define CS0_PIN GPIO_PIN_5

// chip select for mixer 2
#define CS1_PORT GPIOB
#define CS1_PIN GPIO_PIN_1

// chip select for filter resonance
#define CS2_PORT GPIOD
#define CS2_PIN GPIO_PIN_2

// chip select for filter cutoff
#define CS3_PORT GPIOB
#define CS3_PIN GPIO_PIN_9

// chip select for filter modulation intensity
#define CS4_PORT GPIOB
#define CS4_PIN GPIO_PIN_6

// chip select for low frequency oscillator frequency
#define CS5_PORT GPIOC
#define CS5_PIN GPIO_PIN_5

// chip select for voltage controlled amplifier modulation intensity
#define CS6_PORT GPIOB
#define CS6_PIN GPIO_PIN_7

// chip select for voltage controlled amplifier offset level
#define CS7_PORT GPIOC
#define CS7_PIN GPIO_PIN_13

//// chip select for envelope attack threshold
//#define CS8_PORT GPIOC
//#define CS8_PIN GPIO_PIN_4
//
//// chip select for envelope release threshold
//#define CS9_PORT GPIOA
//#define CS9_PIN GPIO_PIN_7

// chip select for envelope attack threshold
#define CS8_PORT GPIOA
#define CS8_PIN GPIO_PIN_7

// chip select for envelope release threshold
#define CS9_PORT GPIOC
#define CS9_PIN GPIO_PIN_4

// chip select for envelope decay/sustain threshold
#define CS10_PORT GPIOB
#define CS10_PIN GPIO_PIN_0


// chip enable (active low) for filter mod source switch
#define VCF_CV_SW_PORT 1
#define VCF_CV_SW_PIN 2

// switch input for filter mod source
#define VCF_SEL_PORT 1
#define VCF_SEL_PIN 1

// chip enable (active low) for LFO output switch
#define LFO_EN_PORT 1
#define LFO_EN_PIN 7

// switch input for LFO output select
#define LFO_SEL_PORT 1
#define LFO_SEL_PIN 6

// chip enable (active low) for VCA mod source switch
#define VCA_CV_SW_PORT 0
#define VCA_CV_SW_PIN 7

// switch input for VCA mod source select
#define VCA_SEL_PORT 1
#define VCA_SEL_PIN 0

// chip enable (active low) for VCA bypass switch
#define VCA_BYP_SW_PORT 0
#define VCA_BYP_SW_PIN 6

// switch input for VCA bypass select
#define VCA_BYP_PORT 0
#define VCA_BYP_PIN 5

// chip enable (active low) for ENV switches
#define ENV_EN_PORT 1
#define ENV_EN_PIN 5

// switch input for ENV loop select
#define ENV_LP_PORT 1
#define ENV_LP_PIN 4

// switch input for ENV output polarity select
#define ENV_POL_PORT 1
#define ENV_POL_PIN 3


typedef struct {

	// system settings
	int vca_mod_source;
	int vca_bypass;
	int vcf_mod_source;
	int fm1_enable;
	int fm2_enable;
	int env_invert_mode;
	int env_loop_mode;
	int lfo_waveform;

	// waveform synthesis settings
	int oscillator1;
	int oscillator2;
	int note_offset1;
	int note_offset2;
	int detune_osc1;
	int detune_osc2;
	float fm_intensity1;
	float fm_intensity2;
	float fm_harmonic1;
	float fm_harmonic2;
	float duty_cycle1;
	float duty_cycle2;
	int volume_osc1;
	int volume_osc2;

	// mixer output level settings
	int dac_mixer_level;
	int filter_out_level;

	// filter modulation source settings
	int vcf_cv_en;
	int vcf_cv_sel;

	// filter digital pot settings
	int vcf_cv_intensity;
	int vcf_cutoff;
	int vcf_resonance;

	// LFO output settings
	int lfo_output_en;
	int lfo_output_waveform;

	// LFO output frequency setting
	int lfo_frequency;

	// VCA modulation source settings
	int vca_cv_en;
	int vca_cv_sel;

	// VCA output enable/bypass settings
	int vca_output_en;
	int vca_output_sel;

	// VCA modulation source intensity and offset settings
	int vca_cv_intensity;
	int vca_offset;

	// envelope output settings
	int env_en;
	int env_loop;
	int env_invert;

	// envelope timing settings
	int env_attack_rate;
	int env_release_rate;
	int env_decay_rate;

} Synth;

extern Synth SynthParameters;

//void Gate_Control(int gate_state);

void Digital_Pot_Wiper_Set(int pot_address, int wiper_code, int invert);
void Synth_Reset_Initialize();

int Synth_Initialize_Preset();

void Mixer_Digital_Pot_Control();

void Filter_Mod_Source_Control();
void Filter_Digital_Pot_Control();

void LFO_Output_Control();
void LFO_Digital_Pot_Control();

void VCA_Mod_Source_Control();
void VCA_Bypass_Switch_Control();
void VCA_Digital_Pot_Control();

void ENV_Mode_Control();
void ENV_Digital_Pot_Control();

#endif /* SRC_SYNTH_H_ */
