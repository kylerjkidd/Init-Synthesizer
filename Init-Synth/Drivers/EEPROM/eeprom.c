/*
 * EEPROM.c
 *
 *  Created on: May 29, 2024
 *      Author: Implacable
 */


#include "i2c.h"
#include "stdint.h"
#include "string.h"

#include "eeprom.h"
#include "system.h"
#include "synth.h"



int Preset_Init(int preset_slot){

	int error_check = 1;
	uint8_t page_buf[8];

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

    sys.checksum = XOR_Checksum(init_preset, 47);
    init_preset[47] = sys.checksum;

	for(int page = 0; page < 6; page++){

		memcpy(page_buf, &init_preset[8 * page], 8); // copy one page of data from buffer
		error_check = EEPROM_Write_Page(PAGE_OFFSET + (preset_slot * 6) + page, page_buf);

		if(error_check != 0){
			return 1;
		}

	}

	// read checksum for EEPROM slot used and verify output
	if(sys.checksum == Readback_Checksum(preset_slot)){
		error_check = 0;
	}

    return error_check;
}

int Preset_Write(int preset_slot){

	int error_check = 1;
	uint8_t page_buf[8];
	uint8_t preset_buf[48];

	// copy synth parameters into buffer
    memcpy(preset_buf, &SynthParameters, sizeof(Synth));

    // copy remaining system settings
    preset_buf[44] = sys.serial_cmd_echo;
    preset_buf[45] = sys.midi_tx_pol;
    preset_buf[46] = sys.midi_tx_en;

    sys.checksum = XOR_Checksum(preset_buf, 47);
    preset_buf[47] = sys.checksum;

	for(int page = 0; page < 6; page++){

		memcpy(page_buf, &preset_buf[8 * page], 8); // copy one page of data from buffer
		error_check = EEPROM_Write_Page(PAGE_OFFSET + (preset_slot * 6) + page, page_buf);

		if(error_check != 0){
			return 1;
		}

	}

	// read checksum for EEPROM slot used and verify output
	if(sys.checksum == Readback_Checksum(preset_slot)){
		error_check = 0;
	}

    return error_check;
}

int Preset_Read(int preset_slot) {

    uint8_t page_buf[8];
    uint8_t preset_buf[48];
    uint8_t read_checksum;

    // read all pages back into the buffer
    for (int page = 0; page < 6; page++) {

        if (EEPROM_Read_Page(PAGE_OFFSET + (preset_slot * 6) + page, page_buf) != 0) {
            return 1; // error during EEPROM read
        }

        memcpy(&preset_buf[8 * page], page_buf, 8); // copy page data into buffer
    }

    // validate the checksum
    read_checksum = preset_buf[47];
    if (read_checksum != XOR_Checksum(preset_buf, 48)) {
        return 1; // checksum mismatch
    }

    // copy buffer data into synth parameters and system structures
    memcpy(&SynthParameters, preset_buf, sizeof(Synth));
    sys.serial_cmd_echo = preset_buf[44];
    sys.midi_tx_pol = preset_buf[45];
    sys.midi_tx_en = preset_buf[46];
    sys.checksum = read_checksum;

    return 0;
}


uint8_t XOR_Checksum(uint8_t *data, int length) {

    uint8_t checksum = 0x00;

    for (int i = 0; i < length; i++) {
        checksum ^= data[i]; // XOR each byte
    }

    return checksum;
}

uint8_t Readback_Checksum(int preset_slot){

	uint8_t i2c_buf[1];
	uint8_t checksum_address = (0x3F + (0x30 * preset_slot));

	HAL_I2C_Mem_Read(&hi2c3, EEPROM_ADDR, checksum_address, I2C_MEMADD_SIZE_8BIT, i2c_buf, 1, 10);

    return i2c_buf[0];
}

int EEPROM_Write_Page(int page_number, uint8_t *data){

	// calculate page start address
    uint8_t start_address = page_number * PAGE_SIZE;

    // write page data
    HAL_StatusTypeDef result = HAL_I2C_Mem_Write(&hi2c3, EEPROM_ADDR, start_address, I2C_MEMADD_SIZE_8BIT, data, PAGE_SIZE, 10);

    // wait for EEPROM write cycle to complete (typically 5ms)
    HAL_Delay(5);

    return (result == HAL_OK) ? 0 : 1;
}

int EEPROM_Read_Page(int page_number, uint8_t *data){

    // calculate page start address
    uint8_t start_address = page_number * PAGE_SIZE;

    // read page data
    HAL_StatusTypeDef result = HAL_I2C_Mem_Read(&hi2c3, EEPROM_ADDR, start_address, I2C_MEMADD_SIZE_8BIT, data, PAGE_SIZE, 10);

    return (result == HAL_OK) ? 0 : 1;
}



