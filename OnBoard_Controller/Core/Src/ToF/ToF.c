/*
 * ToF.c
 *
 *  Created on: Dec 14, 2023
 *      Author: tillkorsmeier
 */


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
//#include "app_tof.h"
#include "ToF.h"
#include "main.h"
#include <stdio.h>

#include "53l4a1_ranging_sensor.h"
#include "ToF_pin_conf.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define TIMING_BUDGET (30U) /* 10 ms < TimingBudget < 200 ms */
#define POLLING_PERIOD (250U) /* refresh rate for polling mode (ms, shall be consistent with TimingBudget value) */

//#define App_Print		// Activate application print functionality

/* Private variables ---------------------------------------------------------*/
static RANGING_SENSOR_Capabilities_t Cap;
static RANGING_SENSOR_ProfileConfig_t Profile;
static int32_t status = 0;
static volatile uint8_t PushButtonDetected = 0;
volatile uint8_t ToF_EventDetected = 0;

/* Private function prototypes -----------------------------------------------*/
static void MX_53L4A1_SimpleRanging_Init(void);
static long MX_53L4A1_SimpleRanging_Process(void);

#ifdef App_Print
static void print_result(RANGING_SENSOR_Result_t *Result);
static int32_t decimal_part(float_t x);
#endif

void MX_TOF_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN TOF_Init_PreTreatment */

  /* USER CODE END TOF_Init_PreTreatment */

  /* Initialize the peripherals and the TOF components */

  MX_53L4A1_SimpleRanging_Init();

  /* USER CODE BEGIN TOF_Init_PostTreatment */

  /* USER CODE END TOF_Init_PostTreatment */
}

/*
 * LM background task
 */
long MX_TOF_Process(void)
{
  /* USER CODE BEGIN TOF_Process_PreTreatment */

  /* USER CODE END TOF_Process_PreTreatment */

  return MX_53L4A1_SimpleRanging_Process();

  /* USER CODE BEGIN TOF_Process_PostTreatment */

  /* USER CODE END TOF_Process_PostTreatment */
}

