#include "main.h"

#define TRIES_UNTIL_STOP 100
#define MSEC 1
#define SEC 1000

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  GPIO_config();

  while (1)
  {
      listenForSignal();
      unsigned short i = 0;
//      while (i++ <= TRIES_UNTIL_STOP && machineStarted) {
//        getNextMmms();
//        scan();
//        if(isMmmsPresentInChamber()) {
//            i = 0;
//            directTo(currentMmmsColor());
//            openLatch();
//            waitFor(100 * MSEC);
//            closeLatch();
//        }
//      }
//      stopMachine();
  }
}

void listenForSignal() {
    int value = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
    if(value) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
