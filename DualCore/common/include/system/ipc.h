/**
  ******************************************************************************
  * @file    system/ipc.h
  * @author  Rainer
  * @brief   functions and data structures for inter core communication
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IPC_H
#define __IPC_H

#include "config/config.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* Type of entry-function for Core1 */
typedef void (*IPC_entryfn) ( void );

void IPC_Init_Core0     (void);
void IPC_Init_Core1     (void);
void IPC_StartCore1     ( IPC_entryfn entry );
bool IPC_SignalCore0to1 ( void );
bool IPC_SignalCore1to0 ( void );
void IPC_AckCore0to1    ( uint32_t msgID ); 
void IPC_AckCore1to0    ( uint32_t msgID ); 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __IPC_H */
