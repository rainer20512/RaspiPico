/**
  ******************************************************************************
  * @file    system/ipc_fsm.c
  * @author  Rainer
  * @brief   FSM for all kinds of IPC messages
  *
  ******************************************************************************
  */

#include "config/config.h"
#include "system/ipc.h"
#include "system/ipc_msg.h"
#include "system/ipc_fsm.h"
#include "task/minitask.h"
#include "debug/debug_helper.h"

IPC_FmsT *actual_fsm; /* state machine that called fsm task handler */


/*********************************************************************************
 * @brief State machine initialization
 ********************************************************************************/
void FSM_Init( IPC_FmsT *fsm, void* sendarg, ICP_FsmSendFunc sendfunc, ICP_SmFunc statemachine)
{
  fsm->fsm_status   = SM_STATE_INITIALIZED; 
  fsm->fsm_result   = SM_FINALSTATE_FAIL;     /* set to fail initially */
  fsm->current_state= 0;                      /* initial state is always 0 */
  fsm->sendarg      = sendarg;                /* runtime argument to sendfunc */
  fsm->sendfunc     = sendfunc;       
  fsm->statemachine = statemachine;   
  fsm->cb           = NULL;                   /* callback is optional */
}


void FSM_SetCB     ( IPC_FmsT *fsm, ICP_FsmFinalCBT pfcb )
{
  fsm->cb = pfcb;
}
  
/*********************************************************************************
 * @brief The function, that sends the IPC message. 
 ********************************************************************************/
void FSM_Start(IPC_FmsT *fsm) 
{
  if ( fsm->fsm_status == SM_STATE_INITIALIZED )
  {
    fsm->fsm_status = SM_STATE_RUNNING;
    FSM_Goto(fsm, fsm->current_state);
  } else {
    DEBUG_PRINTF("FSM_Start: FMS not in state initialized\n");
  }
}

/*********************************************************************************
 * @brief The function, that sends the IPC message. 
 ********************************************************************************/
void FSM_Goto      ( IPC_FmsT *fsm, uint32_t new_state )
{
    fsm->current_state = new_state;
    actual_fsm = fsm;
    TaskNotify(TASK_FSM);

}

/*********************************************************************************
 * @brief The function, that sends the IPC message. 
 ********************************************************************************/
void FSM_Terminate ( IPC_FmsT *fsm )
{
    fsm->current_state = SM_STATE_TERMINATED;
    if ( fsm->cb ) fsm->cb(fsm->fsm_result == SM_FINALSTATE_OK); 
}

/*********************************************************************************
 * @brief The function, that sends the IPC message. 
 ********************************************************************************/
void FSM_SetResult ( IPC_FmsT *fsm, IPC_FinalResultT result)
{
  fsm->fsm_result = result;
}

/*
 * Task handler for finite state machine
 */
void task_handle_fsm( uint32_t param)
{
    assert(actual_fsm);
    if (actual_fsm->fsm_status == SM_STATE_RUNNING )
    {
       actual_fsm->statemachine(actual_fsm);
    } else {
      DEBUG_PRINTF("task_handle_fsm: FMS not in state \"running\"\n");
    }
    
}
