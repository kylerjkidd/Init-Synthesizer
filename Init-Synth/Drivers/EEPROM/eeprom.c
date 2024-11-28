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

uint8_t page_buf[8];
uint8_t byte_buf[4];
uint8_t bytes[4];

union {
    float f;
    uint32_t i;
} float_union;

union {
    float f;
    uint32_t i;
} float_byte;

void Convert_Float_to_Binary(float input){

//	union {
//	    float f;
//	    uint32_t i;
//	} float_union;

    float_union.f = input;

    memcpy(byte_buf, &float_union.i, 4);

	return;
}

void Convert_Binary_to_Float(){

//    //float_union.f = input;
//    // load byte buffer
//    for (uint8_t *p = input; *p < 4; p++) {
//
//    	byte_buf[p];
//
//		}


    // convert the bytes in buffer back to float
//    union {
//        float f;
//        uint32_t i;
//    } float_byte;

    // copy the bytes back into the union
    memcpy(&float_byte.i, byte_buf, 4);

	return;
}

void Convert_Float_to_Bytes(float value, uint8_t *bytes) {
    union {
        float f;
        uint8_t b[4];
    } converter;

    converter.f = value;
    bytes[0] = converter.b[0];
    bytes[1] = converter.b[1];
    bytes[2] = converter.b[2];
    bytes[3] = converter.b[3];
}


void EEPROM_Page_Write(){
	uint8_t i2c_buf[8];

	for(int i=0 ; i<8 ; i++){
		i2c_buf[i] = i;
	}

	HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, i2c_buf, 8, 10);

	return;
}

void EEPROM_Page_Read(){
	uint8_t i2c_buf[8];

	HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, i2c_buf, 8, 10);

	return;
}

void Preset_Save(int address){

	uint8_t preset_buf[64];

	preset_buf[0]  = 'P';
	preset_buf[1]  = 'R';
	preset_buf[2]  = 'E';
	preset_buf[3]  = (uint8_t)address + '0';

	preset_buf[4]  = (uint8_t)oscillator1;
	preset_buf[5]  = (uint8_t)oscillator2;
	preset_buf[6]  = (uint8_t)note_offset1;
	preset_buf[7]  = (uint8_t)note_offset2;

	preset_buf[8]  = (uint8_t)volume_osc1;
	preset_buf[9]  = (uint8_t)volume_osc2;
	preset_buf[10] = (uint8_t)dac_mixer_level;
	preset_buf[11] = (uint8_t)filter_out_level;
	preset_buf[12] = (uint8_t)vcf_cv_en;
	preset_buf[13] = (uint8_t)vcf_cv_sel;
	preset_buf[14] = (uint8_t)vcf_cv_intensity;
	preset_buf[15] = (uint8_t)vcf_cutoff;

	preset_buf[16] = (uint8_t)vcf_resonance;
	preset_buf[17] = (uint8_t)lfo_output_en;
	preset_buf[18] = (uint8_t)lfo_output_waveform;
	preset_buf[19] = (uint8_t)lfo_frequency;
	preset_buf[20] = (uint8_t)vca_cv_en;
	preset_buf[21] = (uint8_t)vca_cv_sel;
	preset_buf[22] = (uint8_t)vca_output_en;
	preset_buf[23] = (uint8_t)vca_output_sel;

	preset_buf[24] = (uint8_t)vca_cv_intensity;
	preset_buf[25] = (uint8_t)vca_offset;
	preset_buf[26] = (uint8_t)env_en;
	preset_buf[27] = (uint8_t)env_loop;
	preset_buf[28] = (uint8_t)env_invert;
	preset_buf[29] = (uint8_t)env_attack_rate;
	preset_buf[30] = (uint8_t)env_release_rate;
	preset_buf[31] = (uint8_t)env_decay_rate;

	Convert_Float_to_Bytes(detune_osc1, bytes);

	preset_buf[32] = bytes[0];
	preset_buf[33] = bytes[1];
	preset_buf[34] = bytes[2];
	preset_buf[35] = bytes[3];

	Convert_Float_to_Bytes(detune_osc2, bytes);

	preset_buf[36] = bytes[0];
	preset_buf[37] = bytes[1];
	preset_buf[38] = bytes[2];
	preset_buf[39] = bytes[3];

	Convert_Float_to_Bytes(fm_intensity1, bytes);

	preset_buf[40] = bytes[0];
	preset_buf[41] = bytes[1];
	preset_buf[42] = bytes[2];
	preset_buf[43] = bytes[3];

	Convert_Float_to_Bytes(fm_intensity2, bytes);

	preset_buf[44] = bytes[0];
	preset_buf[45] = bytes[1];
	preset_buf[46] = bytes[2];
	preset_buf[47] = bytes[3];

	Convert_Float_to_Bytes(fm_harmonic1, bytes);

	preset_buf[48] = bytes[0];
	preset_buf[49] = bytes[1];
	preset_buf[50] = bytes[2];
	preset_buf[51] = bytes[3];

	Convert_Float_to_Bytes(fm_harmonic2, bytes);

	preset_buf[52] = bytes[0];
	preset_buf[53] = bytes[1];
	preset_buf[54] = bytes[2];
	preset_buf[55] = bytes[3];

	Convert_Float_to_Bytes(duty_cycle1, bytes);

	preset_buf[56] = bytes[0];
	preset_buf[57] = bytes[1];
	preset_buf[58] = bytes[2];
	preset_buf[59] = bytes[3];

	Convert_Float_to_Bytes(duty_cycle2, bytes);

	preset_buf[60] = bytes[0];
	preset_buf[61] = bytes[1];
	preset_buf[62] = bytes[2];
	preset_buf[63] = bytes[3];

	return;
}

