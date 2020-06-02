/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f7xx_hal.h"

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
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOB
#define RESET_Pin GPIO_PIN_6
#define RESET_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

typedef enum
{
  MOUSE_RELATIVE = 0,
  MOUSE_ABSOLUTE,
  MOUSE_KEYCODE,
  MOUSE_DISABLED
}
MouseStateTypeDef;

typedef struct _MouseStatus
{
 uint16_t absolute_mouse_x;
 uint16_t absolute_mouse_y;
 uint8_t absolute_mouse_btn;

 uint8_t mouse_threshold_x;
 uint8_t mouse_threshold_y;

 uint8_t mouse_scale_x;
 uint8_t mouse_scale_y;

 uint16_t absolute_mouse_max_x;
 uint16_t absolute_mouse_max_y;
}
MouseStatusTypeDef;



typedef enum
{
  JOYSTICK_EVENT_REPORTING= 0,
  JOYSTICK_INTERROGATION_MODE,
  JOYSTICK_KEYCODE_MODE,
  JOYSTICK_DISABLED
}
JoystickStateTypeDef;

typedef enum
{
  RESET_1  = 						0x80,
  RESET_2  = 						0x01,
  SET_MOUSE_ACTION = 				0x07,
  SET_RELATIVE_MOUSE_POSITIONING = 	0x08,
  SET_ABSOLUTE_MOUSE_POSITIONING = 	0x09,
  SET_MOUSE_KEYCODE = 				0x0A,
  SET_MOUSE_THRESHOLD =				0x0B,
  SET_MOUSE_SCALE = 				0x0C,
  INTERROGATE_MOUSE_POSITION =		0x0D,
  LOAD_MOUSE_POSITION =				0x0E,
  SET_Y0_AT_BOTTOM = 				0x0F,
  SET_Y0_AT_TOP =					0x10,
  RESUME = 							0x11,
  DISABLE_MOUSE = 					0x12,
  PAUSE_OUTPUT = 					0x13,
  SET_JOYSTICK_EVENT_REPORTING = 	0x14,
  SET_JOYSTICK_INTERROGATION_MODE =	0x15,
  JOYSTICK_INTERROGATE = 			0x16,
  SET_JOYSTICK_MONITORING = 		0x17,
  SET_FIRE_BUTTON_MONITORING =		0x18,
  SET_JOYSTICK_KEYCODE_MODE = 		0x19,
  DISABLE_JOYSTICKS = 				0x1A,
  TIME_OF_DAY_CLOCK_SET = 			0x1B,
  INTERROGATE_TIME_OF_DAT_CLOCK = 	0x1C,
}
CommandSetTypeDef;



/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
