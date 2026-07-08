/**
  ******************************************************************************
  * @file    system/ipc_fsm.h
  * @author  Rainer
  * @brief   FSM for all kinds of IPC messages
  *
  ******************************************************************************
  */
#ifndef __IPC_FMS_H_
#define __IPC_FMS_H_

#include "config/config.h"

/***** Forward declarations *****************************************************/
typedef struct IPC_FmsT IPC_FmsT;

/*********************************************************************************
 * @brief Indicator for State machine status: running or terminated
 ********************************************************************************/
typedef enum {
  SM_STATE_INITIALIZED = 1,
  SM_STATE_RUNNING     = 2,
  SM_STATE_TERMINATED  = 3,
} IPC_StatusEnum;

/*********************************************************************************
 * @brief Final result at FSM termination, to be evaluated by caller
 ********************************************************************************/
typedef enum {
  SM_FINALSTATE_FAIL = 0,
  SM_FINALSTATE_OK   = 1,
} IPC_FinalResultT;

/*********************************************************************************
 * @brief The function, that sends the IPC message. 
 ********************************************************************************/
typedef bool (*ICP_FsmSendFunc)(void *,IPC_ResultCB);

/*********************************************************************************
 * @brief The State machine
 ********************************************************************************/
typedef void (*ICP_SmFunc)(IPC_FmsT*);

/*********************************************************************************
 * @brief Callback on termination
 ********************************************************************************/
typedef void (*ICP_FsmFinalCBT)(bool);


typedef struct IPC_FmsT {
  IPC_StatusEnum fsm_status;    /* status: running/terminated  */
  IPC_FinalResultT fsm_result;  /* result when terminated: ok or fail */
  uint32_t current_state;       /* current FSM state, will be initiated to 0 */
  uint32_t wait_retries;        /* How often the wait for ACK may be retried */
  void *sendarg;                /* runtime argument to sendfunc */
  ICP_FsmSendFunc sendfunc;     /* IPC message send function */
  ICP_SmFunc statemachine;      /* state machine function */ 
  ICP_FsmFinalCBT cb;           /* optional callback on termination */
} IPC_FmsT;

void FSM_Init           ( IPC_FmsT *fsm, void* sendarg, ICP_FsmSendFunc sendfunc, ICP_SmFunc statemachine);
void FSM_SetWaitRetries ( IPC_FmsT *fsm, uint32_t retries );
void FSM_SetCB          ( IPC_FmsT *fsm, ICP_FsmFinalCBT pfcb );
void FSM_Start          ( IPC_FmsT *fsm);
void FSM_Goto           ( IPC_FmsT *fsm, uint32_t new_state );
void FSM_Terminate      ( IPC_FmsT *fsm );
void FSM_SetResult      ( IPC_FmsT *fsm, IPC_FinalResultT result);

#endif /* __IPC_FMS_H_ */