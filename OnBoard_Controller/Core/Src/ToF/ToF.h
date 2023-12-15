/*
 * ToF.h
 *
 *  Created on: Dec 14, 2023
 *      Author: tillkorsmeier
 */


/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef SRC_TOF_H_
#define SRC_TOF_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported defines ----------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void MX_TOF_Init(void);
long MX_TOF_Process(void);

#ifdef __cplusplus
}
#endif

#endif /* SRC_TOF_H_ */
