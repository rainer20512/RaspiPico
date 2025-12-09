/**
  ******************************************************************************
  * @file    system/status.h
  * @author  Rainer
  * @brief   Enumerations for status and their printable output
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STATUS_H
#define __STATUS_H

#include "config/config.h"

#ifdef __cplusplus
 extern "C" {
#endif

void pin_toggle_wait( uint8_t pin, uint32_t delay_ms, uint32_t cycles );
void pin_toggle_nowait( uint8_t pin, uint32_t delay_ms, uint32_t cycles );


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STATUS_H */
