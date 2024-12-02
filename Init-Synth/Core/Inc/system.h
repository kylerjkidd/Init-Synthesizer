/*
 * system.h
 *
 *  Created on: Nov 30, 2024
 *      Author: Implacable
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include "stdint.h"

#define RED_LED_PORT GPIOA
#define RED_LED_PIN  GPIO_PIN_10

#define GRN_LED_PORT GPIOA
#define GRN_LED_PIN  GPIO_PIN_9

#define TP0_PORT GPIOB
#define TP0_PIN  GPIO_PIN_5

#define USB_RENUM_PORT GPIOB
#define USB_RENUM_PIN  GPIO_PIN_5

typedef struct {

	// system controls
	int serial_cmd_echo;
	int midi_tx_pol;
	int midi_tx_en;
	int	value_returned;
	int green_led_state;
	int	red_led_state;
	int blink_counter;

	// communication buffers
	uint8_t usb_vcp_buffer[64];
	uint8_t midi_buf[3];
	uint8_t temp_buf[3];
	int temp_buf_index;

	// communication status flags
	int usb_data_present;
	int midi_data_present;

	// midi variables
	int note_status_bit;
	int midi_note_input;
	int midi_note_velocity;

	// synth controls
	int gate;
	int velocity_enable;

} System;

extern System sys;

void System_Reset_Initialize();

#endif /* INC_SYSTEM_H_ */
