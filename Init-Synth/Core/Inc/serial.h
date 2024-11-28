/*
 * serial.h
 *
 *  Created on: Apr 15, 2024
 *      Author: Implacable
 */

#ifndef INC_SERIAL_H_
#define INC_SERIAL_H_

#include "stdint.h"

extern uint8_t usb_vcp_buffer[64]; // import buffer from main
extern int usb_data_present; // import state from main

extern uint8_t midi_buf[3];
extern int midi_data_present;

extern int velocity_enable;

#define RED_LED_PORT GPIOA
#define RED_LED_PIN  GPIO_PIN_10

#define GRN_LED_PORT GPIOA
#define GRN_LED_PIN  GPIO_PIN_9

void Serial_Command_Handler();

int VCA_Command_Handler(int address, int data);
void VCA_Mod_Source_Decode(int data);
void VCA_Bypass_Decode(int data);

int Envelope_Command_Handler(int address, int data);
void Envelope_Output_Polarity_Decode(int data);
void Envelope_Loop_Mode_Decode(int data);

int Filter_Command_Handler(int address, int data);
void Filter_Mod_Source_Decode(int data);

int Mixer_Command_Handler(int address, int data);

int LFO_Command_Handler(int address, int data);
void LFO_Waveform_Decode(int data);

int Preset_Command_Handler(int address, int data);

void Oscillator_1_Command_Handler(int address, int data);
void Oscillator_2_Command_Handler(int address, int data);


void Command_Error();
void Command_Success();
int Command_Range_Check(int data, int variable, int max_value);
int Command_Range_Check_ptr(int data, int variable, int max_value, int *returned_value);
int Command_Range_Check_Error(int data, int max_value);
int Command_Option_Check_Error(int data, int max_value);
float Command_Range_Check_Float(float data, float variable, float max_value);

#endif /* INC_SERIAL_H_ */
