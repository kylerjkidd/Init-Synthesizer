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

//extern typedef enum {
//    MAX_RANGE_CHECK,
//    OPTION_BOX_CHECK,
//    NUMBER_CHECK,
//    FLOAT_RANGE_CHECK
//} ErrorCheckType;

void Serial_Command_Handler();

int VCA_Command_Handler(int address, int data);
void VCA_Mod_Source_Decode(int data);
void VCA_Bypass_Decode(int data);
void VCA_Value_Query(int data);

int Communication_Command_Handler(int address, int data);

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


/*void Command_Error();
void Command_Success();

void Command_Response_Handler(int error_check, uint8_t command_byte, uint8_t address_byte, uint8_t data_byte);

int Command_Error_Check(ErrorCheckType checkType, int data, int max_value, float float_data, float float_max);

void Command_Blink_Status_LED(int error_check);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);*/

int Command_Range_Check_Error(int data, int max_value);
int Command_Option_Check_Error(int data, int max_value);
int Command_Number_Check_Error(int data, int max_value);
float Command_Range_Check_Float(float data, float variable, float max_value);

#endif /* INC_SERIAL_H_ */
