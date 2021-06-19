#include "config.h"

void GPIO_config() {
    GPIO_InitTypeDef gpio13c;
    gpio13c.Pin = GPIO_PIN_13;
    gpio13c.Mode = GPIO_MODE_OUTPUT_PP;
    gpio13c.Pull = GPIO_NOPULL;
    gpio13c.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &gpio13c);

    GPIO_InitTypeDef gpio9a;
    gpio9a.Pin = GPIO_PIN_9;
    gpio9a.Mode = GPIO_MODE_INPUT;
    gpio9a.Pull = GPIO_PULLDOWN;
    gpio9a.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &gpio9a);
}

void SystemClock_Config()
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

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

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
    HAL_RCC_EnableCSS();
}

void MX_I2C1_Init()
{

    I2C_HandleTypeDef i2cHandler;
    i2cHandler.Instance = I2C1;
    i2cHandler.Init.ClockSpeed = 100000;
    i2cHandler.Init.DutyCycle = I2C_DUTYCYCLE_2;
    i2cHandler.Init.OwnAddress1 = 0;
    i2cHandler.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2cHandler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    i2cHandler.Init.OwnAddress2 = 0;
    i2cHandler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    i2cHandler.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&i2cHandler) != HAL_OK)
    {
        Error_Handler();
    }
}

void MX_GPIO_Init()
{
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

}

void Error_Handler()
{
    __disable_irq();
    while (1)
    {
    }
}