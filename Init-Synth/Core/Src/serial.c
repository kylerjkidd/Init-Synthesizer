/*
 * serial.c
 *
 *  Created on: Apr 15, 2024
 *      Author: Implacable
 */

#include "usart.h"
#include "stdint.h"
#include "string.h"
#include "gpio.h"
#include "usbd_cdc_if.h"
#include "tim.h"

#include "synth.h"
#include "serial.h"
#include "system.h"

//extern System sys;

void Serial_Command_Handler(){

	int error_check = 1;

	int command_byte;
	int address_byte;
	int data_byte;

	sys.value_returned = 0;

    if(sys.usb_data_present == 1){
    	sys.usb_data_present = 0;

        command_byte = sys.usb_vcp_buffer[0];
        address_byte = sys.usb_vcp_buffer[1];
        data_byte    = sys.usb_vcp_buffer[2];

    }

    if(sys.midi_data_present == 1){
    	sys.midi_data_present = 0;

        command_byte = sys.midi_buf[0];
        address_byte = sys.midi_buf[1];
        data_byte    = sys.midi_buf[2];

    }

    switch(command_byte) {
        case 'A': // voltage controlled amplifier command

        	error_check = VCA_Command_Handler(address_byte, data_byte);

            break;
        case 'C': // communication settings command

        	error_check = Communication_Command_Handler(address_byte, data_byte);

            break;
        case 'E': // envelope command

        	error_check = Envelope_Command_Handler(address_byte, data_byte);

            break;
        case 'F': // low pass filter command

        	error_check = Filter_Command_Handler(address_byte, data_byte);

            break;
        case 'I': // initialize preset command

        	error_check = Synth_Initialize_Preset();

            break;
        case 'L': // load preset command

            break;
        case 'M': // mixer command

        	error_check = Mixer_Command_Handler(address_byte, data_byte);

            break;
        case 'O': // LFO command

        	error_check = LFO_Command_Handler(address_byte, data_byte);

            break;
        case 'P': // preset save/load command

        	error_check = Preset_Command_Handler(address_byte, data_byte);

            break;
        case 'R': // reset command

        	for(int i=0; i <6 ; i++){
        	HAL_GPIO_TogglePin(GRN_LED_PORT, GRN_LED_PIN);
        	HAL_GPIO_TogglePin(RED_LED_PORT, RED_LED_PIN);
        	HAL_Delay(125);
        	}

			NVIC_SystemReset();

            break;
        case 'S': // save preset command

            break;
        case 'X': // oscillator 1 command

            break;
        case 'Y': // oscillator 2 command

            break;
        case 'Z': // FM command

            break;
        default:
            // do nothing on invalid command

            break;
    }

    Command_Blink_Status_LED(error_check);

    Command_Response_Handler(error_check, command_byte, address_byte, data_byte);

    memset (sys.usb_vcp_buffer, '\0', 64); // clear buffer of old data

}

// ===========================================================================================================
// A - VCA functions

int VCA_Command_Handler(int address, int data){

	int error_check = 1;

    switch(address) {
        case '1': // VCA modulation offset

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 255, 0, 0);

            if (error_check == 0) {
                SynthParameters.vca_offset = data;
                VCA_Digital_Pot_Control();
            }

        	return error_check;
        case '2': // VCA modulation intensity

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 255, 0, 0);

            if (error_check == 0) {
                SynthParameters.vca_cv_intensity = data;
                VCA_Digital_Pot_Control();
            }

        	return error_check;
        case '3': // VCA modulation control

        	error_check = Command_Error_Check(OPTION_BOX_CHECK, data, 3, 0, 0);

            if (error_check == 0) {
                SynthParameters.vca_mod_source = data;
                VCA_Mod_Source_Decode(SynthParameters.vca_mod_source);
                VCA_Mod_Source_Control();
            }

        	return error_check;
        case '4': // VCA signal bypass control

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 1, 0, 0);

            if (error_check == 0) {
                SynthParameters.vca_bypass = data;
                VCA_Mod_Source_Decode(SynthParameters.vca_bypass);
                VCA_Bypass_Switch_Control();
            }

        	return error_check;
        case '5': // read back setting value

        	error_check = Command_Error_Check(NUMBER_CHECK, data, 4, 0, 0);

        	//VCA_Value_Query(data);
            if (error_check == 0) {
            	VCA_Value_Query(data);
                sys.value_returned = 1; // tell the serial handler a response has been sent
            }

        	return error_check;
        default:
            // do nothing on invalid command

        	return error_check;
    }

    return error_check;
}

