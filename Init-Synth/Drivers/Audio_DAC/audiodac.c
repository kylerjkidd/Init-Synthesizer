/*
 * audio_dac.c
 *
 *  Created on: Jul 28, 2023
 *      Author: Implacable
 */

#include "i2c.h"

#include "audiodac.h"

void DAC_Register_Write(uint8_t register_addr, uint8_t data){
	uint8_t i2c_buf[2];

	i2c_buf[0] = data;

	HAL_I2C_Mem_Write(&hi2c3, DAC_ADDR, register_addr, I2C_MEMADD_SIZE_8BIT, i2c_buf, 1, 10);

	return;
}

uint8_t DAC_Register_Read(uint8_t register_addr){
	uint8_t i2c_buf[2];

	HAL_I2C_Mem_Read(&hi2c3, DAC_ADDR, register_addr, I2C_MEMADD_SIZE_8BIT, i2c_buf, 1, 10);

	return i2c_buf[0];
}

void DAC_Register_Init(){

	DAC_Register_Write(DAC_REG_RESET , 0x80); // set system reset bit (device clears after reset)

	DAC_Register_Write(DAC_REG_VOL_L , 0x27); // left channel volume -6dB
	DAC_Register_Write(DAC_REG_VOL_R , 0x27); // right channel volume -6dB

	DAC_Register_Write(DAC_REG_ATTEN_L , 0x27); // left channel digital attenuator -24dB
	DAC_Register_Write(DAC_REG_ATTEN_R , 0x27); // right channel digital attenuator -24dB

	DAC_Register_Write(DAC_REG_DEMP_DGC , 0x00); // de-emphasis filter off, DAC format I2S, no digital gain, over sampling enabled

	DAC_Register_Write(DAC_REG_DAC_PWR , 0xE0); // power on bias circuitry and left/right DAC and filter

	DAC_Register_Write(DAC_REG_SAMPLE_RATE , 0x01); // zero cross detection enable

	DAC_Register_Write(DAC_REG_HPA_DETECT , 0x03); // analog left/right mixer power on

	DAC_Register_Write(DAC_REG_ANALOG_MIXER , 0x11); // analog mixer close switch 2 and 5

	DAC_Register_Write(DAC_REG_DAC_PWR , 0xEC); // enable headphone amplifier

	DAC_Register_Write(DAC_REG_ANALOG_CONFIG , 0x01); // VCOM power up

	DAC_Register_Write(DAC_REG_ANALOG_GAIN , 0x30); // power up analog front end

	return;
}

