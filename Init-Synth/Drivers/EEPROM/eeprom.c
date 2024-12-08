/*
 * EEPROM.c
 *
 *  Created on: May 29, 2024
 *      Author: Implacable
 */


#include "i2c.h"
#include "stdint.h"
#include "string.h"

#include "eeprom.h"
#include "system.h"
#include "synth.h"

// write preset to EEPROM and check result
int Preset_Write(int preset_slot){

	int error_check = 1;
	uint8_t page_buf[8];
	uint8_t preset_buf[48];

	// copy synth parameters into buffer
    memcpy(preset_buf, &SynthParameters, sizeof(Synth));

    // copy remaining system settings
    preset_buf[44] = sys.serial_cmd_echo;
    preset_buf[45] = sys.midi_tx_pol;
    preset_buf[46] = sys.midi_tx_en;

    sys.checksum = XOR_Checksum(preset_buf, 48);
    preset_buf[47] = sys.checksum;

	for(int page = 0; page < 6; page++){

		memcpy(page_buf, &preset_buf[8 * page], 8); // copy one page of data from buffer
		error_check = EEPROM_Write_Page(PAGE_OFFSET + (preset_slot * 6) + page, page_buf);

	}

	//int check = Readback_Checksum(preset_slot);

	if(sys.checksum == Readback_Checksum(preset_slot)){
		error_check = 0;
	}

    return error_check;
}

// calculate an XOR checksum for an array of data
uint8_t XOR_Checksum(uint8_t *data, int length) {

    uint8_t checksum = 0x00;

    for (int i = 0; i < length; i++) {
        checksum ^= data[i]; // XOR each byte
    }

    return checksum;
}

// read and return checksum value for data
uint8_t Readback_Checksum(int preset_slot){

	uint8_t i2c_buf[1];
	uint8_t checksum_address = (0x3F + (0x30 * preset_slot));

	HAL_I2C_Mem_Read(&hi2c3, EEPROM_ADDR, checksum_address, I2C_MEMADD_SIZE_8BIT, i2c_buf, 1, 10);

    return i2c_buf[0];
}

// write a single page to the EEPROM
int EEPROM_Write_Page(int page_number, uint8_t *data){

	// calculate page start address
    uint8_t start_address = page_number * PAGE_SIZE;

    // write page data
    HAL_StatusTypeDef result = HAL_I2C_Mem_Write(&hi2c3, EEPROM_ADDR, start_address, I2C_MEMADD_SIZE_8BIT, data, PAGE_SIZE, 10);

    // wait for EEPROM write cycle to complete (typically 5ms)
    HAL_Delay(5);

    return (result == HAL_OK);
}




