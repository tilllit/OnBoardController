/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define F_ACT_Pin GPIO_PIN_0
#define F_ACT_GPIO_Port GPIOA
#define DIR_ACT_Pin GPIO_PIN_1
#define DIR_ACT_GPIO_Port GPIOA
#define CS_ENC_Pin GPIO_PIN_4
#define CS_ENC_GPIO_Port GPIOA
#define SCK_ENC_Pin GPIO_PIN_5
#define SCK_ENC_GPIO_Port GPIOA
#define MISO_ENC_Pin GPIO_PIN_6
#define MISO_ENC_GPIO_Port GPIOA
#define MOSI_ENC_Pin GPIO_PIN_7
#define MOSI_ENC_GPIO_Port GPIOA
#define SCL_TOF_Pin GPIO_PIN_6
#define SCL_TOF_GPIO_Port GPIOB
#define SDA_TOF_Pin GPIO_PIN_7
#define SDA_TOF_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
