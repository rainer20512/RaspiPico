/**
  ******************************************************************************
  * @file    system/ipc_msg.h
  * @author  Rainer
  * @brief   functions, data structures and handlers for inter core communication
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IPCMSG_H_
#define __IPCMSG_H_

#define IPC_FIXEDBUFSIZE      16

#define IPC_MSG_0TO1_INIT     1         /* Initialize Core1 with essential runtime data */

bool Core0_Handle_Payload(uint8_t msgID);
void task_handle_ipc0    ( uint32_t arg );
bool Core1_Handle_Payload(uint8_t msgID);
void task_handle_ipc1    ( uint32_t arg );

bool Core0_Init_IPC_Comm ( void );     /* Setup essential data in core 1 */ 

/******************************************************************************** 
 * Data structure for IPC communication, we need one for each direction
 *******************************************************************************/
#define IPC_BUFSIZE           64
typedef struct {
  uint8_t   buff[IPC_BUFSIZE]; /* buffer for IPC data    */
  uint16_t  uSize;             /* actual number of bytes */
} IPC_BuffT;

/******************************************************************************** 
 * essential data to extablish core1 ipc comm
 * set up by core0, consumed by core 1
 * must be sent before all other things
 * core1 waits for this 
 *******************************************************************************/
typedef struct {
  void      *m1to0;
  IPC_BuffT *pbuf0to1;
  IPC_BuffT *pbuf1to0;
} IPC_EssentialT;
#endif /* __IPCMSG_H_ */