/*
 * synth.c
 *
 *  Created on: May 21, 2024
 *      Author: Implacable
 */

#include "stdbool.h"
#include "spi.h"

#include "synth.h"
#include "gpioxpndr.h"
#include "system.h"

extern System sys;
Synth SynthParameters;// = {10, 3.14};

uint16_t gpio_reg = 0x0000;

void Digital_Pot_Wiper_Set(int pot_address, int wiper_code, int invert){

	uint8_t spi_buf[1];
	GPIO_TypeDef* CSn_PORT;
	uint16_t CSn_PIN;

	//if invert bit is set, the command value is inverted
	spi_buf[0] = invert ? ~(wiper_code) : wiper_code;

    switch(pot_address) {
        case 0:
            CSn_PORT = CS0_PORT;
            CSn_PIN =  CS0_PIN;
            break;
        case 1:
            CSn_PORT = CS1_PORT;
            CSn_PIN =  CS1_PIN;
            break;
        case 2:
            CSn_PORT = CS2_PORT;
            CSn_PIN =  CS2_PIN;
            break;
        case 3:
            CSn_PORT = CS3_PORT;
            CSn_PIN =  CS3_PIN;
            break;
        case 4:
            CSn_PORT = CS4_PORT;
            CSn_PIN =  CS4_PIN;
            break;
        case 5:
            CSn_PORT = CS5_PORT;
            CSn_PIN =  CS5_PIN;
            break;
        case 6:
            CSn_PORT = CS6_PORT;
            CSn_PIN =  CS6_PIN;
            break;
        case 7:
            CSn_PORT = CS7_PORT;
            CSn_PIN =  CS7_PIN;
            break;
        case 8:
            CSn_PORT = CS8_PORT;
            CSn_PIN =  CS8_PIN;
            break;
        case 9:
            CSn_PORT = CS9_PORT;
            CSn_PIN =  CS9_PIN;
            break;
        case 10:
            CSn_PORT = CS10_PORT;
            CSn_PIN =  CS10_PIN;
            break;
        default:
            // do nothing
            break;
    }

	//HAL_GPIO_WritePin(TP0_PORT, TP0_PIN, RESET); // test point
	HAL_GPIO_WritePin(CSn_PORT, CSn_PIN, RESET);

	HAL_SPI_Transmit(&hspi3, (uint8_t*)spi_buf, 1, 10);

	//HAL_GPIO_WritePin(TP0_PORT, TP0_PIN, SET); // test point
	HAL_GPIO_WritePin(CSn_PORT, CSn_PIN, SET);

	return;
}

void Synth_Reset_Initialize(){

	HAL_GPIO_WritePin(TP0_PORT, TP0_PIN, SET); // test point

	// initialize all chip select lines
	HAL_GPIO_WritePin(CS0_PORT,  CS0_PIN,  SET);
	HAL_GPIO_WritePin(CS1_PORT,  CS1_PIN,  SET);
	HAL_GPIO_WritePin(CS2_PORT,  CS2_PIN,  SET);
	HAL_GPIO_WritePin(CS3_PORT,  CS3_PIN,  SET);
	HAL_GPIO_WritePin(CS4_PORT,  CS4_PIN,  SET);
	HAL_GPIO_WritePin(CS5_PORT,  CS5_PIN,  SET);
	HAL_GPIO_WritePin(CS6_PORT,  CS6_PIN,  SET);
	HAL_GPIO_WritePin(CS7_PORT,  CS7_PIN,  SET);
	HAL_GPIO_WritePin(CS8_PORT,  CS8_PIN,  SET);
	HAL_GPIO_WritePin(CS9_PORT,  CS9_PIN,  SET);
	HAL_GPIO_WritePin(CS10_PORT, CS10_PIN, SET);

	// set gate low
	Gate_Control(0);

	Synth_Initialize_Preset();

	return;
}