void VCA_Mod_Source_Decode(int data){

    switch(data) {
        case 1: // VCA modulation disabled
        	SynthParameters.vca_cv_en = 0;
            break;
        case 2: // VCA modulation set to LFO
        	SynthParameters.vca_cv_en = 1;
        	SynthParameters.vca_cv_sel = 1;
            break;
        case 3: // VCA modulation set to envelope
        	SynthParameters.vca_cv_en = 1;
        	SynthParameters.vca_cv_sel = 0;
            break;
        default:
            // do nothing on invalid command
            break;
    }

	return;
}

void VCA_Bypass_Decode(int data){

    if(data == 0){ // signal passes through VCA
    	SynthParameters.vca_output_en = 1;
    	SynthParameters.vca_output_sel = 0;
    }
    else if(data == 1){ // signal bypasses VCA
    	SynthParameters.vca_output_en = 1;
    	SynthParameters.vca_output_sel = 1;
    }

	return;
}

void VCA_Value_Query(int data){

	uint8_t return_data[3] = {'A', data , 0};

    switch(data) {
        case '1': // VCA modulation offset
        	return_data[2] = SynthParameters.vca_offset;
            break;
        case '2': // VCA modulation intensity
        	return_data[2] = SynthParameters.vca_cv_intensity;
            break;
        case '3': // VCA modulation control
        	return_data[2] = SynthParameters.vca_mod_source;
            break;
        case '4': // VCA signal bypass control
        	return_data[2] = SynthParameters.vca_bypass;
            break;
        default:  // do nothing on invalid command

            break;
    }

    CDC_Transmit_FS(return_data, 3); // transmit return message

    if (sys.midi_tx_en == 1) { // send return message if enabled
        HAL_UART_Transmit(&huart2, return_data, 3, 1000);
    }

	return;
}

// ===========================================================================================================
// C - communication settings function

int Communication_Command_Handler(int address, int data){

	int error_check = 1;

    switch(address) {
        case '1': // echo command/acknowledge response setting; true = echo, false = acknowledge

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 1, 0, 0);

            if (error_check == 0) {
            	sys.serial_cmd_echo = data;
            }

        	return error_check;
        case '2': // MIDI transmit enable setting; true = enabled, false = disabled

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 1, 0, 0);

            if (error_check == 0) {
            	sys.midi_tx_en = data;
            	MIDI_Port_Control();
            }

        	return error_check;
        case '3': // MIDI transmit output polarity setting; true = inverted, false = non-inverted

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 1, 0, 0);

            if (error_check == 0) {
            	sys.midi_tx_pol = data;
            	MIDI_Port_Control();
            }

        	return error_check;
        default:
            // do nothing on invalid command

        	return error_check;
    }

    return error_check;
}

// ===========================================================================================================
// E - envelope functions

int Envelope_Command_Handler(int address, int data){

	int error_check = 1;

    switch(address) {
        case '1': // envelope attack rate

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 255, 0, 0);

            if (error_check == 0) {
                SynthParameters.env_attack_rate = data;
                Envelope_Digital_Pot_Control();
            }

        	return error_check;
        case '2': // envelope sustain level/decay rate

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 255, 0, 0);

            if (error_check == 0) {
                SynthParameters.env_decay_rate = data;
                Envelope_Digital_Pot_Control();
            }

        	return error_check;
        case '3': // envelope release rate

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 255, 0, 0);

            if (error_check == 0) {
                SynthParameters.env_release_rate = data;
                Envelope_Digital_Pot_Control();
            }

        	return error_check;
        case '4': // envelope output polarity

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 1, 0, 0);

            if (error_check == 0) {
                SynthParameters.env_invert_mode = data;
                Envelope_Output_Polarity_Decode(SynthParameters.env_invert_mode);
                Envelope_Mode_Control();
            }

        	return error_check;
        case '5': // envelope loop mode control

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 1, 0, 0);

            if (error_check == 0) {
                SynthParameters.env_loop_mode = data;
                Envelope_Loop_Mode_Decode(SynthParameters.env_loop_mode);
                Envelope_Mode_Control();
            }

        	return error_check;
        case '6': // read back setting value

        	error_check = Command_Error_Check(NUMBER_CHECK, data, 5, 0, 0);

            if (error_check == 0) {
            	Envelope_Value_Query(data);
                sys.value_returned = 1; // tell the serial handler a response has been sent
            }

        	return error_check;
        default:
            // do nothing on invalid command

        	return error_check;
    }

    return error_check;
}

