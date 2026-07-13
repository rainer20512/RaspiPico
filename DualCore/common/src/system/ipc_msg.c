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
#include "hardware/dma.h"
#include "system/ipc.h"
#include "system/ipc_msg.h"
#include "system/ipc_fsm.h"
#include "debug/debug_helper.h"

#include "../../GUI/gui_ops.h"


static repeating_timer_t  wait_timer; /* timer for wait states in state machine */
static IPC_FmsT ipcfsm;               /* IPC state machine */
void FSM_Ipc ( IPC_FmsT *fsm );

/******************************************************************************
 * Used in IPC to transfer Info about loaded images from Core1 to Core0
 *****************************************************************************/
typedef struct {
 GUI_RawImage_T *imageinfo;    /* Ptr to GUI_Font_T array          */
 uint8_t         imagenum;     /* number of elements in this array */
} IPC_Imageinfo_T;

/******************************************************************************
 * Used in IPC to transfer Info about loaded fonts from Core1 to Core0
 *****************************************************************************/
typedef struct {
 GUI_Font_T *fontinfo;      /* Ptr to GUI_Font_T array          */
 uint8_t    fontnum;        /* number of elements in this array */
} IPC_Fontinfo_T;


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
   * Echo function ( for testing )
   * Take to 0to1 buffer copy to 1to0, increment every single element by 1 
   * and send back directly with ack
   *****************************************************************************/
  bool Core0_Handle_Echo(void)
  {
    DEBUG_PRINTF("Core0 Echo:%s\n",buf1to0.buff);
    strcpy(buf0to1.buff, buf1to0.buff);
    buf0to1.uSize = buf1to0.uSize;
    char *p = buf0to1.buff;
    while ( *p ) {
      (*p++)++;
    }
    /* pbuf1to0 is payload for ack message */
    return true; 
  }
  /******************************************************************************
   * Handle the Message with ID "msgID".
   * returns true, if handling of msg generates new payload
   * if handling of msg does not generate new payload, return false
   * NOTE: This routine may be executed in interrupt context!
   * Do not ACK message or release mutex in here
   *****************************************************************************/
  bool Core0_Handle_Msg(uint8_t msgID)
  {
    bool ret = false;
    #if DEBUG_IPC > 0 
        DEBUG_PRINTF("Core0 received payload for msg #%d\n", msgID);
    #endif
    switch ( msgID ) {
      case IPC_MSG_1TO0_ECHO:
      case IPC_MSG_0TO1_ECHO:
        ret = Core0_Handle_Echo();
        break;
      case IPC_MSG_0TO1_QRY_IMAGEINFO:
        /* We got fontinfo From Core1 */
        IPC_Imageinfo_T *imgbuff = (IPC_Imageinfo_T *) buf1to0.buff;
        AllImages0    = imgbuff->imageinfo;
        AllImagesNum0 = imgbuff->imagenum;
        DEBUG_PRINTF("Core0 fontinfo: %d fonts\n",AllFontNum0);
        break;        
      case IPC_MSG_0TO1_QRY_FONTINFO:
        /* We got fontinfo From Core1 */
        IPC_Fontinfo_T *fntbuff = (IPC_Fontinfo_T *) buf1to0.buff;
        AllFonts0   = fntbuff->fontinfo;
        AllFontNum0 = fntbuff->fontnum;
        DEBUG_PRINTF("Core0 fontinfo: %d fonts\n",AllFontNum0);
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
  void task_handle_ipc0 ( uint32_t arg ) 
  {
    UNUSED(arg);
    /* Handle message and update payload flag to represent payload of ACK message*/
    if ( !Core0_Handle_Msg(RECV_msg1to0.cooked.MsgID) ) 
      RECV_msg1to0.cooked.flags |= MSG_FLAG_PLD;
    else
      RECV_msg1to0.cooked.flags &= ~MSG_FLAG_PLD;
    IPC_AckCore0to1(RECV_msg1to0);
  }

  /******************************************************************************
   * Setup boot info for Core1 :
   * - dma channel for core1 uart
   * - dma channel for core1 spi
   * Must be setup in fixed buffer before core1 is started
   * will be consumed by core 1 automatically
   *****************************************************************************/
  void Core0_Setup_Core1_BootInfo ( void )
  { 

    IPC_BootInfoT *boot1 = (IPC_BootInfoT*)ipc_fixedbuff;
    boot1->c1_uart_dma_chan = dma_claim_unused_channel(true);
    boot1->c1_spi_dma_chan  = dma_claim_unused_channel(true);
    /* No send, will be consumed by core1 on boot */
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
  static bool Core0_Init_IPC_Comm_Internal ( void* userdata, IPC_ResultCB pfAck )
  { 
    extern mutex_t m1to0;

    IPC_EssentialT *txbuff  = (IPC_EssentialT *)ipc_fixedbuff;
    txbuff->m1to0           = &m1to0;
    txbuff->pbuf0to1        =  &buf0to1;
    txbuff->pbuf1to0        =  &buf1to0;

    /* Send */
    IPC_SignalCore0to1 (IPC_MSG_0TO1_INIT, true, pfAck );
    return true;
  }

  bool Core0_Init_IPC_Comm ( void* arg, IPC_ResultCB onCompletion )
  {
    FSM_Init(&ipcfsm, arg, Core0_Init_IPC_Comm_Internal, FSM_Ipc );
    FSM_SetWaitRetries(&ipcfsm, IPC_INIT_WAITRETRIES);
    if ( onCompletion ) FSM_SetCB(&ipcfsm, onCompletion);
    FSM_Start(&ipcfsm);
  }

  /******************************************************************************
   * IPC test to be initiated by Core0
   * - Send a text to Core1, 
   * - On Receive, Core 1 will inccrement every single character
   * - And send back to Core 0
   *****************************************************************************/
  bool Core0_SendEcho_Internal ( void* userdata, IPC_ResultCB pfAck )
  { 
    strcpy(buf0to1.buff, "The quick brown fox jumped over the crazy dog");
    buf0to1.uSize = strlen(buf0to1.buff)+1;
    /* Send */
    IPC_SignalCore0to1 (IPC_MSG_0TO1_ECHO, true, pfAck );
    return true;
  }

  bool Core0_SendEcho ( void* arg, IPC_ResultCB onCompletion )
  {
    FSM_Init(&ipcfsm, arg, Core0_SendEcho_Internal, FSM_Ipc );
    if ( onCompletion ) FSM_SetCB(&ipcfsm, onCompletion);
    FSM_Start(&ipcfsm);
  }

  /******************************************************************************
   * IPC Query Imageinfo from Core1 to Core 0
   *****************************************************************************/
  static bool Core0_Qry_ImageinfoInternal ( void* userdata, IPC_ResultCB pfAck )
  { 
    /* No payload */
    buf0to1.uSize = 0;
    /* Send */
    IPC_SignalCore0to1 (IPC_MSG_0TO1_QRY_IMAGEINFO, false, pfAck );
    return true;
  }

  bool Core0_Qry_Imageinfo ( void* arg, IPC_ResultCB onCompletion )
  {
    FSM_Init(&ipcfsm, arg, Core0_Qry_ImageinfoInternal, FSM_Ipc );
    if ( onCompletion ) FSM_SetCB(&ipcfsm, onCompletion);
    FSM_Start(&ipcfsm);
  }
  /******************************************************************************
   * IPC Query fontinfo from Core1 to Core 0
   *****************************************************************************/
  static bool Core0_Qry_FontinfoInternal ( void* userdata, IPC_ResultCB pfAck )
  { 
    /* No payload */
    buf0to1.uSize = 0;
    /* Send */
    IPC_SignalCore0to1 (IPC_MSG_0TO1_QRY_FONTINFO, false, pfAck );
    return true;
  }

  bool Core0_Qry_Fontinfo ( void* arg, IPC_ResultCB onCompletion )
  {
    FSM_Init(&ipcfsm, arg, Core0_Qry_FontinfoInternal, FSM_Ipc );
    if ( onCompletion ) FSM_SetCB(&ipcfsm, onCompletion);
    FSM_Start(&ipcfsm);
  }


  /******************************************************************************
   * IPC Core0 send GUI Element Data to Core 1
   *****************************************************************************/
  static bool Core0_Send_Gui_Elem_Internal ( void* userdata, IPC_ResultCB pfAck )
  { 
    /* 
     * Payload of type "IPC_GUI_Xfer_Buff_T "is in userdata: 
     *  
     * all following bytes:  GUI_xxxX_T 
     */
    
    /* First check size */
    uint16_t size  = *((uint16_t *)userdata);

    /* Copy to sendbuf and msg Core1 */
    buf0to1.uSize = size;
    memcpy_fast(buf0to1.buff, userdata, size);
    /* Send */
    IPC_SignalCore0to1 (IPC_MSG_0TO1_GUIELEM, true, pfAck );
    return true;
  }

  bool Core0_Send_Gui_Elem ( void* arg, IPC_ResultCB onCompletion )
  {
    FSM_Init(&ipcfsm, arg, Core0_Send_Gui_Elem_Internal, FSM_Ipc );
    if ( onCompletion ) FSM_SetCB(&ipcfsm, onCompletion);
    FSM_Start(&ipcfsm);
    return true;
  }

#endif

  


#if  RP2040_M0_1 || defined(CORE1_SIM)

  #include <string.h>
  
  IPC_BuffT *pbuf0to1;             /* data from core0 to core 1 */
  IPC_BuffT *pbuf1to0;             /* data from core1 to core 0 */

  /****************************************************************************
   * fixed size and position buffer
   ****************************************************************************/
  #define IPC_FIXEDBUFPOS       0x20041000
  uint8_t *ipc_fixedbuffptr =  (uint8_t *)IPC_FIXEDBUFPOS;

  /****************************************************************************
   * my permanent storage for boot info from core0 
   ****************************************************************************/
  IPC_BootInfoT core1_bootinfo;

  extern IPC_PacketT RECV_msg0to1;
  extern mutex_t *pm1to0;
  void Core1_Receive_LVGL_obj(uint8_t *data, uint16_t buflen);

  /******************************************************************************
   * Get boot info for Core1 :
   * - fetch from fixedbuff
   * - store into core1 private storage
   *****************************************************************************/
  void Core1_Read_BootInfo ( void )
  { 
    IPC_BootInfoT *boot1 = (IPC_BootInfoT*)ipc_fixedbuffptr;
    core1_bootinfo = *boot1;
  }

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
 
    IPC_EssentialT *txbuff = (IPC_EssentialT *)IPC_FIXEDBUFPOS;
    pm1to0    = txbuff->m1to0;
    pbuf0to1  = txbuff->pbuf0to1;
    pbuf1to0  = txbuff->pbuf1to0;
        
    /* No payload for ack message */
    return false; 
  }
  /******************************************************************************
   * Echo function ( for testing )
   * Take to 0to1 buffer copy to 1to0, increment every single element by 1 
   * and send back directly with ack
   *****************************************************************************/
  bool Core1_Handle_Echo(void)
  {
    DEBUG_PRINTF("Core1 Echo:%s\n",pbuf0to1->buff);
    strcpy(pbuf1to0->buff, pbuf0to1->buff);
    pbuf1to0->uSize = pbuf0to1->uSize;
    char *p = pbuf1to0->buff;
    while ( *p ) {
      (*p++)++;
    }
    /* pbuf1to0 is payload for ack message */
    return true; 
  }

  /******************************************************************************
   * Handle the Message with ID "msgID".
   * returns true, if handling of msg generates new payload
   * if handling of msg does not generate new payload, return false
   * NOTE: This routine may be executed in interrupt context!
   * Do not ACK message or release mutex in here
   *****************************************************************************/
  bool Core1_Handle_Msg(uint8_t msgID)
  {
    bool ret = false;
    #if DEBUG_IPC > 0
        DEBUG_PRINTF("Core1 received payload for msg #%d\n", msgID);
    #endif
    switch ( msgID ) {
      case IPC_MSG_0TO1_INIT:
        ret = Core1_Handle_Init_IPC_Comm();
        break;
      case IPC_MSG_0TO1_ECHO:
      case IPC_MSG_1TO0_ECHO:
        ret = Core1_Handle_Echo();
        break;
      case IPC_MSG_0TO1_QRY_IMAGEINFO:
        /* Return imageinfo from Core1 to Core0 */
        IPC_Imageinfo_T *imgbuff = (IPC_Imageinfo_T *) pbuf1to0->buff;
        imgbuff->imageinfo = AllImages1;
        imgbuff->imagenum =  AllImagesNum1;
        pbuf1to0->uSize = sizeof(IPC_Imageinfo_T);
        /* We generated new payload, so return true */
        ret = true;
        break;
      case IPC_MSG_0TO1_QRY_FONTINFO:
        /* Return fontinfo from Core1 to Core0 */
        IPC_Fontinfo_T *fntbuff = (IPC_Fontinfo_T *) pbuf1to0->buff;
        fntbuff->fontinfo = AllFonts1;
        fntbuff->fontnum =  AllFontNum1;
        pbuf1to0->uSize = sizeof(IPC_Fontinfo_T);
        /* We generated new payload, so return true */
        ret = true;
        break;
      case IPC_MSG_0TO1_GUIELEM:
        Core1_Receive_LVGL_obj(pbuf0to1->buff, pbuf0to1->uSize);
        /* No new payload in ACK */
        ret = false;
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
    /* Handle message and update payload flag to represent payload of ACK message*/
    if ( Core1_Handle_Msg(RECV_msg0to1.cooked.MsgID) ) 
      RECV_msg0to1.cooked.flags |= MSG_FLAG_PLD;
    else
      RECV_msg0to1.cooked.flags &= ~MSG_FLAG_PLD;

    /* and send ACK */
    IPC_AckCore1to0(RECV_msg0to1);
  }

  bool Core1_SendEcho_Internal ( void* userdata, IPC_ResultCB pfAck )
  { 
    strcpy(pbuf1to0->buff, "Wer das liest ist doof");
    pbuf1to0->uSize = strlen(pbuf1to0->buff)+1;
    /* Send */
    IPC_SignalCore1to0 (IPC_MSG_1TO0_ECHO, true, pfAck );
    return true;
  }

  bool Core1_SendEcho ( void* arg, IPC_ResultCB onCompletion )
  {
    FSM_Init(&ipcfsm, arg, Core1_SendEcho_Internal, FSM_Ipc );
    if ( onCompletion ) FSM_SetCB(&ipcfsm, onCompletion);
    FSM_Start(&ipcfsm);
  }

#endif



/*=============================================================================
 = IPC comm State Machine
 ============================================================================*/

#ifdef RP2040_M0_0
  #define SMNAME  "FSM_CORE0"
  #define IPC_CLAIM_MUTEX(b) IPC_Core0_ClaimMutexNoWait(b)
#else
  #define SMNAME  "FSM_CORE1"
  #define IPC_CLAIM_MUTEX(b) IPC_Core1_ClaimMutexNoWait(b)
#endif

/******************************************************************************
 * States for IPC call Statemachine
 * State 0 is the first state by design
 *****************************************************************************/
typedef enum {
  IPC_STATE_START = 0,
  IPC_STATE_REDO_CLAIM,
  IPC_STATE_SEND,
  IPC_STATE_WAITACK,
  IPC_STATE_GOTACK,
  IPC_STATE_FINISHWMUTEX,
  IPC_STATE_FINISHNOMUTEX,
} IPC_FSM_STATES;

  /*-----------------------------------------------------------------------------
   * FSM Helper functions: Timeout Callback when waiting for mutex 
   *---------------------------------------------------------------------------*/
  bool mutex_wait_cb(repeating_timer_t *rt) 
  {
      /* we passed the fsm data in timers user_data field */
      IPC_FmsT *fsm = rt->user_data;

      /* this callback ist called when first mutex aquisition failed and waittime
         expired. So here we do a second try
       */
      if ( IPC_CLAIM_MUTEX(true) ) {
        /* if successful: send message*/
        FSM_Goto( fsm, IPC_STATE_SEND );
      } else {
        /* Set fsm result to fail and terminate */
        FSM_SetResult(fsm, SM_FINALSTATE_FAIL);
        FSM_Goto( fsm, IPC_STATE_FINISHNOMUTEX );
      }
      return false; // cancel timer
  }

  /*-----------------------------------------------------------------------------
   * FSM Helper functions:Timeout when waiting for Ack-Message 
   * this callback ist called only when waiting for acknowledge timed out !!!
   *---------------------------------------------------------------------------*/
bool ack_wait_cb(repeating_timer_t *rt) 
  {
      /* we passed the fsm data in timers user_data field */
      IPC_FmsT *fsm = rt->user_data;
      if ( fsm->wait_retries-- > 0 ) {
        /* additional retry chances available: give it a retry */
        FSM_Goto( fsm, IPC_STATE_WAITACK );
        #if DEBUG_IPC > 0
            DEBUG_PRINTF("Retrying when waiting for ACK\n");  
        #endif
      } else {
        /* we got no ACK, no more retries, so final state is "failed" */
        FSM_SetResult(fsm, SM_FINALSTATE_FAIL);
        FSM_Goto( fsm, IPC_STATE_FINISHWMUTEX );
        DEBUG_PRINTF("Timeout when waiting for ACK\n");
      }
      return false; // cancel timer in any case
  }

  /*-----------------------------------------------------------------------------
   * FSM Helper functions: Callback when ACK was received, 
   * will be executed in interrupt context
   *---------------------------------------------------------------------------*/
  void FSM_AckCB ( bool )
  {
    FSM_Goto(&ipcfsm, IPC_STATE_GOTACK);
  }

  /******************************************************************************
   * State machine for IPC message sending
   * will always be called initially with state 0
   * - addr of m0to1 Mutex
   * - addr of buf0to1 and buf1to0
   * Must be the first msg to be sent to core 1, core1 will wait for this before
   * core1 initialization is finished
   * NOTE: msg content is passed via fixed buffer
   *****************************************************************************/
  void FSM_Ipc ( IPC_FmsT *fsm )
  {
      #if DEBUG_IPC > 1
          DEBUG_PRINTF(SMNAME" in state %d\n", fsm->current_state);
      #endif
      switch ( fsm->current_state )
      {
        case IPC_STATE_START:
          /* Initial state: try to claim mutex */
          if ( IPC_CLAIM_MUTEX(true) ) {
            /* if successful: send message*/
            FSM_Goto( fsm, IPC_STATE_SEND );
          } else {
            /* wait and try again one time */
            add_repeating_timer_ms (MUTEX_LOCK_TMO, mutex_wait_cb, fsm, &wait_timer);
          }
          break;
        case IPC_STATE_SEND:
          /* Assemble and send message */
          if ( fsm->sendfunc(fsm->sendarg, FSM_AckCB) ) {
            /* if successful: wait for ACK*/
            FSM_Goto( fsm, IPC_STATE_WAITACK );
          } else {
            /* Send failed: terminate */
            FSM_SetResult(fsm, SM_FINALSTATE_FAIL);
            FSM_Goto( fsm, IPC_STATE_FINISHWMUTEX );
          }
          break;
        case IPC_STATE_WAITACK:
            /* Setup timer to wait for ACK */
            add_repeating_timer_ms (MUTEX_LOCK_TMO, ack_wait_cb, fsm, &wait_timer);
            break;
        case IPC_STATE_GOTACK:
            /* successful sent a message and got an ACK */
            cancel_repeating_timer(&wait_timer);  /* stop ACK wait Timer */
            FSM_SetResult(fsm, SM_FINALSTATE_OK);
            FSM_Goto( fsm, IPC_STATE_FINISHWMUTEX );
          break;
        case IPC_STATE_FINISHWMUTEX:
          /* */
          IPC_CLAIM_MUTEX(false);
          /* fallthru! */
        case IPC_STATE_FINISHNOMUTEX:
          /* Terminate the state machine, be sure the result has been set before */
          FSM_Terminate(fsm);
          break;
        default:
          /* unhandeled state */
          DEBUG_PRINTF("FSM_Core0: unknown state #%d\n");
          FSM_Terminate(fsm);
      } // switch
  }

