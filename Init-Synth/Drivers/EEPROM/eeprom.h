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

// EEPROM ADDRESS (8bits)
#define EEPROM_ADDR 0xA0

// define the page size and number of pages
#define PAGE_SIZE    8     // in bytes
#define PAGE_NUM     32    // number of pages
#define PAGE_OFFSET  2     // starting index for EEPROM data

// ===========================================================================================================
// function definitions

int EEPROM_Write_Page(int page_number, uint8_t *data);
int EEPROM_Read_Page(int page_number, uint8_t *data);
int EEPROM_Write_Byte(uint8_t byte_address, uint8_t *data);
int EEPROM_Read_Byte(uint8_t byte_address, uint8_t *data);

#endif /* EEPROM_EEPROM_H_ */
