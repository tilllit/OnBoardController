/*
  AMT22.h - STM32 library for ATM22 series absolute encoders by CUI Devices.
  Created by Simone Di Blasi, December 2020.
*/


#ifndef AMT22_H_
#define AMT22_H_

#include "stdint.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_tim.h"





void setCSLine (GPIO_TypeDef* encoderPort, uint16_t encoderPin, GPIO_PinState csLine);

uint8_t spiWriteRead(SPI_HandleTypeDef *hspi, uint8_t sendByte, GPIO_TypeDef* encoderPort, uint16_t encoderPin, uint8_t releaseLine, TIM_HandleTypeDef *timer);

float getPositionSPI(SPI_HandleTypeDef *hspi, GPIO_TypeDef* encoderPort, uint16_t encoderPin, uint8_t resolution, TIM_HandleTypeDef *timer);

void setZeroSPI(SPI_HandleTypeDef *hspi, GPIO_TypeDef* encoderPort, uint16_t encoderPin, TIM_HandleTypeDef *timer);

void resetAMT22(SPI_HandleTypeDef *hspi, GPIO_TypeDef* encoderPort, uint16_t encoderPin, TIM_HandleTypeDef *timer);

void delay(TIM_HandleTypeDef *timer, uint32_t delayTime);

float calAngle(uint16_t curPos);


#endif /* SRC_AMT22_H_ */
