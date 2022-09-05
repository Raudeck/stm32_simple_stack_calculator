/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdlib.h"
#include "calculator.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
extern int _write(int file, char *ptr, int len);
volatile size_t mute = 0;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int _write(int file, char *ptr, int len)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, 10);
  return len;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  setbuf(stdout, NULL);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  /* Stack defines */
  struct keypad_string *string;
  string = (struct keypad_string *)malloc(sizeof(struct keypad_string));
  string->top = -1;
  volatile uint8_t text_input = 0;
  /* Initlise stack */
  char *str;
  char *string_clear;
  volatile size_t i, j, capacity = 1, enable_clear_input = 0, last_string_len;
  string->array = (uint8_t *)malloc(sizeof(uint8_t));

  typedef enum
  {
    PRESSED,
    RELEASE,
    DEBOUNCE,
    MUTE,
    UNMUTE,
    EXEC
  } button_status;

  button_status status = 1;
  button_status last_status = 1;

  keyscan();
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  /* Disable Buzzer */
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 29999);
  OLED_Initialise();
  OLED_Clear();
  /* Infinite loop */
  for (;;)
  {

    switch (status)
    {
    case RELEASE:
      if (!HAL_GPIO_ReadPin(MUTE_GPIO_Port, MUTE_Pin))
        status = PRESSED;
      break;
    case PRESSED:
      if (!HAL_GPIO_ReadPin(MUTE_GPIO_Port, MUTE_Pin))
      {
        status = DEBOUNCE;
      }
      else
      {
        status = RELEASE;
      }
      break;
    case DEBOUNCE:
      printf("DEBOUNCE:%d\n", HAL_GPIO_ReadPin(MUTE_GPIO_Port, MUTE_Pin));
      HAL_Delay(100);
      if (!HAL_GPIO_ReadPin(MUTE_GPIO_Port, MUTE_Pin))
      {
        status = EXEC;
      }
      else
      {
        status = RELEASE;
      }
      break;
    case MUTE:
      OLED_ShowString(3, 1, "Mute");
      mute = 1;
      last_status = status;
      status = RELEASE;
      break;
    case UNMUTE:
      mute = 0;
      OLED_ShowString(3, 1, "     ");
      last_status = status;
      status = RELEASE;
      break;
    case EXEC:
      if (last_status == MUTE)
        status = UNMUTE;
      else
        status = MUTE;
      break;
    }

    /* If there is a input */
    if ((text_input = keyscan()) != 0)
    {
      if (mute == 0)
      {
        /* Set duty cycle to 20% */
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 5999);
        HAL_Delay(100);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 29999);
      }
      if (text_input == '=')
      {
        /* Output string initializing */
        str = malloc(sizeof(uint8_t) * string->top);
        sprintf(str, "=%g", calculate(infix_to_postfix((char *)string->array)));
        /* Check if it is invalid */
        for (i = 0; str[i] != '\0'; i++)
        {
          if (str[i] == 'e')
          {
            invalid = 1;
            break;
          }
        }
        /* Clear row 1 and 2 display */
        for (i = 2; i <= 3; i++)
        {
          OLED_SEND_CMD(0xB0 + i);
          OLED_SEND_CMD(0x00);
          OLED_SEND_CMD(0x10);
          for (j = 0; j < 128; j++)
          {
            OLED_WRITE_CMD(0);
          }
        }
        /* Check if invalid or not */
        if (invalid == 1)
        {
          /* Restore */
          invalid = 0;
          str = (char *)realloc(str, sizeof(uint8_t) * 8);
          strcpy(str, "invalid");
          printf("%s\r\n", str);
          OLED_ShowString(2, 1, str);
        }
        else
        {
          printf("%s\r\n", str);
          OLED_ShowString(2, 1, str);
        }
        free(str);
        /* Restore input */
        string->array = (uint8_t *)malloc(1 * sizeof(uint8_t));
        string->array[0] = '\0';
        string->top = -1;
        capacity = 1;
        /* If there's other button is pressed, clear the monitor */
        enable_clear_input = 1;
      }
      else
      {
        if (enable_clear_input == 1)
        {
          enable_clear_input = 0;
          /* Start clearing monitor */
          string_clear = (char *)malloc(1 * sizeof(char));
          for (i = 0; i != last_string_len + 1; i++)
          {
            string_clear[i] = ' ';
            string_clear = realloc(string_clear, i * sizeof(char) + 2);
          }
          string_clear[i - 1] = '\0';
          OLED_ShowString(1, 1, string_clear);
          free(string_clear);
        }

        capacity += 1;
        string->array = realloc(string->array, capacity * sizeof(uint8_t));
        string->array[++string->top] = text_input;
        string->array[string->top + 1] = '\0';
        printf("Current value in the stack %s\r\n", string->array);
        OLED_ShowString(1, 1, (char *)string->array);
        last_string_len = string->top + 1;
      }
    }
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 11;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 29999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SCL_Pin | SDA_Pin | R1_Pin | R2_Pin | R3_Pin | R4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SCL_Pin R1_Pin R2_Pin R3_Pin
                           R4_Pin */
  GPIO_InitStruct.Pin = SCL_Pin | R1_Pin | R2_Pin | R3_Pin | R4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SDA_Pin */
  GPIO_InitStruct.Pin = SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : C4_Pin C3_Pin MUTE_Pin C2_Pin
                           C1_Pin */
  GPIO_InitStruct.Pin = C4_Pin | C3_Pin | MUTE_Pin | C2_Pin | C1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM4 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
