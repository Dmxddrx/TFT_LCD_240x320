/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#define T_Y__Pin GPIO_PIN_0
#define T_Y__GPIO_Port GPIOC
#define T_X__Pin GPIO_PIN_5
#define T_X__GPIO_Port GPIOC
#define LCD_BLK_Pin GPIO_PIN_1
#define LCD_BLK_GPIO_Port GPIOB
#define LCD_RST_Pin GPIO_PIN_12
#define LCD_RST_GPIO_Port GPIOB
#define FSMC_A18_LCD_RS_Pin GPIO_PIN_13
#define FSMC_A18_LCD_RS_GPIO_Port GPIOD
#define FSMC_D0_Y__Pin GPIO_PIN_14
#define FSMC_D0_Y__GPIO_Port GPIOD
#define FSMC_D1_X__Pin GPIO_PIN_15
#define FSMC_D1_X__GPIO_Port GPIOD
#define FSMC_NOE_LCD_RD_Pin GPIO_PIN_4
#define FSMC_NOE_LCD_RD_GPIO_Port GPIOD
#define FSMC_NWE_LCD_WR_Pin GPIO_PIN_5
#define FSMC_NWE_LCD_WR_GPIO_Port GPIOD
#define FSMC_NE1_LCD_CS_Pin GPIO_PIN_7
#define FSMC_NE1_LCD_CS_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
