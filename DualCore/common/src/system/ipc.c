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

#include "system/rtc.h"
#include "system/ipc.h"
#include "system/ipc_msg.h"
#include "task/minitask.h"

/*-Call back when ACK was received, used by both cores */
IPC_ResultCB onAck;




#if defined(RP2040_M0_0)

    /* Mutexes for both directions of communication
     * to ensure only one msg in each direction
     * Both mutexes are claimed and init'ed by Core0
     * Core1 will get a ptr to m1to0 to us it exclusively
     */
    __attribute((section(".mutex_array"))) mutex_t m0to1,m1to0;

    /* 
     * last received messages for message on both directions
     * a message consists of flags, msgID, SendID
     */
    IPC_PacketT RECV_msg1to0;

    /* next send message IDs for message on both directions 
     * will be incremented on successful reception of this message 
     */
    static uint16_t SEND_msg0to1;
#endif

#if defined(RP2040_M0_1) || defined(CORE1_SIM)
    IPC_PacketT RECV_msg0to1;
    static uint16_t SEND_msg1to0;
#endif

#if defined(RP2040_M0_0)

    #define CORE1_VECTORTABLE   0x10100000


    /* Forward decls ---------------------------------------------------------------*/
    void core0_sio_irq_handler(void);

    /*********************************************************************************
     * @brief  Reset and start Core1
     * @param  launch: > 0: Start Core1 after reset
     *                 = 0: reset only
     ********************************************************************************/
    void IPC_Start_Core1(uint32_t launch)
    {
      uint32_t * c1vectors = (uint32_t *)CORE1_VECTORTABLE;
      /* First word is stack, second word is Reset_Vector */   
      uint32_t sp = c1vectors[0];
      IPC_entryfn startfn = (IPC_entryfn)c1vectors[1];

      /* Bring core 1 to an well defined state */
      multicore_reset_core1();

      if ( launch > 0 ) {
          printf("Core1 Start = 0x%08x, SP=0x%08x\n", startfn,sp);
          /* Setup Boot info */
          Core0_Setup_Core1_BootInfo();
          /* And start */
          multicore_launch_core1_raw (startfn, &sp, *c1vectors);
          printf("Core1 Started...\n");
      } else {
          printf("Core1 Reset...\n");
      }
    }

    /*********************************************************************************
     * @brief  Initialize FIFO interrupt on Core 0
     ********************************************************************************/
    void IPC_Init_Core0(void)
    {
        /* Init msg identifier to 1, will be incremented with every acknowledegd msg */
        SEND_msg0to1 = 1;
        mutex_init(&m0to1);
        mutex_init(&m1to0);
        irq_set_exclusive_handler(SIO_FIFO_IRQ_NUM(0), core0_sio_irq_handler);
        irq_set_enabled(SIO_FIFO_IRQ_NUM(0), true);
        #if defined(CORE1_SIM)
          SEND_msg1to0 = 1;
        #endif

    }
    /*********************************************************************************
     * @brief  bClaim = true:  Claim mutex for core 0, must be changed for Core1!
     *         bClaim = false: Release mutex
     ********************************************************************************/
    bool IPC_Core0_ClaimMutexNoWait (bool bClaim)
    {   
        bool ret=true;
        if ( bClaim ) {
          bool ret = mutex_try_enter(&m0to1, NULL);
          if ( !ret ) DEBUG_PRINTF("Core0 IPC-Mutex could not be claimed");
        } else {
          mutex_exit(&m0to1);
        }
        return ret;
    }

    /*********************************************************************************
     * @brief  handle a received fifo message on core 0
     ********************************************************************************/
    void core0_handle_fifo(void)
    {
        uint16_t ack;
        bool bAck;
        DEBUG_PRINTF("Core0 FIFO Recv: 0x%08x\n", RECV_msg1to0.raw);
        /* first check, whether it's an ACK message */
        if ( RECV_msg1to0.cooked.flags & MSG_FLAG_ACK ) {
          /* ACK msg: check ID and inc Send0ID */
          ack = RECV_msg1to0.cooked.SendID;
          DEBUG_PRINTF("Core0 FIFO Recv Ack: %d\n", ack );
          bAck = ack == SEND_msg0to1;
          if (bAck ) {
            SEND_msg0to1 += 1;
            /* If there is additional Payload, then handle it */
            if ( RECV_msg1to0.cooked.flags & MSG_FLAG_PLD ) Core0_Handle_Payload(RECV_msg1to0.cooked.MsgID);
          } else {
            DEBUG_PRINTF("Core0 FIFO Recv wrong Ack: %d vs %d\n", SEND_msg0to1, RECV_msg1to0.cooked.SendID);
          }
          /* Call callback */
          if ( onAck ) onAck( bAck );
        } else {
          /* Normal message: handle message, send acknowledge and release Mutex*/
          TaskNotify(TASK_IPC0);
        }
    }

    /*********************************************************************************
     * @brief  Core0 SIO FIFO Irq handler
     ********************************************************************************/
    void core0_sio_irq_handler(void) 
    {
        /* Receive FIFO Data */
        if (multicore_fifo_rvalid()) {
            RECV_msg1to0.raw = multicore_fifo_pop_blocking();
            core0_handle_fifo();
        }

        /* atm, we are able of handling one entry, so excess entries are just dropped */
        while (multicore_fifo_rvalid()) {
           multicore_fifo_pop_blocking();
           DEBUG_PRINTF("Core0 FIFO: entry dropped\n", RECV_msg1to0.cooked.SendID );
        }

        multicore_fifo_clear_irq();
    }

    /*********************************************************************************
     * @brief  Signal Core1, that Core0 has a message for Core1
     * @note   MUST be executed by Core 0 !
     ********************************************************************************/
    void IPC_SignalCore0to1 ( uint8_t msgID, bool bHasPayload, IPC_ResultCB ackCB )
    {
      IPC_PacketT msg={0};
  
      /* Assemble message */
      msg.cooked.MsgID  = msgID;
      msg.cooked.SendID = SEND_msg0to1;
      if ( bHasPayload )      msg.cooked.flags |= MSG_FLAG_PLD;

      /* remember ACK callback */
      onAck = ackCB;

      /* We assume that a corresponding mutex has been claimed before */
      /* so at this place we just send w/o mutex */
      #if defined(CORE1_SIM)
        /* Fake a core1-fifo message reception */
        RECV_msg0to1 = msg;
        TaskNotify(TASK_IPC1);
      #else
        multicore_fifo_push_blocking(msg.raw);
      #endif
    }

    /*********************************************************************************
     * @brief  Acknowledge reception of a message for Core 0 to Core 1 via FIFO
     * @note   MUST be executed by Core 0 !
    ********************************************************************************/
    void IPC_AckCore0to1 ( IPC_PacketT msg ) 
    {
      msg.cooked.flags |= MSG_FLAG_ACK;
      multicore_fifo_push_blocking(msg.raw);
    } 

