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
#ifndef __DEBUG_DMA_H
#define __DEBUG_DMA_H

#include "config/config.h"

#ifdef __cplusplus
 extern "C" {
#endif

#if DEBUG_FEATURES > 0
  void DBG_dump_dma_config(void);
#endif



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DEBUG_DMA_H */
