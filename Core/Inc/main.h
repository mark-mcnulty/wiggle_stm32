/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define Button_A_Pin GPIO_PIN_12
#define Button_A_GPIO_Port GPIOB
#define Button_B_Pin GPIO_PIN_13
#define Button_B_GPIO_Port GPIOB
#define LED_OUT_Pin GPIO_PIN_15
#define LED_OUT_GPIO_Port GPIOB
#define Pot_1_Pin GPIO_PIN_7
#define Pot_1_GPIO_Port GPIOC
#define SPI_BLANK_Pin GPIO_PIN_6
#define SPI_BLANK_GPIO_Port GPIOB
#define SPI_LAT_Pin GPIO_PIN_7
#define SPI_LAT_GPIO_Port GPIOB
#define SEVEN_SEG_DIG_0_Pin GPIO_PIN_8
#define SEVEN_SEG_DIG_0_GPIO_Port GPIOB
#define SEVEN_SEG_DIG_1_Pin GPIO_PIN_9
#define SEVEN_SEG_DIG_1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
