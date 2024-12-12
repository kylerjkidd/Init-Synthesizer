/*
 * preset.h
 *
 *  Created on: Dec 9, 2024
 *      Author: Implacable
 */

#ifndef INC_PRESET_H_
#define INC_PRESET_H_

#include "stdint.h"

int Synth_Settings_Readback();

int Preset_Init(int preset_slot);
int Preset_Clear();
void Preset_Load();
int Preset_Write(int preset_slot);
int Preset_Read(int preset_slot);

uint8_t XOR_Checksum(uint8_t *data, int length);
uint8_t Readback_Checksum(int preset_slot);

#endif /* INC_PRESET_H_ */
