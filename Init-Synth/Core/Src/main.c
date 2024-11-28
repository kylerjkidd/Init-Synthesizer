/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "i2s.h"
#include "spi.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#define ARM_MATH_CM4

#include "math.h"
#include "arm_math.h"
#include "usbd_cdc_if.h"
#include "string.h"
#include "stdbool.h"

#include "audiodac.h"
#include "gpioxpndr.h"
#include "synth.h"
#include "serial.h"
#include "midi.h"
#include "note.h"
#include "eeprom.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define RED_LED_PORT GPIOA
#define RED_LED_PIN  GPIO_PIN_10

#define GRN_LED_PORT GPIOA
#define GRN_LED_PIN  GPIO_PIN_9

#define TP0_PORT GPIOB
#define TP0_PIN  GPIO_PIN_5

#define USB_RENUM_PORT GPIOB
#define USB_RENUM_PIN  GPIO_PIN_5

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// communication buffers
uint8_t usb_vcp_buffer[64];
uint8_t midi_buf[3];
uint8_t temp_buf[3];
int temp_buf_index = 0;

// communication status flags
int usb_data_present = 0;
int midi_data_present = 0;

// midi variables
int note_status_bit = 0;
int midi_note_input = 0;
int midi_note_velocity = 0;

int gate = 0;
int velocity_enable = 0;

// GPIO expander output ports
uint8_t gpio_mask1 = 0x00;
uint8_t gpio_mask2 = 0x00;
//uint16_t gpio_reg = 0x0000;
//int fm1_enable = 0;
//int fm2_enable = 0;






/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2S2_Init();
  MX_I2C3_Init();
  MX_SPI3_Init();
  MX_USB_Device_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, SET); // enable analog power supply

  DAC_Register_Init();

  DAC_Register_Write(0x40, 0x3F);
  DAC_Register_Write(0x44, 0x3F);

  DAC_Register_Write(0x41, 0x3F);
  DAC_Register_Write(0x45, 0x3F);

  GPIO_Register_Init();

  Synth_Reset_Initalize();

  //HAL_Delay(5000);

  //Synth_Initialize_Preset();

  //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, SET); // trigger gate
  //HAL_Delay(500);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, RESET); // set gate low
  //HAL_GPIO_WritePin(TP0_PORT, TP0_PIN, SET); // test point

  HAL_GPIO_WritePin(RED_LED_PORT, RED_LED_PIN, SET); // red

  //float input = 3.14;

  //Convert_Float_to_Binary(input);
  //Convert_Binary_to_Float();

  //Digital_Pot_Wiper_Set(2,129,0);

  //CDC_Transmit_FS((uint8_t *) data, strlen (data));

  //HAL_UART_Receive(&huart2, temp_buf, 5);

  HAL_UART_Receive_IT(&huart2, temp_buf , 1); // set up interrupt for MIDI/serial input
  //HAL_UART_Receive_IT(&huart3, midi_buf, 3);

  uint8_t data[] = "INIT-READY\n";
  HAL_UART_Transmit(&huart2, data, 10, 1000);

  //HAL_UART_Receive(&huart2, data, 3, HAL_MAX_DELAY);
  //HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9); // green
  //GPIO_Init();
  //GPIO_Register_Init();

  //gpio_reg = GPIO_State_Change(0, gpio_reg, 2, true);

  //gpio_reg = GPIO_State_Change(1, gpio_reg, 2, true);

  //void GPIO_State_Change(int reg_bank, uint8_t *gpio_reg, uint8_t bit_position, bool state);

  //gpio_mask1 = GPIO_State_Change(0, gpio_mask1, 5, true);

  //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, SET); // enable analog power supply

  //UART_Test();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  //int check = 0;
	  //CDC_Transmit_FS((uint8_t *) data, strlen (data));

/*
		if (temp[0] == '\n')
		{
	        	memcpy (ser_buf, midi_buf, indx);
			indx = 0;
		}
*/

//	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9); // green
//	  //HAL_Delay(1000);
//	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8); // trigger gate
//	  HAL_GPIO_TogglePin(TP0_PORT, TP0_PIN); // test point
//
//	  HAL_Delay(10000);
//
//	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8); // trigger gate
//	  HAL_GPIO_TogglePin(TP0_PORT, TP0_PIN); // test point
//	  HAL_Delay(10000);


	  //HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9); // green
	  //HAL_Delay(1000);

	  //gate = 1;
	  //Waveform_Synthesis_Handler(80, 127);

//	  while(check!=1){
//		  check = Serial_Command_Handler_t();
//	  }

	  //while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)!=1) {};

	  //check = Serial_Command_Handler_t();

//	  if(check == 1){
//
//		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
//		  HAL_Delay(2000);
//		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
//		  check = 0;
//		  memset (usb_vcp_buffer, '\0', 64); // to clear buffer of old data
//
//	  }

//	  if (temp[0] == '\n') // look for newline on serial
//	  {
//		  memcpy (cmd_buf, midi_buf, indx);
//		  indx = 0;
//	  }


	  if((midi_data_present == 1)){
		  MIDI_Decode_Handler();
	  }

	  if((usb_data_present == 1)){
		  Serial_Command_Handler();
	  }

	  Gate_Control(note_status_bit == 1 ? 1 : 0);

//
//	  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==1){
//		  CDC_Transmit_FS((uint8_t *) data, strlen (data));
//		  HAL_Delay(500);
//	  }

	  //loop();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 28;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
