/*
 * EEPROM.h
 *
 *  Created on: May 29, 2024
 *      Author: Implacable
 */

#ifndef EEPROM_EEPROM_H_
#define EEPROM_EEPROM_H_

extern I2C_HandleTypeDef hi2c3;
#define EEPROM_I2C &hi2c3

// EEPROM ADDRESS (8bits)
#define EEPROM_ADDR 0xA0

// Define the Page Size and number of pages
#define PAGE_SIZE 8     // in Bytes
#define PAGE_NUM  256   // number of pages

// system settings
extern int oscillator1;
extern int oscillator2;
extern int note_offset1;
extern int note_offset2;
extern float detune_osc1;
extern float detune_osc2;
extern float fm_intensity1;
extern float fm_intensity2;
extern float fm_harmonic1;
extern float fm_harmonic2;
extern float duty_cycle1;
extern float duty_cycle2;
extern int volume_osc1;
extern int volume_osc2;

extern int vca_mod_source;
extern int vcf_mod_source;

// mixer output level settings
extern int dac_mixer_level;
extern int filter_out_level;

// filter modulation source settings
extern int vcf_cv_en;
extern int vcf_cv_sel;

// filter digital pot settings
extern int vcf_cv_intensity;
extern int vcf_cutoff;
extern int vcf_resonance;

// LFO output settings
extern int lfo_output_en;
extern int lfo_output_waveform;

// LFO output frequency setting
extern int lfo_frequency;

// VCA modulation source settings
extern int vca_cv_en;
extern int vca_cv_sel;

// VCA output enable/bypass settings
extern int vca_output_en;
extern int vca_output_sel;

// VCA modulation source intensity and offset settings
extern int vca_cv_intensity;
extern int vca_offset;

// envelope output settings
extern int env_en;
extern int env_loop;
extern int env_invert;

// envelope timing settings
extern int env_attack_rate;
extern int env_release_rate;
extern int env_decay_rate;

#include <stdint.h>

typedef struct {
    uint8_t header[4]; // 'P', 'R', 'E', and address
    uint8_t oscillator1;
    uint8_t oscillator2;
    uint8_t note_offset1;
    uint8_t note_offset2;
    uint8_t volume_osc1;
    uint8_t volume_osc2;
    uint8_t dac_mixer_level;
    uint8_t filter_out_level;
    uint8_t vcf_cv_en;
    uint8_t vcf_cv_sel;
    uint8_t vcf_cv_intensity;
    uint8_t vcf_cutoff;
    uint8_t vcf_resonance;
    uint8_t lfo_output_en;
    uint8_t lfo_output_waveform;
    uint8_t lfo_frequency;
    uint8_t vca_cv_en;
    uint8_t vca_cv_sel;
    uint8_t vca_output_en;
    uint8_t vca_output_sel;
    uint8_t vca_cv_intensity;
    uint8_t vca_offset;
    uint8_t env_en;
    uint8_t env_loop;
    uint8_t env_invert;
    uint8_t env_attack_rate;
    uint8_t env_release_rate;
    uint8_t env_decay_rate;
    uint8_t detune_osc1[4];
    uint8_t detune_osc2[4];
    uint8_t fm_intensity1[4];
    // Add other float fields as necessary
} Preset;



void Convert_Float_to_Binary(float input);
void Convert_Binary_to_Float();

#endif /* EEPROM_EEPROM_H_ */
