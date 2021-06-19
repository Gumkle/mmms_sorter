#ifndef STM32_CONFIG_H
#define STM32_CONFIG_H

#include "stm32f1xx_hal.h"

void SystemClock_Config();
void MX_GPIO_Init();
void MX_I2C1_Init();
void GPIO_config();
void Error_Handler();

#endif
