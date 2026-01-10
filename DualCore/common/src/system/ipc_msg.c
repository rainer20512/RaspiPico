/**
  ******************************************************************************
  * @file    system/ipc_msg.c
  * @author  Rainer
  * @brief   functions, data structures and handlers for inter core communication
  *
  ******************************************************************************
  */

#include "config/config.h"
#include "pico/multicore.h"

#include "system/ipc.h"
#include "system/ipc_msg.h"
#include "debug/debug_helper.h"

#if  RP2040_M0_0
  #include <string.h>
  /* 
   * both buffers are declared in Core0 RAM, core 1 has to query its
   * buffer address
   */
  IPC_BuffT buf0to1;             /* data from core0 to core 1 */
  IPC_BuffT buf1to0;             /* data from core1 to core 0 */

  /* 
   * additionally, there is a buffer of fixed size at a predefined location
   * core1 also knows this location and may access this buffer
   */
  #define IPCMEM                __attribute((section(".ipc_fixed")))
  static IPCMEM uint8_t ipc_fixedbuff[IPC_FIXEDBUFSIZE];

  extern IPC_PacketT RECV_msg1to0;

  /******************************************************************************
   * Handle the Message with ID "msgID".
   * returns true, if handling of msg generates new payload
   * if handling of msg does not generate new payload, return false
   * NOTE: This routine may be executed in interrupt context!
   * Do not ACK message or release mutex in here
   *****************************************************************************/
  bool Core0_Handle_Payload(uint8_t msgID)
  {
    DEBUG_PRINTF("Core0 received msg #%d\n", msgID);
    return false;
  }
  /******************************************************************************
   * Handle a previously received msg. All message parameters are stored in
   * "RECV_msg1to0"
   * Handler MUST send an ACK and release the mutex
   *****************************************************************************/
  void task_handle_ipc0 ( uint32_t arg ) 
  {
    UNUSED(arg);
    /* Check, whether there is a payload at all */
    if ( RECV_msg1to0.cooked.flags & MSG_FLAG_PLD ) {
        /* reset payload flag , if handler does not generate payload */
        if ( Core0_Handle_Payload(RECV_msg1to0.cooked.MsgID) ) RECV_msg1to0.cooked.flags &= ~MSG_FLAG_PLD;
    }
    IPC_AckCore0to1(RECV_msg1to0);
  }

  /******************************************************************************
   * Setup essentials for Core1 :
   * - addr of runtime clock array
   * - addr of m0to1 Mutex
   * - addr of buf0to1 and buf1to0
   * Must be the first msg to be sent to core 1, core1 will wait for this before
   * core1 initialization is finished
   * NOTE: msg content is passed via fixed buffer
   *****************************************************************************/
  bool Core0_Init_IPC_Comm ( void )
  { 
    extern mutex_t m1to0;
    void clock_get_clock_array(uint32_t **addr, uint32_t *size);

    IPC_EssentialT *txbuff = (IPC_EssentialT *)ipc_fixedbuff;
    txbuff->m1to0           = &m1to0;
    txbuff->pbuf0to1        =  &buf0to1;
    txbuff->pbuf1to0        =  &buf1to0;
//    clock_get_clock_array(&(txbuff->runtime_clocks), &(txbuff->num_array_bytes));

    /* Send */
    return IPC_SignalCore0to1 (IPC_MSG_0TO1_INIT, true );
  }

#endif


#if  RP2040_M0_1 || defined(CORE1_SIM)

  #include <string.h>
  
  IPC_BuffT *pbuf0to1;             /* data from core0 to core 1 */
  IPC_BuffT *pbuf1to0;             /* data from core1 to core 0 */

  /* 
   * fixed size and position buffer
   */

  #define IPC_FIXEDBUFPOS       0x20041000
  uint8_t *ipc_fixedbuffptr =  (uint8_t *)IPC_FIXEDBUFPOS;

  extern IPC_PacketT RECV_msg0to1;
  extern mutex_t *pm1to0;

  /******************************************************************************
   * Got setup essentials for Core1 :
   * - addr of runtime clock array
   * - addr of m0to1 Mutex
   * - addr of buf0to1 and buf1to0
   * Must be the first msg to be sent to core 1, core1 will wait for this before
   * core1 initialization is finished
   * NOTE: msg content is passed via fixed buffer
   *****************************************************************************/
  bool Core1_Handle_Init_IPC_Comm(void)
  {
    void clock_get_clock_array(uint32_t **addr, uint32_t *size);
    uint32_t size;
    uint32_t *myClocks;

    IPC_EssentialT *txbuff = (IPC_EssentialT *)IPC_FIXEDBUFPOS;
    pm1to0    = txbuff->m1to0;
    pbuf0to1  = txbuff->pbuf0to1;
    pbuf1to0  = txbuff->pbuf1to0;
        
    /* No payload for ack message */
    return false; 
  }

  /******************************************************************************
   * Handle the Message with ID "msgID".
   * returns true, if handling of msg generates new payload
   * if handling of msg does not generate new payload, return false
   * NOTE: This routine may be executed in interrupt context!
   * Do not ACK message or release mutex in here
   *****************************************************************************/
  bool Core1_Handle_Payload(uint8_t msgID)
  {
    bool ret = false;
    DEBUG_PRINTF("Core1 received msg #%d\n", msgID);

    switch ( msgID ) {
      case IPC_MSG_0TO1_INIT:
        ret = Core1_Handle_Init_IPC_Comm();
        break;
      default:
        DEBUG_PRINTF("No handler for IPC msg #%d\n", msgID);
    } /* switch */
    return ret;
  }
  /******************************************************************************
   * Handle a previously received msg. All message parameters are stored in
   * "RECV_msg1to0"
   * Handler MUST send an ACK and release the mutex
   *****************************************************************************/
  void task_handle_ipc1 ( uint32_t arg ) 
  {
    UNUSED(arg);
    /* Check, whether there is a payload at all */
    if ( RECV_msg0to1.cooked.flags & MSG_FLAG_PLD ) {
        /* reset payload flag , if handler does not generate payload */
        if ( Core1_Handle_Payload(RECV_msg0to1.cooked.MsgID) ) RECV_msg0to1.cooked.flags &= ~MSG_FLAG_PLD;
    }
    IPC_AckCore1to0(RECV_msg0to1);
  }

#endif