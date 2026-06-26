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

#include "system/ipc.h"

#define IPC_FIXEDBUFSIZE      16

#define IPC_INIT_WAITRETRIES  5         /* Number of Retries when waiting for initial ACK */

#define IPC_MSG_0TO1_INIT         1         /* Initialize Core1 with essential runtime data */
#define IPC_MSG_0TO1_ECHO         2         /* Core1 Simply echoes a msg from core0 (for test purposes)     */
#define IPC_MSG_1TO0_ECHO         3         /* Core0 Simply echoes a msg from core1 (for test purposes)     */
#define IPC_MSG_0TO1_QRY_FONTINFO 4         /* Core0 queries fontinfo an number of loaded fonts from Core1  */
#define IPC_MSG_0TO1_GUIELEM      5         /* Core0 sends GUI Element data to Core 1                       */

bool Core0_Handle_Msg           (uint8_t msgID);
void task_handle_ipc0           ( uint32_t arg );
bool Core1_Handle_Msg           (uint8_t msgID);
void task_handle_ipc1           ( uint32_t arg );

void Core0_Setup_Core1_BootInfo ( void );   /* Seup core1 boot info */
bool Core0_Init_IPC_Comm        ( void*, IPC_ResultCB );   /* Setup essential IPC data in core 1 */ 
bool Core0_SendEcho             ( void* arg, IPC_ResultCB onCompletion );  /* for testing */
bool Core0_Qry_Fontinfo         ( void* arg, IPC_ResultCB onCompletion );
bool Core0_Send_Gui_Elem        ( void* arg, IPC_ResultCB onCompletion );  /* Transfer one GUI element */

bool Core1_SendEcho             ( void* arg, IPC_ResultCB onCompletion );  /* for testing */
void Core1_Read_BootInfo        ( void );   /* read/store bootinfo from core */

/******************************************************************************** 
 * Data structure for IPC communication, we need one for each direction
 *******************************************************************************/
#define IPC_BUFSIZE          128
typedef struct {
  uint8_t   buff[IPC_BUFSIZE]; /* buffer for IPC data    */
  uint16_t  uSize;             /* actual number of bytes */
} IPC_BuffT;

/******************************************************************************** 
 * data for core1 when booting. must be put zo fixed buffer _before_ Core1
 * is booted by Core0
 * must be sent before all other things
 * core1 waits for this 
 *******************************************************************************/
typedef struct {
  uint32_t  c1_uart_dma_chan;
  uint32_t  c1_spi_dma_chan;
} IPC_BootInfoT;

#if RP2040_M0_1
  /* my permanent storage for boot info from core0 */
  extern IPC_BootInfoT core1_bootinfo;
#endif
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