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

void HAL_UART_RxCpltCallback();
//void MIDI_Decode_Handler(int status_byte, int data_byte1, int data_byte2);
void MIDI_Decode_Handler();

#endif /* INC_MIDI_H_ */
