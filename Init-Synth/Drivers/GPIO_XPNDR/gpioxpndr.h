/*
 * gpioxpndr.h
 *
 *  Created on: Sep 8, 2023
 *      Author: Implacable
 */

#ifndef GPIO_XPNDR_GPIOXPNDR_H_
#define GPIO_XPNDR_GPIOXPNDR_H_

#include "stdbool.h"

// EEPROM ADDRESS (8bits)
#define GPIO_ADDR      0xEE

#define GPIO_IN_PORT0  0x00
#define GPIO_IN_PORT1  0x01
#define GPIO_OUT_PORT0 0x02
#define GPIO_OUT_PORT1 0x03
#define GPIO_POL_PORT0 0x04
#define GPIO_POL_PORT1 0x05
#define GPIO_CFG_PORT0 0x06
#define GPIO_CFG_PORT1 0x07

void GPIO_Register_Write(uint8_t register_addr, uint8_t data);
uint8_t GPIO_Register_Read(uint8_t register_addr);
void GPIO_Register_Init();
uint16_t GPIO_State_Change(int reg_bank, uint16_t gpio_reg, int bit_position, bool state);

#endif /* GPIO_XPNDR_GPIOXPNDR_H_ */
