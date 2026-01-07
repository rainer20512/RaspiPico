/**
  ******************************************************************************
  * @file    system/ipc.c
  * @author  Rainer
  * @brief   functions and data structures for inter core communication
  *
  ******************************************************************************
  */

#include "config/config.h"
#include "pico/multicore.h"
#include "hardware/irq.h"
#include "debug/debug_helper.h"

#include "system/ipc.h"
 #ifdef DO_RAM_PRESET
 #endif

/* Timeout for mutex lock */
const uint32_t MUTEX_LOCK_TMO = 2000;

/* Stack for Core1 */

static uint32_t stack_core1[CORE1_STACKSIZE/4];

/* Mutexes for both directions of communication
 * to ensure only one msg in each direction
 */
static __attribute((section(".mutex_array"))) mutex_t m0to1,m1to0;

/* last received message IDs for message on both directions */
static uint32_t RECV_msg0to1, RECV_msg1to0;

/* next send message IDs for message on both directions 
 * will be incremented on successful reception of this message 
 */
static uint32_t SEND_msg0to1, SEND_msg1to0;
/* Msg ID is 24 bit long, bits 31-24 are ued as flag bits for msg type */
const uint32_t SEND_ID_MASK = 0x00ffffff;
const uint32_t MSG_FLAG_ACK = 0x80000000; /* Acknowlegde bit, ie msg num is ack'ed*/

/*********************************************************************************
 * @brief  Core0 SIO FIFO Irq handler
 ********************************************************************************/
void core0_sio_irq_handler(void) 
{
    uint32_t ack;
    /* Receive FIFO Data */
    while (multicore_fifo_rvalid()) {
        RECV_msg1to0 = multicore_fifo_pop_blocking();
        DEBUG_PRINTF("Core0 FIFO Recv: %u\n", RECV_msg1to0);
        /* first check, whether it's an ACK message */
        if ( RECV_msg1to0 & MSG_FLAG_ACK ) {
          /* ACK msg: check ID and inc Send0ID */
          ack = RECV_msg1to0 & ~MSG_FLAG_ACK;
          DEBUG_PRINTF("Core0 FIFO Recv Ack: %d\n", ack );
          if ( ack == SEND_msg0to1 ) {
            SEND_msg0to1 = SEND_msg0to1 + 1 & SEND_ID_MASK;
          } else {
            DEBUG_PRINTF("Core0 FIFO Recv wrong Ack: %d vs %d\n", SEND_msg0to1, RECV_msg1to0);
          }
          /* release mutex in any case */
          mutex_exit(&m0to1);
        } else {
          /* Normal message:
           * handle message an send acknowledge 
           */
          // RHB tbd
        }
    }
    multicore_fifo_clear_irq();
}

/*********************************************************************************
 * @note   MUST be executed by Core 1 !
 * @brief  Core1 SIO FIFO Irq handler
 ********************************************************************************/
void core1_sio_irq_handler(void) 
{
    uint32_t ack;
    /* Receive FIFO Data */
    while (multicore_fifo_rvalid()) {
        RECV_msg0to1 = multicore_fifo_pop_blocking();
        DEBUG_PRINTF("Core1 FIFO Recv: %u\n", RECV_msg0to1);
        /* first check, whether it's an ACK message */
        if ( RECV_msg0to1 & MSG_FLAG_ACK ) {
          /* ACK msg: check ID and inc Send0ID */
          ack = RECV_msg0to1 & ~MSG_FLAG_ACK;
          DEBUG_PRINTF("Core1 FIFO Recv Ack: %d\n", ack );
          if ( ack == SEND_msg1to0 ) {
            SEND_msg1to0 = SEND_msg1to0 + 1 & SEND_ID_MASK;
          } else {
            DEBUG_PRINTF("Core1 FIFO Recv wrong Ack: %d vs %d\n", SEND_msg1to0, RECV_msg0to1);
          }
          /* release mutex in any case */
          mutex_exit(&m0to1);
        } else {
          /* Normal message:
           * handle message and send acknowledge 
           */
          // RHB tbd: handle message
          IPC_AckCore1to0(RECV_msg0to1);
        }
    }
    multicore_fifo_clear_irq();
}

