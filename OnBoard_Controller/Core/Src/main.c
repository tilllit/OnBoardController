/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include "math.h"

#include "Data.h"		// Data structure
#include "AMT22.h"		// Angular sensor (Encoder)
#include "ToF.h"		// Distance sensor (ToF)

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// ###############################################
// ########### Activate UART debugging ###########
// ###############################################

//#define UART_DEBUG


// ###############################################
// ### Define ToF in polling or interrupt mode ###
// ###############################################

//#define ToF_Polling
#define ToF_Interrupt

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

extern Sensors SENS;						// Sensor data

extern volatile uint8_t ToF_EventDetected;	// ToF RX data flag

uint8_t ENCFlag = 1;						// Encoder RX data flag

uint8_t TXFlag = 1;							// SPI RX data flag

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t spi_buf[8] = {0,0,0,0,0};

uint8_t tx_buf[4]  = { (uint8_t) 1, (uint8_t)2, (uint8_t)3, (uint8_t)4};

uint8_t *pTX_Buff  = (uint8_t*) &tx_buf;		// Pointer to SPI TX buffer

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_SPI2_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */


  // Initialize Sensors
  MX_TOF_Init();
  HAL_TIM_Base_Start(&htim1);

  // Encoder offset compensation
  setZeroSPI(&hspi1, CS_ENC_GPIO_Port, CS_ENC_Pin, &htim1);


#ifdef ToF_Interrupt	// ToF (interrupt)
  	  ToF_Start_IT();
#endif


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  // #################################
	  // ##########   SPI-COM   ##########
	  // #################################


	  if (TXFlag == 1)		// SPI COM-Flag (TX-RX-complete)
	  {


#ifdef ToF_Interrupt		// ToF (interrupt)

	  if (ToF_EventDetected != 0)
	  {
		  // collect data when flag is set
		  SENS.Distance = (uint16_t) ToF_Process_IT();
	  }
#endif


	  if(ENCFlag == 1)		// Poll encoder data
	  {
		  ENCFlag = 0;
		  SENS.Angle = getPositionSPI(&hspi1, CS_ENC_GPIO_Port, CS_ENC_Pin, (uint8_t) 0xFFFF, &htim1);
	  }

	  // Data preparation
	  uint16_t deg	= round(SENS.Angle);
	  uint16_t x 	= SENS.Distance;

	  // Filling bytes of TX buffer
	  tx_buf[0] = (uint8_t) deg;
	  tx_buf[1] =  deg >> 8;
	  tx_buf[2] = (uint8_t) x;
	  tx_buf[3] =  x >> 8;

	  // Reset flags
	  TXFlag = 0;
	  ENCFlag = 1;

	  // DMA SPI - transmit receive
	  HAL_SPI_TransmitReceive_DMA(&hspi2, (uint8_t *) pTX_Buff, (uint8_t*) &spi_buf, 4);



// ##### Optional UART output #####
#ifdef UART_DEBUG
	  printf("Angle:%0.2f, "
			  "Distance:%i\r\n",
			  SENS.Angle,
			  SENS.Distance);
#endif

	  }

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart1, (uint8_t*) ptr, len, HAL_MAX_DELAY);

	return len;
}







void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	TXFlag = 1;
}










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
