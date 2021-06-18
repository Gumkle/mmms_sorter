#ifndef STM32_CONFIG_H
#define STM32_CONFIG_H
#endif

#include "stm32f1xx_hal.h"

I2C_HandleTypeDef hi2c1;

void SystemClock_Config();
void MX_GPIO_Init();
void MX_I2C1_Init();
void GPIO_config();
void Error_Handler();

