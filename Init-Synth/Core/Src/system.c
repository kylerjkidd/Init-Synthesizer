/*
 * system.c
 *
 *  Created on: Nov 30, 2024
 *      Author: Implacable
 */

#include "stdint.h"
#include "string.h"
#include "gpio.h"
#include "usbd_cdc_if.h"
#include "tim.h"
#include "usart.h"

#include "system.h"
#include "audiodac.h"
#include "gpioxpndr.h"

System sys;

void System_Reset_Initialize(){

	// system controls
	sys.serial_cmd_echo = 0;
	sys.midi_tx_pol = 0;
	sys.midi_tx_en = 0;
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
	sys.gpio_reg = 0;

	HAL_GPIO_WritePin(RED_LED_PORT, RED_LED_PIN, SET); // red

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, SET); // enable analog power supply

	DAC_Register_Init();

	DAC_Register_Write(0x40, 0x3F);
	DAC_Register_Write(0x44, 0x3F);

	DAC_Register_Write(0x41, 0x3F);
	DAC_Register_Write(0x45, 0x3F);

	GPIO_Register_Init();

	MIDI_Port_Control();
	Gate_Control();

	return;
}

// ===========================================================================================================
// system functions

void Gate_Control(){

	// gate control for envelope trigger and LED indicator
	sys.gpio_reg = GPIO_State_Change(GATE_PORT, sys.gpio_reg, GATE_PIN, sys.gate);

	return;
}

void MIDI_Port_Control(){

	// enable or disable transmit through MIDI port
	sys.gpio_reg = GPIO_State_Change(MIDI_TX_EN_PORT, sys.gpio_reg, MIDI_TX_EN_PIN, sys.midi_tx_en == 0);

	// toggle polarity of MIDI UART output
	sys.gpio_reg = GPIO_State_Change(MIDI_TX_POL_PORT, sys.gpio_reg, MIDI_TX_POL_PIN, sys.midi_tx_pol);

	return;
}

void Command_Error(){

	for(int i=0; i <6 ; i++){

		HAL_GPIO_TogglePin(RED_LED_PORT, RED_LED_PIN);
		HAL_Delay(125);
	}

	return;
}

void Command_Success(){

	for(int i=0; i <2 ; i++){

		HAL_GPIO_TogglePin(GRN_LED_PORT, GRN_LED_PIN);
		HAL_Delay(125);
	}

	return;
}

void Command_Response_Handler(int error_check, uint8_t command_byte, uint8_t address_byte, uint8_t data_byte){

	uint8_t return_data[3];

	if(sys.value_returned == 1){ // exit early if response already returned
        return;
	}

    if (error_check == 1) {
    	memcpy(return_data, "ERR", 3); // report error

    } else if (sys.serial_cmd_echo == 1) { // echo command

        return_data[0] = command_byte;
        return_data[1] = address_byte;
        return_data[2] = data_byte;

    } else {
    	memcpy(return_data, "ACK", 3); // acknowledge input

    }

    CDC_Transmit_FS(return_data, 3); // transmit return message

    if (sys.midi_tx_en == 1) { // send return message if enabled
        HAL_UART_Transmit(&huart2, return_data, 3, 1000);
    }

	return;
}

// check command argument to verify input is valid
int Command_Error_Check(ErrorCheckType checkType, int data, int max_value, float float_data, float float_max){
    switch (checkType) {
        case MAX_RANGE_CHECK:

            return (data <= max_value) ? 0 : 1;
        case OPTION_BOX_CHECK:

            return (data <= 0 || data > max_value) ? 1 : 0;
        case NUMBER_CHECK:

            return (data <= '0' || data > max_value + '0') ? 1 : 0;
        case FLOAT_RANGE_CHECK:

            return (float_data <= float_max) ? 0 : 1;
        default:
            return 1; // invalid input value
    }
}

// if an error is detected, blink red LED twice, blink green LED once if no error
void Command_Blink_Status_LED(int error_check){

	if(error_check == 1){
		sys.red_led_state = 1;
	}
	else sys.green_led_state = 1;

	HAL_TIM_Base_Start_IT(&htim6); // start LED timer

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if(htim == &htim6){

		if(sys.red_led_state == 1){

	        if (sys.blink_counter < 4) {
	            HAL_GPIO_TogglePin(RED_LED_PORT, RED_LED_PIN);
	            sys.blink_counter++;
	        } else {
	            HAL_TIM_Base_Stop_IT(&htim6); // stop the timer
	            sys.red_led_state = 0;
	            sys.blink_counter = 0;
	        }
		}

		if(sys.green_led_state == 1){

	        if (sys.blink_counter < 2) {
	            HAL_GPIO_TogglePin(GRN_LED_PORT, GRN_LED_PIN);
	            sys.blink_counter++;
	        } else {
	            HAL_TIM_Base_Stop_IT(&htim6); // stop the timer
	            sys.green_led_state = 0;
	            sys.blink_counter = 0;
	        }
		}

	}

}
