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

uint8_t Readback_Checksum(int preset_slot){

	uint8_t i2c_buf[1];
	uint8_t checksum_address = (0x3F + (0x30 * preset_slot));

	HAL_I2C_Mem_Read(&hi2c3, EEPROM_ADDR, checksum_address, I2C_MEMADD_SIZE_8BIT, i2c_buf, 1, 10);

    return i2c_buf[0];
}

int EEPROM_Write_Page(int page_number, uint8_t *data){

	// calculate page start address
    uint8_t start_address = page_number * PAGE_SIZE;

    // write page data
    HAL_StatusTypeDef result = HAL_I2C_Mem_Write(&hi2c3, EEPROM_ADDR, start_address, I2C_MEMADD_SIZE_8BIT, data, PAGE_SIZE, 10);

    // wait for EEPROM write cycle to complete (typically 5ms)
    HAL_Delay(5);

    return (result == HAL_OK) ? 0 : 1;
}

int EEPROM_Read_Page(int page_number, uint8_t *data){

    // calculate page start address
    uint8_t start_address = page_number * PAGE_SIZE;

    // read page data
    HAL_StatusTypeDef result = HAL_I2C_Mem_Read(&hi2c3, EEPROM_ADDR, start_address, I2C_MEMADD_SIZE_8BIT, data, PAGE_SIZE, 10);

    return (result == HAL_OK) ? 0 : 1;
}

int EEPROM_Write_Byte(uint8_t byte_address, uint8_t *data){

    // write byte data
    HAL_StatusTypeDef result = HAL_I2C_Mem_Write(&hi2c3, EEPROM_ADDR, byte_address, I2C_MEMADD_SIZE_8BIT, data, 1, 10);

    // wait for EEPROM write cycle to complete (typically 5ms)
    HAL_Delay(5);

    return (result == HAL_OK) ? 0 : 1;
}

int EEPROM_Read_Byte(uint8_t byte_address, uint8_t *data){

    // read byte data
    HAL_StatusTypeDef result = HAL_I2C_Mem_Read(&hi2c3, EEPROM_ADDR, byte_address, I2C_MEMADD_SIZE_8BIT, data, 1, 10);

    return (result == HAL_OK) ? 0 : 1;
}
