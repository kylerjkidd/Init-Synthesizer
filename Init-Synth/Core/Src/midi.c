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
#include "system.h"

void MIDI_Decode_Handler(){

	int status_byte = sys.midi_buf[0];
	int data_byte1  = sys.midi_buf[1];
	int data_byte2  = sys.midi_buf[2];

	if(status_byte < 0x80){ // if the status byte is less than 128, it is assumed to be ASCII and a system level command

		Serial_Command_Handler();

		sys.temp_buf_index = 0;
	    memset (sys.midi_buf, '\0', 3); // clear buffer of old data

	    return;
	}

	sys.midi_data_present = 0; // reset interrupt flag

	status_byte = ( 0xF0 & status_byte); // mask out MIDI channel byte

    if(status_byte == 0x80){
    	sys.note_status_bit    =  0; // note off
    	sys.midi_note_input    = data_byte1;
    	sys.midi_note_velocity = data_byte2;

    	sys.gate = 0;
    	Gate_Control();
    	Command_Blink_Status_LED(0); // report no error
    }
    else if(status_byte == 0x90){
    	sys.note_status_bit    =  1; // note on
    	sys.midi_note_input    = data_byte1;
    	sys.midi_note_velocity = data_byte2;

    	sys.gate = 1;
    	Gate_Control();
    	Command_Blink_Status_LED(0);
    }
    else{
    	sys.note_status_bit = -1; // invalid status command
    	Command_Blink_Status_LED(1); // report an error;
    }

    sys.temp_buf_index = 0;
    memset (sys.midi_buf, '\0', 3); // clear buffer of old data

    return;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	sys.midi_buf[sys.temp_buf_index] = sys.temp_buf[0]; // store the received byte into midi_buf

        // increment the buffer index
        if (++sys.temp_buf_index >= 3)
        {
        	sys.temp_buf_index = 0;    // reset index after storing 3 bytes (to start a new group)
            sys.midi_data_present = 1; // set flag to indicate a complete command has been entered
        }

        // reset interrupt for next byte
        HAL_UART_Receive_IT(&huart2, sys.temp_buf, 1);

}