#endif /* Core0 specific */




#if defined(RP2040_M0_1) || defined ( CORE1_SIM )

    mutex_t *pm1to0;

    
    /* Forward decls ---------------------------------------------------------------*/
    void core1_sio_irq_handler(void);

    /*********************************************************************************
     * @brief  Initialize FIFO interrupt on Core 1
     * @note   MUST be executed by Core 1 !
     * @note   When booting core1, runtime will put temporary stack to 0x20020000
     *         which is end of SCRATCH_X. Size r/a 10 words Be sure not to use that area
     ********************************************************************************/
    void IPC_Init_Core1(void)
    {
        SEND_msg1to0 = 1;
        #if !defined(CORE1_SIM)
          irq_set_exclusive_handler(SIO_FIFO_IRQ_NUM(1), core1_sio_irq_handler);
          irq_set_enabled(SIO_FIFO_IRQ_NUM(1), true);
        #endif
    }

    /*********************************************************************************
     * @brief  bClaim = true:  Claim mutex for core 0, must be changed for Core1!
     *         bClaim = false: Release mutex
     ********************************************************************************/
    bool IPC_Core1_ClaimMutexNoWait (bool bClaim)
    {   
        bool ret=true;
        if ( bClaim ) {
          bool ret = mutex_try_enter(pm1to0, NULL);
          if ( !ret ) DEBUG_PRINTF("Core0 IPC-Mutex could not be claimed");
        } else {
          mutex_exit(pm1to0);
        }
        return ret;
    }

   /*********************************************************************************
     * @brief  handle a received fifo message on core 1
     ********************************************************************************/
    void core1_handle_fifo(void)
    {
        uint16_t ack;
        bool bAck;
        DEBUG_PRINTF("Core1 FIFO Recv: 0x%08x\n", RECV_msg0to1.raw);
        /* first check, whether it's an ACK message */
        if ( RECV_msg0to1.cooked.flags & MSG_FLAG_ACK ) {
          /* ACK msg: check ID and inc Send0ID */
          ack = RECV_msg0to1.cooked.SendID;
          DEBUG_PRINTF("Core1 FIFO Recv Ack: %d\n", ack );
          bAck = ack == SEND_msg1to0 ;
          if ( bAck) {
            SEND_msg1to0 += 1;
            /* If there is additional Payload, then handle it */
            if ( RECV_msg0to1.cooked.flags & MSG_FLAG_PLD ) Core1_Handle_Payload(RECV_msg0to1.cooked.MsgID);
          } else {
            DEBUG_PRINTF("Core1 FIFO Recv wrong Ack: %d vs %d\n", SEND_msg1to0, RECV_msg0to1.cooked.SendID);
          }
          /* Call callback */
          if ( onAck ) onAck( bAck );
        } else {
          /* Normal message: handle message, send acknowledge and release Mutex*/
          TaskNotify(TASK_IPC1);
        }
    }
    /*********************************************************************************
     * @note   MUST be executed by Core 1 !
     * @brief  Core1 SIO FIFO Irq handler
     ********************************************************************************/
    void core1_sio_irq_handler(void) 
    {
        /* Receive FIFO Data */
        if (multicore_fifo_rvalid()) {
            RECV_msg0to1.raw = multicore_fifo_pop_blocking();
            core1_handle_fifo();
        }

        /* atm, we are able of handling one entry, so excess entries are just dropped */
        while (multicore_fifo_rvalid()) {
           multicore_fifo_pop_blocking();
           DEBUG_PRINTF("Core1 FIFO: entry dropped\n", RECV_msg0to1.cooked.SendID );
        }

        multicore_fifo_clear_irq();
    }

    /*********************************************************************************
     * @brief  Signal Core1, that Core0 has a message for Core1
     * @note   MUST be executed by Core 1 !
     ********************************************************************************/
    void IPC_SignalCore1to0 ( uint8_t msgID, bool bHasPayload, IPC_ResultCB ackCB )
    {
      IPC_PacketT msg={0};
  
      /* Assemble message */
      msg.cooked.MsgID  = msgID;
      msg.cooked.SendID = SEND_msg1to0;
      if ( bHasPayload ) msg.cooked.flags |= MSG_FLAG_PLD;

      /* remember ACK callback */
      onAck = ackCB;

      /* We assume that a corresponding mutex has been claimed before */
      /* so at this place we just send w/o mutex */
      multicore_fifo_push_blocking(msg.raw);
    }

    /*********************************************************************************
     * @brief  Acknowledge reception of a message for Core 1 to Core 0 via FIFO
     * @note   MUST be executed by Core 1 !
    ********************************************************************************/
    void IPC_AckCore1to0 ( IPC_PacketT msg ) 
    {
      msg.cooked.flags |= MSG_FLAG_ACK;
      #if defined(CORE1_SIM)
        RECV_msg1to0 = msg;
        core0_handle_fifo();
      #else
        multicore_fifo_push_blocking(msg.raw);
      #endif
    } 
#endif /* Core1, Core1-Sim specific */