void Envelope_Output_Polarity_Decode(int data){

    if(data == 0){ // non-inverted envelope output
    	SynthParameters.env_en = 1;
    	SynthParameters.env_invert = 1;
    }
    else if(data == 1){ // inverted envelope output
    	SynthParameters.env_en = 1;
    	SynthParameters.env_invert = 0;
    }

	return;
}

void Envelope_Loop_Mode_Decode(int data){

    if(data == 0){ // disable loop mode
    	SynthParameters.env_en = 1;
    	SynthParameters.env_loop = 1;
    }
    else if(data == 1){ // enable loop mode
    	SynthParameters.env_en = 1;
    	SynthParameters.env_loop = 0;
    }

	return;
}

void Envelope_Value_Query(int data){

	uint8_t return_data[3] = {'E', data , 0};

    switch(data) {
        case '1': // envelope attack rate
        	return_data[2] = SynthParameters.env_attack_rate;
            break;
        case '2': // envelope sustain level/decay rate
        	return_data[2] = SynthParameters.env_decay_rate;
            break;
        case '3': // envelope release rate
        	return_data[2] = SynthParameters.env_release_rate;
            break;
        case '4': // envelope output polarity
        	return_data[2] = SynthParameters.env_invert_mode;
            break;
        case '5': // envelope loop mode control
        	return_data[2] = SynthParameters.env_loop_mode;
            break;
        default:  // do nothing on invalid command

            break;
    }

    CDC_Transmit_FS(return_data, 3); // transmit return message

    if (sys.midi_tx_en == 1) { // send return message if enabled
        HAL_UART_Transmit(&huart2, return_data, 3, 1000);
    }

	return;
}

// ===========================================================================================================
// F - filter functions

int Filter_Command_Handler(int address, int data){

	int error_check = 1;

    switch(address) {
        case '1': // filter cutoff

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 255, 0, 0);

            if (error_check == 0) {
                SynthParameters.vcf_cutoff = data;
                Filter_Digital_Pot_Control();
            }

        	return error_check;
        case '2': // filter resonance intensity

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 255, 0, 0);

            if (error_check == 0) {
                SynthParameters.vcf_resonance = data;
                Filter_Digital_Pot_Control();
            }

        	return error_check;
        case '3': // filter modulation intensity

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 255, 0, 0);

            if (error_check == 0) {
                SynthParameters.vcf_cv_intensity = data;
                Filter_Digital_Pot_Control();
            }

        	return error_check;
        case '4': // filter modulation control

        	error_check = Command_Error_Check(OPTION_BOX_CHECK, data, 3, 0, 0);

            if (error_check == 0) {
                SynthParameters.vcf_mod_source = data;
                Filter_Mod_Source_Decode(SynthParameters.vcf_mod_source);
                Filter_Mod_Source_Control();
            }

        	return error_check;
        case '5': // read back setting value

        	error_check = Command_Error_Check(NUMBER_CHECK, data, 4, 0, 0);

            if (error_check == 0) {
            	Filter_Value_Query(data);
                sys.value_returned = 1; // tell the serial handler a response has been sent
            }

        	return error_check;
        default:
            // do nothing on invalid command

        	return error_check;
    }

    return error_check;
}

void Filter_Mod_Source_Decode(int data){

    switch(data) {
        case 1: // filter modulation disabled
        	SynthParameters.vcf_cv_en = 0;
            break;
        case 2: // filter modulation set to LFO
        	SynthParameters.vcf_cv_en = 1;
        	SynthParameters.vcf_cv_sel = 1;
            break;
        case 3: // filter modulation set to envelope
        	SynthParameters.vcf_cv_en = 1;
        	SynthParameters.vcf_cv_sel = 0;
            break;
        default:
            // do nothing on invalid command

            break;
    }

	return;
}

