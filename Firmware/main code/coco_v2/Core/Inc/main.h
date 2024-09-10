/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f4xx_hal.h"
#include "cppmain.h"
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

extern I2C_HandleTypeDef hi2c1;

extern SPI_HandleTypeDef hspi1;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim14;

extern UART_HandleTypeDef huart3;

#define TIM5_START HAL_TIM_Base_Start(&htim5)
#define TIM5_STOP HAL_TIM_Base_Stop(&htim5)
#define TIM6_IT_START HAL_TIM_Base_Start_IT(&htim6)
#define TIM6_IT_STOP HAL_TIM_Base_Stop_IT(&htim6)
#define TIM13_IT_START HAL_TIM_Base_Start_IT(&htim13)
#define TIM13_IT_STOP HAL_TIM_Base_Stop_IT(&htim13)
#define TIM14_IT_START HAL_TIM_Base_Start_IT(&htim14)
#define TIM14_IT_STOP HAL_TIM_Base_Stop_IT(&htim14)
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TB_Pin GPIO_PIN_13
#define TB_GPIO_Port GPIOC
#define RF_EMITTER_Pin GPIO_PIN_14
#define RF_EMITTER_GPIO_Port GPIOC
#define D_EMITTER_Pin GPIO_PIN_15
#define D_EMITTER_GPIO_Port GPIOC
#define RF_RECEIVER_Pin GPIO_PIN_0
#define RF_RECEIVER_GPIO_Port GPIOC
#define DR_RECEIVER_Pin GPIO_PIN_1
#define DR_RECEIVER_GPIO_Port GPIOC
#define R_RECEIVER_Pin GPIO_PIN_2
#define R_RECEIVER_GPIO_Port GPIOC
#define M_RECEIVER_Pin GPIO_PIN_3
#define M_RECEIVER_GPIO_Port GPIOC
#define RPWMB_Pin GPIO_PIN_0
#define RPWMB_GPIO_Port GPIOA
#define RPWMA_Pin GPIO_PIN_2
#define RPWMA_GPIO_Port GPIOA
#define DRDY_Pin GPIO_PIN_3
#define DRDY_GPIO_Port GPIOA
#define CS_Pin GPIO_PIN_4
#define CS_GPIO_Port GPIOA
#define SCK_Pin GPIO_PIN_5
#define SCK_GPIO_Port GPIOA
#define MISO_Pin GPIO_PIN_6
#define MISO_GPIO_Port GPIOA
#define MOSI_Pin GPIO_PIN_7
#define MOSI_GPIO_Port GPIOA
#define L_RECEIVER_Pin GPIO_PIN_4
#define L_RECEIVER_GPIO_Port GPIOC
#define DL_RECEIVER_Pin GPIO_PIN_5
#define DL_RECEIVER_GPIO_Port GPIOC
#define VOL_METER_Pin GPIO_PIN_0
#define VOL_METER_GPIO_Port GPIOB
#define LF_RECEIVER_Pin GPIO_PIN_1
#define LF_RECEIVER_GPIO_Port GPIOB
#define INT_Pin GPIO_PIN_10
#define INT_GPIO_Port GPIOB
#define LPWMA_Pin GPIO_PIN_11
#define LPWMA_GPIO_Port GPIOB
#define LED9_Pin GPIO_PIN_12
#define LED9_GPIO_Port GPIOB
#define LED10_Pin GPIO_PIN_13
#define LED10_GPIO_Port GPIOB
#define LED11_Pin GPIO_PIN_14
#define LED11_GPIO_Port GPIOB
#define LED8_Pin GPIO_PIN_15
#define LED8_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_6
#define BUZZER_GPIO_Port GPIOC
#define R_EMITTER_Pin GPIO_PIN_7
#define R_EMITTER_GPIO_Port GPIOC
#define L_EMITTER_Pin GPIO_PIN_8
#define L_EMITTER_GPIO_Port GPIOC
#define LF_EMITTER_Pin GPIO_PIN_9
#define LF_EMITTER_GPIO_Port GPIOC
#define ENC_LA_Pin GPIO_PIN_8
#define ENC_LA_GPIO_Port GPIOA
#define ENC_LB_Pin GPIO_PIN_9
#define ENC_LB_GPIO_Port GPIOA
#define LED7_Pin GPIO_PIN_10
#define LED7_GPIO_Port GPIOA
#define LED6_Pin GPIO_PIN_11
#define LED6_GPIO_Port GPIOA
#define LED5_Pin GPIO_PIN_12
#define LED5_GPIO_Port GPIOA
#define JTMS_Pin GPIO_PIN_13
#define JTMS_GPIO_Port GPIOA
#define JTCK_Pin GPIO_PIN_14
#define JTCK_GPIO_Port GPIOA
#define LED4_Pin GPIO_PIN_15
#define LED4_GPIO_Port GPIOA
#define BTX_Pin GPIO_PIN_10
#define BTX_GPIO_Port GPIOC
#define BRX_Pin GPIO_PIN_11
#define BRX_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_12
#define LED3_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_2
#define LED2_GPIO_Port GPIOD
#define LPWMB_Pin GPIO_PIN_3
#define LPWMB_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_4
#define LED1_GPIO_Port GPIOB
#define BOOT0_Pin GPIO_PIN_5
#define BOOT0_GPIO_Port GPIOB
#define ENC_RB_Pin GPIO_PIN_6
#define ENC_RB_GPIO_Port GPIOB
#define ENC_RA_Pin GPIO_PIN_7
#define ENC_RA_GPIO_Port GPIOB
#define SCL_Pin GPIO_PIN_8
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_9
#define SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern int mouseState;

extern bool LF, RF, LD, RD;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