int Synth_Initialize_Preset(){

	// system settings
	SynthParameters.vca_mod_source = 1;
	SynthParameters.vca_bypass = 1;
	SynthParameters.vcf_mod_source = 1;
	SynthParameters.fm1_enable = 0;
	SynthParameters.fm2_enable = 0;
	SynthParameters.env_invert_mode = 0;
	SynthParameters.env_loop_mode = 0;
	SynthParameters.lfo_waveform = 1;

	// waveform synthesis settings
	SynthParameters.oscillator1 = 4;
	SynthParameters.oscillator2 = 4;
	SynthParameters.note_offset1 = 12;
	SynthParameters.note_offset2 = 12;
	SynthParameters.detune_osc1 = 100;
	SynthParameters.detune_osc2 = 100;
	SynthParameters.fm_intensity1 = 0.5;
	SynthParameters.fm_intensity2 = 0.5;
	SynthParameters.fm_harmonic1 = 1.0;
	SynthParameters.fm_harmonic2 = 1.0;
	SynthParameters.duty_cycle1 = 0.5;
	SynthParameters.duty_cycle2 = 0.5;
	SynthParameters.volume_osc1 = 100;
	SynthParameters.volume_osc2 = 100;

	// mixer output level settings
	SynthParameters.dac_mixer_level = 127;
	SynthParameters.filter_out_level = 127;

	// filter modulation source settings
	SynthParameters.vcf_cv_en = 0;
	SynthParameters.vcf_cv_sel = 0;

	// filter digital pot settings
	SynthParameters.vcf_cv_intensity = 127;
	SynthParameters.vcf_cutoff = 127;
	SynthParameters.vcf_resonance = 127;

	// LFO output settings
	SynthParameters.lfo_output_en = 0;
	SynthParameters.lfo_output_waveform = 0;

	// LFO output frequency setting
	SynthParameters.lfo_frequency = 127;

	// VCA modulation source settings
	SynthParameters.vca_cv_en = 0;
	SynthParameters.vca_cv_sel = 0;

	// VCA output enable/bypass settings
	SynthParameters.vca_output_en = 1;
	SynthParameters.vca_output_sel = 1;

	// VCA modulation source intensity and offset settings
	SynthParameters.vca_cv_intensity = 127;
	SynthParameters.vca_offset = 127;

	// envelope output settings
	SynthParameters.env_en = 1;
	SynthParameters.env_loop = 0;
	SynthParameters.env_invert = 0;

	// envelope timing settings
	//SynthParameters.env_attack_rate = 0;
	SynthParameters.env_attack_rate = 127;
	SynthParameters.env_release_rate = 127;
	//SynthParameters.env_release_rate = 255;
	SynthParameters.env_decay_rate = 127;
	//SynthParameters.env_decay_rate = 0;

	Mixer_Digital_Pot_Control();

	Filter_Mod_Source_Control();
	Filter_Digital_Pot_Control();

	LFO_Output_Control();
	LFO_Digital_Pot_Control();

	VCA_Mod_Source_Control();
	VCA_Bypass_Switch_Control();
	VCA_Digital_Pot_Control();

	ENV_Mode_Control();
	ENV_Digital_Pot_Control();

	return 0;
}

void Gate_Control(int gate_state){

	gpio_reg = GPIO_State_Change(GATE_PORT, gpio_reg, GATE_PIN, gate_state);

	return;
}

void Mixer_Digital_Pot_Control(){

    Digital_Pot_Wiper_Set(0, SynthParameters.dac_mixer_level, 0);  // set DAC mixer output level
    Digital_Pot_Wiper_Set(1, SynthParameters.filter_out_level, 0); // set filter output level

    return;
}

void VCA_Mod_Source_Control() {

    // control CV enable/disable (active low)
    gpio_reg = GPIO_State_Change(VCA_CV_SW_PORT, gpio_reg, VCA_CV_SW_PIN, SynthParameters.vca_cv_en == 0);

    // select modulation source
    gpio_reg = GPIO_State_Change(VCA_SEL_PORT, gpio_reg, VCA_SEL_PIN, SynthParameters.vca_cv_sel == 1);

    return;
}

