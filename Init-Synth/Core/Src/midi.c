/*
 * midi.c
 *
 *  Created on: May 13, 2024
 *      Author: Implacable
 */

#include "usart.h"
#include "stdint.h"
#include "string.h"

#include "midi.h"
#include "serial.h"

void MIDI_Decode_Handler(){

	//memcpy (midi_buf, rx_buf, temp_buf_index);

	int status_byte = midi_buf[0];
	int data_byte1  = midi_buf[1];
	int data_byte2  = midi_buf[2];

	if(status_byte < 0x80){ // if the status byte is less than 128, it is assumed to be ASCII and a system level command

		Serial_Command_Handler();

		temp_buf_index = 0;
	    memset (midi_buf, '\0', 3); // clear buffer of old data

	    return;
	}

	midi_data_present = 0; // reset interrupt flag

	status_byte = ( 0xF0 & status_byte); // mask out MIDI channel byte

    if(status_byte == 0x80){
    	note_status_bit    =  0; // note off
    	midi_note_input    = data_byte1;
    	midi_note_velocity = data_byte2;
    }
    else if(status_byte == 0x90){
    	note_status_bit    =  1; // note on
    	midi_note_input    = data_byte1;
    	midi_note_velocity = data_byte2;
    }
    else{
    	note_status_bit = -1; // invalid status command
    	Command_Error();
    }

	temp_buf_index = 0;
    memset (midi_buf, '\0', 3); // clear buffer of old data

    return;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

        midi_buf[temp_buf_index] = temp_buf[0]; // store the received byte into midi_buf

        // increment the buffer index
        if (++temp_buf_index >= 3)
        {
            temp_buf_index = 0;    // reset index after storing 3 bytes (to start a new group)
            midi_data_present = 1; // set flag to indicate a complete command has been entered
        }

        // reset interrupt for next byte
        HAL_UART_Receive_IT(&huart2, temp_buf, 1);

}


