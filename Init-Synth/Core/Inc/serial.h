/*
 * serial.h
 *
 *  Created on: Apr 15, 2024
 *      Author: Implacable
 */

#ifndef INC_SERIAL_H_
#define INC_SERIAL_H_

#include "stdint.h"

#include "system.h"

void Serial_Command_Handler();

int VCA_Command_Handler(int address, int data);
void VCA_Mod_Source_Decode(int data);
void VCA_Bypass_Decode(int data);
void VCA_Value_Query(int data);

int Communication_Command_Handler(int address, int data);

int Envelope_Command_Handler(int address, int data);
void Envelope_Output_Polarity_Decode(int data);
void Envelope_Loop_Mode_Decode(int data);
void Envelope_Value_Query(int data);

int Filter_Command_Handler(int address, int data);
void Filter_Mod_Source_Decode(int data);
void Filter_Value_Query(int data);

int Mixer_Command_Handler(int address, int data);
void Mixer_Value_Query(int data);

int LFO_Command_Handler(int address, int data);
void LFO_Waveform_Decode(int data);
void LFO_Value_Query(int data);

int Preset_Command_Handler(int address, int data);

void Oscillator_1_Command_Handler(int address, int data);
void Oscillator_2_Command_Handler(int address, int data);

float Command_Range_Check_Float(float data, float variable, float max_value);

#endif /* INC_SERIAL_H_ */