void VCA_Bypass_Switch_Control() {

    // control VCA bypass switch enable/disable (active low)
    gpio_reg = GPIO_State_Change(VCA_BYP_SW_PORT, gpio_reg, VCA_BYP_SW_PIN, SynthParameters.vca_output_en == 0);

    // update output source selection; true = bypass VCA, false = output through VCA
    gpio_reg = GPIO_State_Change(VCA_BYP_PORT, gpio_reg, VCA_BYP_PIN, SynthParameters.vca_output_sel == 1);

    return;
}

void VCA_Digital_Pot_Control(){

    Digital_Pot_Wiper_Set(6, SynthParameters.vca_cv_intensity, 0); // set intensity of mod source
    Digital_Pot_Wiper_Set(7, SynthParameters.vca_offset, 0);       // set VCA offset

    return;
}

void Filter_Mod_Source_Control() {

    // control CV enable/disable (active low)
    gpio_reg = GPIO_State_Change(VCF_CV_SW_PORT, gpio_reg, VCF_CV_SW_PIN, SynthParameters.vcf_cv_en == 0);

    // select modulation source; true = LFO, false = envelope
    gpio_reg = GPIO_State_Change(VCF_SEL_PORT, gpio_reg, VCF_SEL_PIN, SynthParameters.vcf_cv_sel == 1);

    return;
}


void Filter_Digital_Pot_Control(){

    Digital_Pot_Wiper_Set(4, SynthParameters.vcf_cv_intensity, 0); // set intensity of mod source
    Digital_Pot_Wiper_Set(3, SynthParameters.vcf_cutoff, 0);       // set filter cutoff
    Digital_Pot_Wiper_Set(2, SynthParameters.vcf_resonance, 0);    // set resonance intensity

    return;
}

void LFO_Output_Control() {

    // control LFO output enable/disable (active low)
    gpio_reg = GPIO_State_Change(LFO_EN_PORT, gpio_reg, LFO_EN_PIN, SynthParameters.lfo_output_en == 0);

    // select LFO output waveform; true = square wave, false = triangle wave
    gpio_reg = GPIO_State_Change(LFO_SEL_PORT, gpio_reg, LFO_SEL_PIN, SynthParameters.lfo_output_waveform == 1);

    return;
}

void LFO_Digital_Pot_Control(){

    Digital_Pot_Wiper_Set(5, SynthParameters.lfo_frequency, 0); // set LFO frequency

    return;
}

void ENV_Mode_Control() {

    // control envelope enable/disable (active low)
    gpio_reg = GPIO_State_Change(ENV_EN_PORT, gpio_reg, ENV_EN_PIN, SynthParameters.env_en == 0);

    // select envelope loop/trigger mode; true = trigger mode, false = loop mode
    gpio_reg = GPIO_State_Change(ENV_LP_PORT, gpio_reg, ENV_LP_PIN, SynthParameters.env_loop == 0);

    // select envelope inversion mode; true = inverter output, false = non-inverted output
    gpio_reg = GPIO_State_Change(ENV_POL_PORT, gpio_reg, ENV_POL_PIN, SynthParameters.env_invert == 0);

    return;
}

void ENV_Digital_Pot_Control(){

	HAL_GPIO_WritePin(TP0_PORT, TP0_PIN, SET); // test point

    Digital_Pot_Wiper_Set(8, SynthParameters.env_attack_rate, 0);    // set envelope attack rate
    Digital_Pot_Wiper_Set(9, SynthParameters.env_release_rate, 0);   // set envelop release rate
    Digital_Pot_Wiper_Set(10, SynthParameters.env_decay_rate, 0);    // set decay rate and sustain level

    return;
}





