/*
 * EEPROM.h
 *
 *  Created on: May 29, 2024
 *      Author: Implacable
 */

#ifndef EEPROM_EEPROM_H_
#define EEPROM_EEPROM_H_

#include "stdint.h"

#include "synth.h"
#include "system.h"

extern I2C_HandleTypeDef hi2c3;

// EEPROM ADDRESS (8bits)
#define EEPROM_ADDR 0xA0

// define the page size and number of pages
#define PAGE_SIZE    8     // in bytes
#define PAGE_NUM     32    // number of pages
#define PAGE_OFFSET  2     // starting index for EEPROM data

// ===========================================================================================================
// function definitions

int Preset_Write_test(int preset_slot);

int Preset_Init(int preset_slot);
int Preset_Write(int preset_slot);
int Preset_Read(int preset_slot);
uint8_t XOR_Checksum(uint8_t *data, int length);
uint8_t Readback_Checksum(int preset_slot);

int EEPROM_Write_Page(int page_number, uint8_t *data);
int EEPROM_Read_Page(int page_number, uint8_t *data);

#endif /* EEPROM_EEPROM_H_ */
