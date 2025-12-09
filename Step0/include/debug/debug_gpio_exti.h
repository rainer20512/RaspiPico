/**
  ******************************************************************************
  * @file    debug_gpio_exti.h
  * @author  Rainer
  * @brief   functions for debugging/dumping GPIO and EXTI Settings
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEBUG_GPIO_EXTI_H
#define __DEBUG_GPIO_EXTI_H

#include "config/config.h"

#ifdef __cplusplus
 extern "C" {
#endif

#if DEBUG_FEATURES > 0
  void DBG_dump_gpio_status(uint8_t banksel );
  void DBG_dump_exti_config(uint32_t exti_domain);
  void DBG_dump_nvic_config(void);
#endif



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DEBUG_GPIO_EXTI_H */