static void MX_53L4A1_SimpleRanging_Init(void)
{
  /* Initialize Virtual COM Port */
  //BSP_COM_Init(COM1);

  /* Initialize button */
  //BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

  /* reset XSHUT (XSDN) pin */
  HAL_GPIO_WritePin(VL53L4A1_XSHUT_C_PORT, VL53L4A1_XSHUT_C_PIN, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_GPIO_WritePin(VL53L4A1_XSHUT_C_PORT, VL53L4A1_XSHUT_C_PIN, GPIO_PIN_SET);
  HAL_Delay(2);

  printf("53L4A1 Simple Ranging demo application\n");
  status = VL53L4A1_RANGING_SENSOR_Init(VL53L4A1_DEV_CENTER);

  if (status != BSP_ERROR_NONE)
  {
    printf("VL53L4A1_RANGING_SENSOR_Init failed\n");
    while (1);
  }
}

static long MX_53L4A1_SimpleRanging_Process(void)
{
  uint32_t Id;

  RANGING_SENSOR_Result_t Result;

  VL53L4A1_RANGING_SENSOR_ReadID(VL53L4A1_DEV_CENTER, &Id);
  VL53L4A1_RANGING_SENSOR_GetCapabilities(VL53L4A1_DEV_CENTER, &Cap);

  Profile.RangingProfile = VL53L4CD_PROFILE_CONTINUOUS;
  Profile.TimingBudget = TIMING_BUDGET;
  Profile.Frequency = 0;			// Induces intermeasurement period, NOT USED for normal ranging
  Profile.EnableAmbient = 1; 		// Enable: 1, Disable: 0
  Profile.EnableSignal = 1; 		// Enable: 1, Disable: 0

  // set the profile if different from default one
  VL53L4A1_RANGING_SENSOR_ConfigProfile(VL53L4A1_DEV_CENTER, &Profile);

  status = VL53L4A1_RANGING_SENSOR_Start(VL53L4A1_DEV_CENTER, RS_MODE_BLOCKING_CONTINUOUS);

  if (status != BSP_ERROR_NONE)
  {
    printf("VL53L4A1_RANGING_SENSOR_Start failed\n");
    while (1);
  }

  //while (1)
  {
    /* polling mode */
    status = VL53L4A1_RANGING_SENSOR_GetDistance(VL53L4A1_DEV_CENTER, &Result);

    if (status == BSP_ERROR_NONE)
    {
      //print_result(&Result);
      printf("Distance: %li, \r\n", (long)Result.ZoneResult[0].Distance[0]);
    }

    //HAL_Delay(POLLING_PERIOD);
    return (long)Result.ZoneResult[0].Distance[0];
  }
}








void ToF_Start_IT()
{
	  uint32_t Id;

	  VL53L4A1_RANGING_SENSOR_ReadID(VL53L4A1_DEV_CENTER, &Id);
	  VL53L4A1_RANGING_SENSOR_GetCapabilities(VL53L4A1_DEV_CENTER, &Cap);

	  Profile.RangingProfile = VL53L4CD_PROFILE_CONTINUOUS;
	  Profile.TimingBudget = TIMING_BUDGET;
	  Profile.Frequency = 0; /* Induces intermeasurement period, NOT USED for normal ranging */
	  Profile.EnableAmbient = 1; /* Enable: 1, Disable: 0 */
	  Profile.EnableSignal = 1; /* Enable: 1, Disable: 0 */

	  /* set the profile if different from default one */
	  VL53L4A1_RANGING_SENSOR_ConfigProfile(VL53L4A1_DEV_CENTER, &Profile);

	  status = VL53L4A1_RANGING_SENSOR_Start(VL53L4A1_DEV_CENTER, RS_MODE_ASYNC_CONTINUOUS);

	  if (status != BSP_ERROR_NONE)
	  {
	    printf("VL53L4A1_RANGING_SENSOR_Start failed\n");
	    while (1);
	  }
}

long ToF_Process_IT(void)
{

	ToF_EventDetected = 0;
	RANGING_SENSOR_Result_t Result;
    status = VL53L4A1_RANGING_SENSOR_GetDistance(VL53L4A1_DEV_CENTER, &Result);

    if (status == BSP_ERROR_NONE)
    {
      return (long)Result.ZoneResult[0].Distance[0];
    }
    else
    {
    	return 0;
    }
}










#ifdef App_Print

// Printing measured data
static void print_result(RANGING_SENSOR_Result_t *Result)
{
  uint8_t i;
  uint8_t j;

  for (i = 0; i < RANGING_SENSOR_MAX_NB_ZONES; i++)
  {
    printf("\nTargets = %lu", (unsigned long)Result->ZoneResult[i].NumberOfTargets);

    for (j = 0; j < Result->ZoneResult[i].NumberOfTargets; j++)
    {
      printf("\n |---> ");

      printf("Status = %ld, Distance = %5ld mm ",
             (long)Result->ZoneResult[i].Status[j],
             (long)Result->ZoneResult[i].Distance[j]);

      if (Profile.EnableAmbient)
        printf(", Ambient = %ld.%02ld kcps/spad",
               (long)Result->ZoneResult[i].Ambient[j],
               (long)decimal_part(Result->ZoneResult[i].Ambient[j]));

      if (Profile.EnableSignal)
        printf(", Signal = %ld.%02ld kcps/spad",
               (long)Result->ZoneResult[i].Signal[j],
               (long)decimal_part(Result->ZoneResult[i].Signal[j]));
    }
  }
  printf("\n");
}

// Hilfsfunktion
static int32_t decimal_part(float_t x)
{
  int32_t int_part = (int32_t) x;
  return (int32_t)((x - int_part) * 100);
}

#endif


#ifdef __cplusplus
}
#endif