/*********************************************************************************
 * @brief  Initialize FIFO interrupt on Core 0
 ********************************************************************************/
void IPC_Init_Core0(void)
{
    SEND_msg0to1 = 1;
    mutex_init(&m0to1);
    irq_set_exclusive_handler(SIO_FIFO_IRQ_NUM(0), core0_sio_irq_handler);
    irq_set_enabled(SIO_FIFO_IRQ_NUM(0), true);
}

/*********************************************************************************
 * @brief  Initialize FIFO interrupt on Core 1
 * @note   MUST be executed by Core 1 !
 ********************************************************************************/
void IPC_Init_Core1(void)
{
    SEND_msg1to0 = 1;
   // RHB ToDo mutex_init(&m1to0);
    irq_set_exclusive_handler(SIO_FIFO_IRQ_NUM(1), core1_sio_irq_handler);
    irq_set_enabled(SIO_FIFO_IRQ_NUM(1), true);
}

void IPC_StartCore1 ( IPC_entryfn entry, uint32_t *sp, uint32_t vector_table )
{
  /* Bring core 1 to an well defined state */
  multicore_reset_core1();

  /* And start */
  // multicore_launch_core1_with_stack(entry, stack_core1, CORE1_STACKSIZE );
  multicore_launch_core1_raw (entry, sp, vector_table);
}

/*********************************************************************************
 * @brief  Signal Core1, that Core0 has a message for Core1
 * @note   MUST be executed by Core 0 !
 ********************************************************************************/
bool IPC_SignalCore0to1 ( void )
{
  /* aquire mutex to ensure no other msg to Core1 is still waiting */
  bool ret = mutex_enter_timeout_ms(&m0to1, MUTEX_LOCK_TMO);
  if ( ret ) {
    /* Mutex aquiredd, send ID */
    multicore_fifo_push_blocking(SEND_msg0to1 & SEND_ID_MASK );
  } else {
    /* Mutex NOT aquired, error */
    DEBUG_PRINTF("Core0->1 Mutex could not be aquired SendID=%d\n",SEND_msg0to1);
  }
  return ret;
}

/*********************************************************************************
 * @brief  Send a message (uint32_t) from Core 1 to Core 0 via FIFO
 * @note   MUST be executed by Core 1 !
 ********************************************************************************/
bool IPC_SignalCore1to0 (void )
{
  /* aquire mutex to ensure no other msg to Core1 is still waiting */
  bool ret = mutex_enter_timeout_ms(&m1to0, MUTEX_LOCK_TMO);
  if ( ret ) {
    /* Mutex aquiredd, send ID */
    multicore_fifo_push_blocking(SEND_msg1to0 & SEND_ID_MASK );
  } else {
    /* Mutex NOT aquired, error */
    DEBUG_PRINTF("Core1->0 Mutex could not be aquired SendID=%d\n",SEND_msg1to0);
  }
  return ret;
}
/*********************************************************************************
 * @brief  Acknowledge reception of a message for Core 0 to Core 1 via FIFO
 * @note   MUST be executed by Core 0 !
********************************************************************************/
void IPC_AckCore0to1 ( uint32_t msgID ) 
{
  multicore_fifo_push_blocking(msgID | MSG_FLAG_ACK );
} 
/*********************************************************************************
 * @brief  Acknowledge reception of a message for Core 1 to Core 0 via FIFO
 * @note   MUST be executed by Core 1 !
********************************************************************************/
void IPC_AckCore1to0 ( uint32_t msgID ) 
{
  multicore_fifo_push_blocking(msgID | MSG_FLAG_ACK );
} 
