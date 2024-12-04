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

void MIDI_Decode_Handler();
void HAL_UART_RxCpltCallback();

#endif /* INC_MIDI_H_ */
