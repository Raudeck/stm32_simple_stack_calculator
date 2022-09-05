/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
extern volatile size_t button_pressed_count;
extern volatile size_t invalid;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern uint8_t keyscan();
extern void OLED_SEND_CMD(uint16_t cmd);
extern void OLED_WRITE_CMD(uint16_t cmd);
extern void OLED_Initialise();
extern void OLED_Clear();
extern void OLED_SetCursor(uint8_t Y, uint8_t X);
extern void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
extern void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
extern void OLED_Modify(uint8_t Line, uint8_t Column);
extern void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SCL_Pin GPIO_PIN_0
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_1
#define SDA_GPIO_Port GPIOB
#define C4_Pin GPIO_PIN_10
#define C4_GPIO_Port GPIOB
#define C3_Pin GPIO_PIN_11
#define C3_GPIO_Port GPIOB
#define MUTE_Pin GPIO_PIN_3
#define MUTE_GPIO_Port GPIOB
#define C2_Pin GPIO_PIN_4
#define C2_GPIO_Port GPIOB
#define C1_Pin GPIO_PIN_5
#define C1_GPIO_Port GPIOB
#define R1_Pin GPIO_PIN_6
#define R1_GPIO_Port GPIOB
#define R2_Pin GPIO_PIN_7
#define R2_GPIO_Port GPIOB
#define R3_Pin GPIO_PIN_8
#define R3_GPIO_Port GPIOB
#define R4_Pin GPIO_PIN_9
#define R4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
/* Matrix keypad input */
struct keypad_string {
  uint8_t *array;
  uint8_t top;
};
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
