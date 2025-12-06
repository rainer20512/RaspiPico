/*
 ******************************************************************************
 * @file    util.c
 * @author  Rainer
 * @brief   common utility functions
 *
 ******************************************************************************
 */

#include "config/config.h"
#include "debug/debug_helper.h"

#include "hardware/timer.h"

/******************************************************************************
 * gleitende Mittelung mit exponentieller Relaxation mit variablem n
 * M1 = (M * 2^n - M + Val) / 2^n
 * \param avg   - actual average
 * \param value - new value to add into average
 * \param n     - relaxation coefficient as power of 2 
 *                senseful values are 3,4,5,6
 * \returns       new average
 *****************************************************************************/
int16_t average_n ( int16_t avg, int16_t value, uint8_t n  )
{
        int32_t temp = (int32_t)avg;

        temp = (temp << n) - temp + value;
        return (int16_t)(temp >> n);
}

uint32_t My_Delay(uint32_t waittime )
{
  busy_wait_ms(waittime);
  return waittime;
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  source file and line number
  * @retval None
  */
void Error_Handler(char *file, int line)
{

  DEBUG_PRINTF("Error in %s line %d\n", file, line);
  
  while(1)
  {
    /* Error if LED2 is slowly blinking (1 sec. period) */
    // BSP_LED_Toggle(LED2); 
    DEBUG_PRINTF("Increments per second=%u\n",My_Delay(1000)); 
  }  
}

void Error_Handler_XX(int32_t code, char *file, int line)
{
    DEBUG_PRINTF("Errorcode %d-", code);
    Error_Handler( file, line );
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  DEBUG_PRINTF("Assert failed in %s line %d\n", file, line);
  /* Infinite loop */
  while (1)
  {
  }
}
#endif

