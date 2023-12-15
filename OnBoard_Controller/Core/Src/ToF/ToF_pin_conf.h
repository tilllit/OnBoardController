/*
 * ToF_pin_conf.h
 *
 *  Created on: Dec 14, 2023
 *      Author: tillkorsmeier
 */

#ifndef SRC_TOF_TOF_PIN_CONF_H_
#define SRC_TOF_TOF_PIN_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported symbols ----------------------------------------------------------*/
#define TOF_INT_EXTI_PIN    (GPIO_PIN_5)
#define TOF_INT_EXTI_PORT   (GPIOB)

#define VL53L4A1_XSHUT_C_PIN   (GPIO_PIN_4)
#define VL53L4A1_XSHUT_C_PORT  (GPIOB)

#ifdef __cplusplus
}
#endif


#endif /* SRC_TOF_TOF_PIN_CONF_H_ */
