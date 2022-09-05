#include "main.h"
#include "stdio.h"

volatile size_t button_pressed_count = 0;

uint8_t keyscan()
{
	/* Check the column 1 if there is a key being pressed */
	HAL_GPIO_WritePin(GPIOB, R1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, R2_Pin | R3_Pin | R4_Pin, GPIO_PIN_SET);
	if (!(HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin)))
	{
		HAL_Delay(75);
		if (!(HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin)))
		{
			return '1';
		}
	}
	else if (!(HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin)))
	{
		/* Check the row 1 column 2 if there is a key being pressed */
		HAL_Delay(75);
		if (!(HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin)))
		{
			return '2';
		}
	}
	else if (!(HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin)))
	{
		HAL_Delay(75);
		if (!(HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin)))
		{
			return '3';
		}
	}
	else if (!(HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin)))
	{
		HAL_Delay(75);
		if (!(HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin)))
		{
			return '+';
		}
	}
	/* Row 2 scanning */
	HAL_GPIO_WritePin(GPIOB, R2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, R1_Pin | R3_Pin | R3_Pin, GPIO_PIN_SET);
	if (!HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin))
	{
		HAL_Delay(75);
		if (!HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin))
		{
			return '4';
		}
	}
	if (!HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin))
	{
		HAL_Delay(75);
		if (!HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin))
		{
			return '5';
		}
	}
	if (!HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin))
	{
		HAL_Delay(75);
		if (!HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin))
		{
			return '6';
		}
	}
	if (!HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin))
	{
		HAL_Delay(75);
		if (!HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin))
		{
			return '-';
		}
	}

	/* Row 3 scanning */
	HAL_GPIO_WritePin(GPIOB, R3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, R1_Pin | R2_Pin | R4_Pin, GPIO_PIN_SET);
	if (!HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin))
	{
		HAL_Delay(75);
		if (!HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin))
		{
			return '7';
		}
	}
	if (!HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin))
	{
		HAL_Delay(75);
		if (!HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin))
		{
			return '8';
		}
	}
	if (!HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin))
	{
		HAL_Delay(75);
		if (!HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin))
		{
			return '9';
		}
	}

	if (!HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin))
	{
		HAL_Delay(75);
		if (!HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin))
		{
			return '*';
		}
	}
	HAL_GPIO_WritePin(GPIOB, R4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, R1_Pin | R2_Pin | R3_Pin, GPIO_PIN_SET);
	if (!HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin))
	{
		HAL_Delay(75);
		if (!HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin))
		{
			if ((button_pressed_count++) % 2 == 0)
			{
				return '(';
			}
			else
			{
				return ')';
				/* Prevent overflow */
				button_pressed_count = 0;
			}
		}
	}
	if (!HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin))
	{
		HAL_Delay(75);
		if (!HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin))
		{
			return '0';
		}
	}
	if (!HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin))
	{
		HAL_Delay(75);
		if (!HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin))
			return '=';
	}

	if (!HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin))
	{
		HAL_Delay(75);
		if (!HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin))
		{
			return '/';
		}
	}
	return 0;
}