void Filter_Value_Query(int data){

	uint8_t return_data[3] = {'F', data , 0};

    switch(data) {
        case '1': // filter cutoff
        	return_data[2] = SynthParameters.vcf_cutoff;
            break;
        case '2': // filter resonance intensity
        	return_data[2] = SynthParameters.vcf_resonance;
            break;
        case '3': // filter modulation intensity
        	return_data[2] = SynthParameters.vcf_cv_intensity;
            break;
        case '4': // filter modulation control
        	return_data[2] = SynthParameters.vcf_mod_source;
            break;
        default:  // do nothing on invalid command

            break;
    }

    CDC_Transmit_FS(return_data, 3); // transmit return message

    if (sys.midi_tx_en == 1) { // send return message if enabled
        HAL_UART_Transmit(&huart2, return_data, 3, 1000);
    }

	return;
}

// ===========================================================================================================
// M - mixer function

int Mixer_Command_Handler(int address, int data){

	int error_check = 1;

    switch(address) {
        case '1': // DAC output level

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 255, 0, 0);

            if (error_check == 0) {
                SynthParameters.dac_mixer_level = data;
                Mixer_Digital_Pot_Control();
            }

        	return error_check;
        case '2': // filter output level

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 255, 0, 0);

            if (error_check == 0) {
                SynthParameters.filter_out_level = data;
                Mixer_Digital_Pot_Control();
            }

        	return error_check;
        case '3': // enable/disable MIDI velocity

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 1, 0, 0);

            if (error_check == 0) {
            	sys.velocity_enable = data;
            }

        	return error_check;
        case '4': // read back setting value

        	error_check = Command_Error_Check(NUMBER_CHECK, data, 3, 0, 0);

            if (error_check == 0) {
            	Mixer_Value_Query(data);
                sys.value_returned = 1; // tell the serial handler a response has been sent
            }

        	return error_check;
        default:
            // do nothing on invalid command

        	return error_check;
    }

    return error_check;
}

void Mixer_Value_Query(int data){

	uint8_t return_data[3] = {'M', data , 0};

    switch(data) {
        case '1': // DAC output level
        	return_data[2] = SynthParameters.dac_mixer_level;
            break;
        case '2': // filter output level
        	return_data[2] = SynthParameters.filter_out_level;
            break;
        case '3': // enable/disable MIDI velocity
        	return_data[2] = sys.velocity_enable;
            break;
        default:  // do nothing on invalid command

            break;
    }

    CDC_Transmit_FS(return_data, 3); // transmit return message

    if (sys.midi_tx_en == 1) { // send return message if enabled
        HAL_UART_Transmit(&huart2, return_data, 3, 1000);
    }

	return;
}

// ===========================================================================================================
// O - LFO functions

int LFO_Command_Handler(int address, int data){

	int error_check = 1;

    switch(address) {
        case '1': // LFO frequency/rate

        	error_check = Command_Error_Check(MAX_RANGE_CHECK, data, 255, 0, 0);

            if (error_check == 0) {
                SynthParameters.lfo_frequency = data;
                LFO_Digital_Pot_Control();
            }

        	return error_check;
        case '2': // LFO output waveform

        	error_check = Command_Error_Check(OPTION_BOX_CHECK, data, 3, 0, 0);

            if (error_check == 0) {
                SynthParameters.lfo_waveform = data;
                LFO_Waveform_Decode(SynthParameters.lfo_waveform);
                LFO_Output_Control();
            }

        	return error_check;
        case '3': // read back setting value

        	error_check = Command_Error_Check(NUMBER_CHECK, data, 2, 0, 0);

            if (error_check == 0) {
            	LFO_Value_Query(data);
                sys.value_returned = 1; // tell the serial handler a response has been sent
            }

        	return error_check;
        default:
            // do nothing on invalid command

        	return error_check;
    }

    return error_check;
}

void LFO_Waveform_Decode(int data){

    switch(data) {
        case 1: // LFO output disabled
        	SynthParameters.lfo_output_en = 0;
            break;
        case 2: // LFO output set to square wave
        	SynthParameters.lfo_output_en = 1;
        	SynthParameters.lfo_output_waveform = 1;
            break;
        case 3: // LFO output set to triangle wave
        	SynthParameters.lfo_output_en = 1;
        	SynthParameters.lfo_output_waveform = 0;
            break;
        default:
            // do nothing on invalid command

            break;
    }

	return;
}

void LFO_Value_Query(int data){

	uint8_t return_data[3] = {'O', data , 0};

    switch(data) {
        case '1': // LFO frequency/rate
        	return_data[2] = SynthParameters.lfo_frequency;
            break;
        case '2': // LFO output waveform
        	return_data[2] = SynthParameters.lfo_waveform;
            break;
        default:  // do nothing on invalid command

            break;
    }

    CDC_Transmit_FS(return_data, 3); // transmit return message

    if (sys.midi_tx_en == 1) { // send return message if enabled
        HAL_UART_Transmit(&huart2, return_data, 3, 1000);
    }

	return;
}

