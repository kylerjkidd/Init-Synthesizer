/*
 * synth.h
 *
 *  Created on: May 21, 2024
 *      Author: Implacable
 */

#ifndef SRC_SYNTH_H_
#define SRC_SYNTH_H_

// ===========================================================================================================
// default values for SynthParameters

// system settings
#define VCA_MOD_SOURCE_DEFAULT      1
#define VCA_BYPASS_DEFAULT          1
#define VCF_MOD_SOURCE_DEFAULT      1
#define FM1_ENABLE_DEFAULT          0
#define FM2_ENABLE_DEFAULT          0
#define ENV_INVERT_MODE_DEFAULT     0
#define ENV_LOOP_MODE_DEFAULT       0
#define LFO_WAVEFORM_DEFAULT        1

// waveform synthesis settings
#define OSCILLATOR1_DEFAULT         4
#define OSCILLATOR2_DEFAULT         4
#define NOTE_OFFSET1_DEFAULT        12
#define NOTE_OFFSET2_DEFAULT        12
#define DETUNE_OSC1_DEFAULT         100
#define DETUNE_OSC2_DEFAULT         100
#define FM_INTENSITY1_DEFAULT       127
#define FM_INTENSITY2_DEFAULT       127
#define FM_HARMONIC1_DEFAULT        127
#define FM_HARMONIC2_DEFAULT        127
#define DUTY_CYCLE1_DEFAULT         127
#define DUTY_CYCLE2_DEFAULT         127
#define VOLUME_OSC1_DEFAULT         100
#define VOLUME_OSC2_DEFAULT         100

// mixer output level settings
#define DAC_MIXER_LEVEL_DEFAULT     127
#define FILTER_OUT_LEVEL_DEFAULT    127

// filter modulation source settings
#define VCF_CV_EN_DEFAULT           0
#define VCF_CV_SEL_DEFAULT          0

// filter digital pot settings
#define VCF_CV_INTENSITY_DEFAULT    127
#define VCF_CUTOFF_DEFAULT          127
#define VCF_RESONANCE_DEFAULT       127

// LFO output settings
#define LFO_OUTPUT_EN_DEFAULT       0
#define LFO_OUTPUT_WAVEFORM_DEFAULT 0

// LFO output frequency setting
#define LFO_FREQUENCY_DEFAULT       127

// VCA modulation source settings
#define VCA_CV_EN_DEFAULT           0
#define VCA_CV_SEL_DEFAULT          0

// VCA output enable/bypass settings
#define VCA_OUTPUT_EN_DEFAULT       1
#define VCA_OUTPUT_SEL_DEFAULT      1

// VCA modulation source intensity and offset settings
#define VCA_CV_INTENSITY_DEFAULT    127
#define VCA_OFFSET_DEFAULT          127

// envelope output settings
#define ENV_EN_DEFAULT              1
#define ENV_LOOP_DEFAULT            0
#define ENV_INVERT_DEFAULT          0

// envelope timing settings
#define ENV_ATTACK_RATE_DEFAULT     127
#define ENV_RELEASE_RATE_DEFAULT    127
#define ENV_DECAY_RATE_DEFAULT      127

// ===========================================================================================================
// pin definitions

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

// ===========================================================================================================
// synth parameters struct declaration

#pragma pack(push, 1) // ensure no padding between struct members
typedef struct {

    // system settings
    uint8_t vca_mod_source;
    uint8_t vca_bypass;
    uint8_t vcf_mod_source;
    uint8_t fm1_enable;
    uint8_t fm2_enable;
    uint8_t env_invert_mode;
    uint8_t env_loop_mode;
    uint8_t lfo_waveform;

    // waveform synthesis settings
    uint8_t oscillator1;
    uint8_t oscillator2;
    uint8_t note_offset1;
    uint8_t note_offset2;
    uint8_t detune_osc1;
    uint8_t detune_osc2;
    uint8_t fm_intensity1;
    uint8_t fm_intensity2;
    uint8_t fm_harmonic1;
    uint8_t fm_harmonic2;
    uint8_t duty_cycle1;
    uint8_t duty_cycle2;
    uint8_t volume_osc1;
    uint8_t volume_osc2;

    // mixer output level settings
    uint8_t dac_mixer_level;
    uint8_t filter_out_level;

    // filter modulation source settings
    uint8_t vcf_cv_en;
    uint8_t vcf_cv_sel;

    // filter digital pot settings
    uint8_t vcf_cv_intensity;
    uint8_t vcf_cutoff;
    uint8_t vcf_resonance;

    // LFO output settings
    uint8_t lfo_output_en;
    uint8_t lfo_output_waveform;

    // LFO output frequency setting
    uint8_t lfo_frequency;

    // VCA modulation source settings
    uint8_t vca_cv_en;
    uint8_t vca_cv_sel;

    // VCA output enable/bypass settings
    uint8_t vca_output_en;
    uint8_t vca_output_sel;

    // VCA modulation source intensity and offset settings
    uint8_t vca_cv_intensity;
    uint8_t vca_offset;

    // envelope output settings
    uint8_t env_en;
    uint8_t env_loop;
    uint8_t env_invert;

    // envelope timing settings
    uint8_t env_attack_rate;
    uint8_t env_release_rate;
    uint8_t env_decay_rate;

} Synth;
#pragma pack(pop)


extern Synth SynthParameters;

// ===========================================================================================================
// function definitions

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

void Envelope_Mode_Control();
void Envelope_Digital_Pot_Control();

#endif /* SRC_SYNTH_H_ */
