/*
 * audio_dac.h
 *
 *  Created on: Jul 28, 2023
 *      Author: Implacable
 */

#ifndef AUDIO_DAC_AUDIODAC_H_
#define AUDIO_DAC_AUDIODAC_H_

#define DAC_ADDR 0x8E

#define DAC_REG_VOL_L 			0x40
#define DAC_REG_VOL_R 			0x41
#define DAC_REG_ATTEN_L			0x44
#define DAC_REG_ATTEN_R 		0x45
#define DAC_REG_DEMP_DGC 		0x46
#define DAC_REG_ANALOG_MIXER 	0x48
#define DAC_REG_DAC_PWR 		0x49
#define DAC_REG_ANALOG_CONFIG 	0x4A
#define DAC_REG_HPA_DETECT 		0x4B
#define DAC_REG_SHDN_STATUS 	0x4D
#define DAC_REG_ANALOG_GAIN_EN 	0x52
#define DAC_REG_MASTER_SLAVE 	0x54
#define DAC_REG_RESET 			0x55
#define DAC_REG_SAMPLE_RATE 	0x56
#define DAC_REG_ANALOG_MUX 		0x57
#define DAC_REG_ANALOG_MIX_SW 	0x58
#define DAC_REG_ANALOG_GAIN 	0x59
#define DAC_REG_MIC_BOOST 		0x5A
#define DAC_REG_BASS_BOOST 		0x5C
#define DAC_REG_MID_BOOST 		0x5D
#define DAC_REG_TREBLE_BOOST 	0x5E
#define DAC_REG_3D_SOUND 		0x5F
#define DAC_REG_MONO_MIX		0x60

void DAC_Register_Write(uint8_t register_addr, uint8_t data);
uint8_t DAC_Register_Read(uint8_t register_addr);
void DAC_Register_Init();

#endif /* AUDIO_DAC_AUDIODAC_H_ */
