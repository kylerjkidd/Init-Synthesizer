/*
 * gpioxpndr.c
 *
 *  Created on: Sep 8, 2023
 *      Author: Implacable
 */

#include "stdbool.h"

#include "i2c.h"
#include "gpioxpndr.h"

void GPIO_Register_Write(uint8_t register_addr, uint8_t data){

	uint8_t i2c_buf[2];

	i2c_buf[0] = data;

	HAL_I2C_Mem_Write(&hi2c3, GPIO_ADDR, register_addr, I2C_MEMADD_SIZE_8BIT, i2c_buf, 1, 10);

	return;
}

uint8_t GPIO_Register_Read(uint8_t register_addr){

	uint8_t i2c_buf[2];

	HAL_I2C_Mem_Read(&hi2c3, GPIO_ADDR, register_addr, I2C_MEMADD_SIZE_8BIT, i2c_buf, 1, 10);

	return i2c_buf[0];
}

void GPIO_Register_Init(){

	GPIO_Register_Write(GPIO_CFG_PORT0, 0x00); // configure all of port 0 as output
	GPIO_Register_Write(GPIO_CFG_PORT1, 0x00); // configure all of port 1 as output

	GPIO_Register_Write(GPIO_OUT_PORT0, 0x00); // set initial state to all low
	GPIO_Register_Write(GPIO_OUT_PORT1, 0x00); // set initial state to all low

	return;
}

/*uint8_t GPIO_State_Change(int reg_bank, uint8_t gpio_reg, uint8_t bit_position, bool state){

    if (bit_position > 7) {
        // return if bit position is out of range for an 8-bit integer
        return;
    }

    if (state) {
        gpio_reg |=  (1 << bit_position); // set the bit
    } else {
        gpio_reg &= ~(1 << bit_position); // reset (clear) the bit
    }

    uint8_t selected_bank = (reg_bank) ? GPIO_OUT_PORT1 : GPIO_OUT_PORT0;

    GPIO_Register_Write(selected_bank, gpio_reg);

    return gpio_reg;
}*/

uint16_t GPIO_State_Change(int reg_bank, uint16_t gpio_reg, int bit_position, bool state) {
    if (bit_position > 7) {
        return 0xFFFF; // return an error code if bit position is out of range for an 8-bit section
    }

    if (reg_bank == 0) {
        // modify the lower byte (bits 0-7)
        if (state) {
            gpio_reg |= (1 << bit_position); // set the bit in the lower byte
        } else {
            gpio_reg &= ~(1 << bit_position); // clear the bit in the lower byte
        }

        // write the modified low byte to the corresponding GPIO register
        uint8_t gpio_reg_low = gpio_reg & 0xFF;
        GPIO_Register_Write(GPIO_OUT_PORT0, gpio_reg_low);

    } else if (reg_bank == 1) {
        // modify the higher byte (bits 8-15)
        if (state) {
            gpio_reg |= (1 << (bit_position + 8)); // set the bit in the higher byte
        } else {
            gpio_reg &= ~(1 << (bit_position + 8)); // clear the bit in the higher byte
        }

        // write the modified high byte to the corresponding GPIO register
        uint8_t gpio_reg_high = (gpio_reg >> 8) & 0xFF;
        GPIO_Register_Write(GPIO_OUT_PORT1, gpio_reg_high);
    } else {
        return 0xFFFF; // return error code if reg_bank is out of range
    }

    return gpio_reg; // return the modified 16-bit register value
}

void GPIO_Init(){

	uint8_t i2c_buf[1];

	i2c_buf[0] = GPIO_CFG_PORT0;
	i2c_buf[1] = 0x00;

	HAL_I2C_Master_Transmit(&hi2c3, GPIO_ADDR, i2c_buf, 2, HAL_MAX_DELAY);

	i2c_buf[0] = GPIO_CFG_PORT1;
	i2c_buf[1] = 0x00;

	HAL_I2C_Master_Transmit(&hi2c3, GPIO_ADDR, i2c_buf, 2, HAL_MAX_DELAY);

	i2c_buf[0] = GPIO_OUT_PORT0;
	i2c_buf[1] = 0x03;

	HAL_I2C_Master_Transmit(&hi2c3, GPIO_ADDR, i2c_buf, 2, HAL_MAX_DELAY);

	i2c_buf[0] = GPIO_OUT_PORT1;
	i2c_buf[1] = 0x00;

	HAL_I2C_Master_Transmit(&hi2c3, GPIO_ADDR, i2c_buf, 2, HAL_MAX_DELAY);

	return;

}