void Preset_Load(int address) {

	uint8_t preset_buf[64];

    //'P'                   = preset_buf[0];
    //'R'                   = preset_buf[1];
    //'E'                   = preset_buf[2];
    //address + '0'         = preset_buf[3];

    oscillator1           = preset_buf[4];
    oscillator2           = preset_buf[5];
    note_offset1          = preset_buf[6];
    note_offset2          = preset_buf[7];

    volume_osc1           = preset_buf[8];
    volume_osc2           = preset_buf[9];
    dac_mixer_level       = preset_buf[10];
    filter_out_level      = preset_buf[11];
    vcf_cv_en             = preset_buf[12];
    vcf_cv_sel            = preset_buf[13];
    vcf_cv_intensity      = preset_buf[14];
    vcf_cutoff            = preset_buf[15];

    vcf_resonance         = preset_buf[16];
    lfo_output_en         = preset_buf[17];
    lfo_output_waveform   = preset_buf[18];
    lfo_frequency         = preset_buf[19];
    vca_cv_en             = preset_buf[20];
    vca_cv_sel            = preset_buf[21];
    vca_output_en         = preset_buf[22];
    vca_output_sel        = preset_buf[23];

    vca_cv_intensity      = preset_buf[24];
    vca_offset            = preset_buf[25];
    env_en                = preset_buf[26];
    env_loop              = preset_buf[27];
    env_invert            = preset_buf[28];
    env_attack_rate       = preset_buf[29];
    env_release_rate      = preset_buf[30];
    env_decay_rate        = preset_buf[31];

    Convert_Float_to_Bytes(detune_osc1, bytes);

    bytes[0] = preset_buf[32];
    bytes[1] = preset_buf[33];
    bytes[2] = preset_buf[34];
    bytes[3] = preset_buf[35];

    Convert_Float_to_Bytes(detune_osc2, bytes);

    bytes[0] = preset_buf[36];
    bytes[1] = preset_buf[37];
    bytes[2] = preset_buf[38];
    bytes[3] = preset_buf[39];

    Convert_Float_to_Bytes(fm_intensity1, bytes);

    bytes[0] = preset_buf[40];
    bytes[1] = preset_buf[41];
    bytes[2] = preset_buf[42];
    bytes[3] = preset_buf[43];

    Convert_Float_to_Bytes(fm_intensity2, bytes);

    bytes[0] = preset_buf[44];
    bytes[1] = preset_buf[45];
    bytes[2] = preset_buf[46];
    bytes[3] = preset_buf[47];

    Convert_Float_to_Bytes(fm_harmonic1, bytes);

    bytes[0] = preset_buf[48];
    bytes[1] = preset_buf[49];
    bytes[2] = preset_buf[50];
    bytes[3] = preset_buf[51];

    Convert_Float_to_Bytes(fm_harmonic2, bytes);

    bytes[0] = preset_buf[52];
    bytes[1] = preset_buf[53];
    bytes[2] = preset_buf[54];
    bytes[3] = preset_buf[55];

    Convert_Float_to_Bytes(duty_cycle1, bytes);

    bytes[0] = preset_buf[56];
    bytes[1] = preset_buf[57];
    bytes[2] = preset_buf[58];
    bytes[3] = preset_buf[59];

    Convert_Float_to_Bytes(duty_cycle2, bytes);

    bytes[0] = preset_buf[60];
    bytes[1] = preset_buf[61];
    bytes[2] = preset_buf[62];
    bytes[3] = preset_buf[63];

    return;
}


//extern float detune_osc1;
//extern float detune_osc2;
//extern float fm_intensity1;
//extern float fm_intensity2;
//extern float fm_harmonic1;
//extern float fm_harmonic2;
//extern float duty_cycle1;
//extern float duty_cycle2;


