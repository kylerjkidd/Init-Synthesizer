/*
 * midi.h
 *
 *  Created on: May 13, 2024
 *      Author: Implacable
 */

#ifndef INC_MIDI_H_
#define INC_MIDI_H_

#include "stdint.h"

extern UART_HandleTypeDef huart2;
extern uint8_t midi_buf[3];
extern uint8_t temp_buf[3];
extern int temp_buf_index;

extern uint8_t usb_vcp_buffer[64]; // import buffer from main

// communication status flags
extern int usb_data_present;
extern int midi_data_present;

extern int note_status_bit;
extern int midi_note_input;
extern int midi_note_velocity;

void HAL_UART_RxCpltCallback();
//void MIDI_Decode_Handler(int status_byte, int data_byte1, int data_byte2);
void MIDI_Decode_Handler();

#endif /* INC_MIDI_H_ */
