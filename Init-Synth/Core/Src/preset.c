/*
 * preset.c
 *
 *  Created on: Dec 9, 2024
 *      Author: Implacable
 */

#include "usbd_cdc_if.h"
#include "stdint.h"
#include "string.h"
#include "usart.h"

#include "eeprom.h"
#include "preset.h"
#include "system.h"
#include "synth.h"

int Synth_Settings_Readback(){

	int error_check = 1;

	uint8_t return_data[48];

	// copy synth parameters into buffer
    memcpy(return_data, &SynthParameters, sizeof(Synth));

    // copy remaining system settings
    return_data[44] = sys.serial_cmd_echo;
    return_data[45] = sys.midi_tx_pol;
    return_data[46] = sys.midi_tx_en;

    return_data[47] = 0xAA;

    error_check = CDC_Transmit_FS(return_data, 48); // transmit current preset values

    if (sys.midi_tx_en == 1) { // send return message if enabled
        error_check = HAL_UART_Transmit(&huart2, return_data, 48, 1000);
    }

    return error_check;
}

int Preset_Init(int preset_slot){

	int error_check = 1;
	uint8_t page_buf[8];
    uint8_t checksum;

	// default parameter array
	uint8_t init_preset[48] = {
	    VCA_MOD_SOURCE_DEFAULT,
	    VCA_BYPASS_DEFAULT,
	    VCF_MOD_SOURCE_DEFAULT,
	    FM1_ENABLE_DEFAULT,
	    FM2_ENABLE_DEFAULT,
	    ENV_INVERT_MODE_DEFAULT,
	    ENV_LOOP_MODE_DEFAULT,
	    LFO_WAVEFORM_DEFAULT,
	    OSCILLATOR1_DEFAULT,
	    OSCILLATOR2_DEFAULT,
	    NOTE_OFFSET1_DEFAULT,
	    NOTE_OFFSET2_DEFAULT,
	    DETUNE_OSC1_DEFAULT,
	    DETUNE_OSC2_DEFAULT,
	    FM_INTENSITY1_DEFAULT,
	    FM_INTENSITY2_DEFAULT,
	    FM_HARMONIC1_DEFAULT,
	    FM_HARMONIC2_DEFAULT,
	    DUTY_CYCLE1_DEFAULT,
	    DUTY_CYCLE2_DEFAULT,
	    VOLUME_OSC1_DEFAULT,
	    VOLUME_OSC2_DEFAULT,
	    DAC_MIXER_LEVEL_DEFAULT,
	    FILTER_OUT_LEVEL_DEFAULT,
	    VCF_CV_EN_DEFAULT,
	    VCF_CV_SEL_DEFAULT,
	    VCF_CV_INTENSITY_DEFAULT,
	    VCF_CUTOFF_DEFAULT,
	    VCF_RESONANCE_DEFAULT,
	    LFO_OUTPUT_EN_DEFAULT,
	    LFO_OUTPUT_WAVEFORM_DEFAULT,
	    LFO_FREQUENCY_DEFAULT,
	    VCA_CV_EN_DEFAULT,
	    VCA_CV_SEL_DEFAULT,
	    VCA_OUTPUT_EN_DEFAULT,
	    VCA_OUTPUT_SEL_DEFAULT,
	    VCA_CV_INTENSITY_DEFAULT,
	    VCA_OFFSET_DEFAULT,
	    ENV_EN_DEFAULT,
	    ENV_LOOP_DEFAULT,
	    ENV_INVERT_DEFAULT,
	    ENV_ATTACK_RATE_DEFAULT,
	    ENV_RELEASE_RATE_DEFAULT,
	    ENV_DECAY_RATE_DEFAULT,
	    SERIAL_CMD_ECHO_DEFAULT,
	    MIDI_TX_POL_DEFAULT,
	    MIDI_TX_EN_DEFAULT,
		0
	};


    checksum = XOR_Checksum(init_preset, 48);
    init_preset[47] = checksum;

	for(int page = 0; page < 6; page++){

		memcpy(page_buf, &init_preset[8 * page], 8); // copy one page of data from buffer
		error_check = EEPROM_Write_Page(PAGE_OFFSET + (preset_slot * 6) + page, page_buf);

		if(error_check != 0){
			return 1;
		}

	}

	// read checksum for EEPROM slot used and verify output
	if(checksum == Readback_Checksum(preset_slot)){
		error_check = 0;
	}

    return error_check;
}

int Preset_Clear(){

	for(int i = 0; i < 5; i++){

		if(Preset_Init(i) != 0){
			return 1;
		}

	}

	return 0;
}

// load the values into their destination peripherals
void Preset_Load(){

	Mixer_Digital_Pot_Control();

	Filter_Mod_Source_Control();
	Filter_Digital_Pot_Control();

	LFO_Output_Control();
	LFO_Digital_Pot_Control();

	VCA_Mod_Source_Control();
	VCA_Bypass_Switch_Control();
	VCA_Digital_Pot_Control();

	Envelope_Mode_Control();
	Envelope_Digital_Pot_Control();

	return;
}

int Preset_Write(int preset_slot){

	int error_check = 1;
	uint8_t page_buf[8];
	uint8_t preset_buf[48];
    uint8_t checksum;

	// copy synth parameters into buffer
    memcpy(preset_buf, &SynthParameters, sizeof(Synth));

    // copy remaining system settings
    preset_buf[44] = sys.serial_cmd_echo;
    preset_buf[45] = sys.midi_tx_pol;
    preset_buf[46] = sys.midi_tx_en;

    checksum = XOR_Checksum(preset_buf, 48);
    preset_buf[47] = checksum;

    error_check = EEPROM_Write_Byte(preset_slot, &checksum);

	for(int page = 0; page < 6; page++){

		memcpy(page_buf, &preset_buf[8 * page], 8); // copy one page of data from buffer
		error_check = EEPROM_Write_Page(PAGE_OFFSET + (preset_slot * 6) + page, page_buf);

		if(error_check != 0){
			return 1;
		}

	}

	// read checksum for EEPROM slot used and verify output
	if(checksum == Readback_Checksum(preset_slot)){
		error_check = 0;
	}

    return error_check;
}

int Preset_Read(int preset_slot){

    uint8_t page_buf[8];
    uint8_t preset_buf[48];
    uint8_t checksum;

    // readback stored checksum
    if(EEPROM_Read_Byte(preset_slot, &checksum) != 0){
    	return 1;
    }

    // read all pages back into the buffer
    for (int page = 0; page < 6; page++) {

        if (EEPROM_Read_Page(PAGE_OFFSET + (preset_slot * 6) + page, page_buf) != 0) {
            return 1; // error during EEPROM read
        }

        memcpy(&preset_buf[8 * page], page_buf, 8); // copy page data into buffer
    }

    // validate the checksum
    if (checksum != XOR_Checksum(preset_buf, 47)) {
        return 1; // checksum mismatch
    }

    // copy buffer data into synth parameters and system structures
    memcpy(&SynthParameters, preset_buf, sizeof(Synth));
    sys.serial_cmd_echo = preset_buf[44];
    sys.midi_tx_pol = preset_buf[45];
    sys.midi_tx_en = preset_buf[46];

    return 0;
}


uint8_t XOR_Checksum(uint8_t *data, int length){

    uint8_t checksum = 0x00;

    for (int i = 0; i < length; i++) {
        checksum ^= data[i]; // XOR each byte
    }

    return checksum;
}
