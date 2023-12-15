/*
 * ToF_pin_conf.c
 *
 *  Created on: Dec 14, 2023
 *      Author: tillkorsmeier
 */


#include "ToF_pin_conf.h"


extern volatile uint8_t ToF_EventDetected;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == TOF_INT_EXTI_PIN)
  {
    ToF_EventDetected = 1;
  }
}
