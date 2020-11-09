/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lib/MPL3115A2/MPL3115A2.h"


static const uint16_t ALT_ADDR = 0xC0;      // MPL3115A2 I2C Address
static const uint8_t WHO_AM_I_REG = 0x0C;   // MPL3115A2 WHO_AM_I Address


void SystemClock_Config(void);

GPIO_TypeDef* LEDPorts[4] = {LED1_GPIO_Port, LED2_GPIO_Port, LED3_GPIO_Port, LED4_GPIO_Port};
uint16_t LEDPins[4] = {LED1_Pin, LED2_Pin, LED3_Pin, LED4_Pin};
int i = 1;
int direction = 1;
void LED_loop() {
    HAL_GPIO_TogglePin(LEDPorts[i], LEDPins[i]);
    if(i == 3) {
        direction *= -1;
    } else if(i == 0) {
        direction *= -1;
    }
    i += direction;
    HAL_GPIO_TogglePin(LEDPorts[i], LEDPins[i]);
}

int main(void) {
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();

  uint8_t buf[8];
  memset(buf, 0, 8);

  while (1) {
    printf("Starting...\r\n");

    MPL3115A2 mpl;
    bool ret = mpl.begin(&hi2c1);
    if(!ret) {
        printf("Altimeter failed to begin.\r\n");
        return 0;
    }
    mpl.setSeaPressure(101325);
    float alt = -1;
    float temp = -1;

    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

    while(1) {
        LED_loop();

        alt = mpl.getAltitude()*3.28084;
        printf("Altitude: %f ft\r\n", alt);

        temp = mpl.getTemperature();
        printf("Temperature: %f degrees C\r\n", temp);

        HAL_Delay(500);
    }
    return 0;
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
