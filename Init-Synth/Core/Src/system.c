/*
 * system.c
 *
 *  Created on: Nov 30, 2024
 *      Author: Implacable
 */

#include "gpio.h"

#include "system.h"
#include "audiodac.h"
#include "gpioxpndr.h"

System sys;

void System_Reset_Initialize(){

	// system controls
	sys.serial_cmd_echo = 0;
	sys.midi_tx_pol = 0;
	sys.midi_tx_en = 1;
	sys.value_returned = 0;
	sys.green_led_state = 0;
	sys.red_led_state = 0;
	sys.blink_counter = 0;

	// communication buffers
	sys.usb_vcp_buffer[64] = 0;
	sys.midi_buf[3] = 0;
	sys.temp_buf[3] = 0;
	sys.temp_buf_index = 0;

	// communication status flags
	sys.usb_data_present = 0;
	sys.midi_data_present = 0;

	// midi variables
	sys.note_status_bit = 0;
	sys.midi_note_input = 0;
	sys.midi_note_velocity = 0;

	sys.gate = 0;
	sys.velocity_enable = 0;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, SET); // enable analog power supply

	DAC_Register_Init();

	DAC_Register_Write(0x40, 0x3F);
	DAC_Register_Write(0x44, 0x3F);

	DAC_Register_Write(0x41, 0x3F);
	DAC_Register_Write(0x45, 0x3F);

	GPIO_Register_Init();

	return;
}


