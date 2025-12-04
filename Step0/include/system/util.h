/*
 ******************************************************************************
 * @file    util.h
 * @author  Rainer
 * @brief   common utility functions
 *
 ******************************************************************************
 */
#ifndef _UTIL_H_
#define _UTIL_H_

#include "config/config.h"

int16_t average_n ( int16_t avg, int16_t value, uint8_t n  );
uint32_t My_Delay(uint32_t waittime );
void Error_Handler(char *file, int line);
void Error_Handler_XX(int32_t code, char *file, int line);

#ifdef  USE_FULL_ASSERT
  void assert_failed(char* file, uint32_t line);
  #define assert_param(expr) ((expr) ? (void)0U : assert_failed((char *)__FILE__, __LINE__))
#else
  #define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */

#endif /* _UTIL_H_ */