// ===========================================================================================================
// P preset function

int Preset_Command_Handler(int address, int data){

    switch(address) {
        case '1': // preset save function

        	// preset save function

        	return 0;
        case '2': // preset load function

        	// preset load function

        	return 0;
        default:
            // do nothing on invalid command

        	//Command_Error();

        	return 1;
    }

    return 1;
}

// ===========================================================================================================
// X - oscillator 1 function

void Oscillator_1_Command_Handler(int address, int data){

    switch(address) {
        case '1': // oscillator semitone offset

        	SynthParameters.note_offset1 = Command_Range_Check(data, SynthParameters.note_offset1, 24);

            break;
        case '2': // oscillator detune

        	SynthParameters.detune_osc1 = Command_Range_Check(data, SynthParameters.detune_osc1, 200);

            break;
        case '3': // oscillator waveform selection

        	SynthParameters.oscillator1 = Command_Range_Check(data, SynthParameters.oscillator1, 4);

            break;
        case '4': // volume setting

        	SynthParameters.volume_osc1 = Command_Range_Check(data, SynthParameters.volume_osc1, 63);

            break;
        case '5': // duty cycle for PWM waveform

            float temp = (float) data/100;
        	SynthParameters.duty_cycle1 = Command_Range_Check_Float(temp, SynthParameters.duty_cycle1, 1.0);

            break;
        default:
            // do nothing on invalid command

        	//Command_Error();

            break;
    }

    return;
}

// ===========================================================================================================
// Y - oscillator 2 function

void Oscillator_2_Command_Handler(int address, int data){

    switch(address) {
        case '1': // oscillator semitone offset

        	SynthParameters.note_offset2 = Command_Range_Check(data, SynthParameters.note_offset2, 24);

            break;
        case '2': // oscillator detune

        	SynthParameters.detune_osc2 = Command_Range_Check(data, SynthParameters.detune_osc2, 200);

            break;
        case '3': // oscillator waveform selection

        	SynthParameters.oscillator2 = Command_Range_Check(data, SynthParameters.oscillator2, 4);

            break;
        case '4': // volume setting

        	SynthParameters.volume_osc2 = Command_Range_Check(data, SynthParameters.volume_osc2, 63);

            break;
        case '5': // duty cycle for PWM waveform

            float temp = (float) data/100;
        	SynthParameters.duty_cycle2 = Command_Range_Check_Float(temp, SynthParameters.duty_cycle2, 1.0);

            break;
        default:
            // do nothing on invalid command

        	//Command_Error();

            break;
    }

    return;
}

// ===========================================================================================================
// Z - frequency modulation function

void Frequency_Modulation_Command_Handler(int address, int data){

	float temp;

    switch(address) {
        case '1': // oscillator 1 FM intensity

            temp = (float) data/100;
        	SynthParameters.fm_intensity1 = Command_Range_Check_Float(temp, SynthParameters.fm_intensity1, 1.0);

            break;
        case '2': // oscillator 1 FM harmonic

        	temp = (float) data/100;
        	SynthParameters.fm_harmonic1 = Command_Range_Check(data, SynthParameters.fm_harmonic1, 200);

            break;
        case '3': // oscillator 2 FM intensity

            temp = (float) data/100;
        	SynthParameters.fm_intensity2 = Command_Range_Check_Float(temp, SynthParameters.fm_intensity2, 1.0);

            break;
        case '4': // oscillator 2 FM harmonic

        	temp = (float) data/100;
        	SynthParameters.fm_harmonic2 = Command_Range_Check(data, SynthParameters.fm_harmonic2, 200);

            break;
        case '5': // oscillator 1 enable

        	SynthParameters.fm1_enable = Command_Range_Check(data, SynthParameters.fm1_enable, 1);

            break;
        case '6': // oscillator 2 enable

        	SynthParameters.fm2_enable = Command_Range_Check(data, SynthParameters.fm2_enable, 1);

            break;
        default:
            // do nothing on invalid command

        	//Command_Error(1);

            break;
    }

    return;
}

float Command_Range_Check_Float(float data, float variable, float max_value){

	if(data <= max_value){  // check to see if input is valid
		return data;
	}
	else{
		Command_Error();
		return variable;
	}

}
