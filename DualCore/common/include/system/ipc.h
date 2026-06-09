/**
  ******************************************************************************
  * @file    system/ipc.h
  * @author  Rainer
  * @brief   functions and data structures for inter core communication
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IPC_H_
#define __IPC_H_

#include "config/config.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* Timeout for mutex lock or wait for ack */
#define MUTEX_LOCK_TMO 1000



/* Type of entry-function for Core1 */
typedef void (*IPC_entryfn) ( void );

/* Type of ACk-Msg callback */
typedef void (*IPC_ResultCB) ( bool );


typedef union IPC_PacketType {
  uint32_t raw;
  struct {
    uint8_t   flags;
    uint8_t   MsgID;
    uint16_t  SendID;
  } cooked;
} IPC_PacketT;

#define MSG_FLAG_ACK    0x80  /* Acknowlegde bit, ie msg num is ack'ed */
#define MSG_FLAG_PLD    0x40  /* Flag for: Message has additional payload */
#define MSG_FLAG_FIXPLD 0x20  /* Flag for: Message has payload in fixed ipc data */

void IPC_Init_Core0       		(void);
void IPC_Init_Core1       		(void);
void IPC_Start_Core1      		(uint32_t launch);
void IPC_SignalCore0to1   		( uint8_t msgID, bool bHasPayload, IPC_ResultCB ackCB );
void IPC_AckCore0to1      		( IPC_PacketT msg );
bool IPC_CheckAckCore0    		(uint32_t timeout);
void IPC_SignalCore1to0   		( uint8_t msgID, bool bHasPayload, IPC_ResultCB ackCB );
void IPC_AckCore1to0      		( IPC_PacketT msg );
bool IPC_Core0_ClaimMutexNoWait (bool bClaim);
bool IPC_Core1_ClaimMutexNoWait (bool bClaim);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __IPC_H_ */
