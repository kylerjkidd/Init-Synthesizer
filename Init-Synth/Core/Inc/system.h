/*
 * system.h
 *
 *  Created on: Nov 30, 2024
 *      Author: Implacable
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include "stdint.h"

// ===========================================================================================================
// pin definitions

#define RED_LED_PORT GPIOA
#define RED_LED_PIN  GPIO_PIN_10

#define GRN_LED_PORT GPIOA
#define GRN_LED_PIN  GPIO_PIN_9

#define TP0_PORT GPIOB
#define TP0_PIN  GPIO_PIN_5

#define WRITE_PROTECT_PORT GPIOC
#define WRITE_PROTECT_PIN  GPIO_PIN_7

// gate signal
#define GATE_PORT 0
#define GATE_PIN 3

// pin used to re-enumerate USB interface (for debug)
#define USB_RENUM_PORT GPIOB
#define USB_RENUM_PIN  GPIO_PIN_5

// toggle polarity of MIDI UART output
#define MIDI_TX_POL_PORT 0
#define MIDI_TX_POL_PIN 2

// enable transmit out of MIDI port
#define MIDI_TX_EN_PORT 0
#define MIDI_TX_EN_PIN 0

// ===========================================================================================================
// conditional check option declaration

typedef enum {
    MAX_RANGE_CHECK,
    OPTION_BOX_CHECK,
    NUMBER_CHECK,
    FLOAT_RANGE_CHECK
} ErrorCheckType;

// ===========================================================================================================
// default values for system settings
#define SERIAL_CMD_ECHO_DEFAULT     0
#define MIDI_TX_POL_DEFAULT         0
#define MIDI_TX_EN_DEFAULT          0

// ===========================================================================================================
// system parameters struct declaration

typedef struct {

	// system controls
	int serial_cmd_echo;
	int midi_tx_pol;
	int midi_tx_en;
	int	value_returned;
	int green_led_state;
	int	red_led_state;
	int blink_counter;
	int write_protect;

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
	uint16_t gpio_reg;

} System;

extern System sys;

// ===========================================================================================================
// function definitions

void System_Reset_Initialize();

void Write_Protect_Control();
void MIDI_Port_Control();
void Gate_Control();

void Command_Error();
void Command_Success();

void Command_Response_Handler(int error_check, uint8_t command_byte, uint8_t address_byte, uint8_t data_byte);

int Command_Error_Check(ErrorCheckType checkType, int data, int max_value, float float_data, float float_max);

void Command_Blink_Status_LED(int error_check);

#endif /* INC_SYSTEM_H_ */
