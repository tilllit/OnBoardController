/*
 * Data.h
 *
 *  Created on: Dec 15, 2023
 *      Author: tillkorsmeier
 */

#ifndef SRC_DATA_H_
#define SRC_DATA_H_


#include "stm32f4xx_hal.h"

typedef struct{

	// Attributes
	uint16_t	Distance;
	float		Angle;

} Sensors;


#endif /* SRC_DATA_H_ */
