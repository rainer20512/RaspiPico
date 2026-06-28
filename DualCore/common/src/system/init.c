/**
 ******************************************************************************
 * @file    INIT.c
 * @author  rainer
 * @brief   Initialization and configuration tasks
 *          exclusively called by main
 ******************************************************************************
 */

#include "config/config.h"
#include "system/profiling.h"
#include "system/rtc.h"
#include "system/ipc_msg.h"
#include "debug/debug_helper.h"

#if DEBUG_MODE > 0
    #include "debug/debug_helper.h"
#endif

/* - Add additional conditional #includes here ------------------------------*/

/******************************************************************************
 * Find, dump and clear the most recent reset reason in PWR-SRx
 *****************************************************************************/
void Init_DumpAndClearResetSource(void)
{
}


/******************************************************************************
 * Init all other devices except those, that are initialized early ( IODEV and
 * debug uart ). This code portion is heavily contaminated by #ifdef's
 * if you add addtional devices, THIS is the place to add the initialization
 * code 
 *****************************************************************************/
void Init_OtherDevices(void)
{

}

#include "task/minitask.h"
#include "interpreters.h"
#include "system/periodic.h"
#include "xml_parser_main.h"

#if USE_USB > 0
    void U2U_InitTask(void);
    void U2U_RunTask(uint32_t arg);
#endif

#if USE_LVGL > 0
    bool task_init_lvgl1(void);
    void task_handle_lvgl1(uint32_t arg);
#endif
#if USE_GUI_INTERFACE &&  defined(RP2040_M0_0)
    bool task_init_lvgl0(void);
    void task_handle_lvgl0(uint32_t arg);
#endif

void task_handle_out  (uint32_t);
void task_handle_fsm  (uint32_t);
bool task_init_io     (void);

/******************************************************************************
 * Initially put all tasks to task list. 
 * This code portion is heavily contaminated by #ifdef's. If you add 
 * addtional devices, THIS is the place to add the initialization code 
 *****************************************************************************/
void Init_DefineTasks(void)
{
  TaskRegisterTask(CMD_Init,      task_handle_com,  TASK_COM,      JOB_TASK_DBGIO,    "Debug input");//
  TaskRegisterTask(task_init_io,  task_handle_out,  TASK_LOG,      JOB_TASK_DBGIO,    "Debug output");  
  TaskRegisterTask(NULL,          task_handle_fsm,  TASK_FSM,      JOB_TASK_FSM,      "State machine");  
  TaskRegisterTask(NULL,          task_periodic,    TASK_PERIODIC, JOB_TASK_PERIODIC, "periodic task");
  TaskRegisterTask(task_init_rtc, task_handle_rtc,  TASK_RTC,      JOB_TASK_RTC,      "RTC task");

#if defined(RP2040_M0_0)
  TaskRegisterTask(task_init_xml, task_handle_xml,  TASK_XML,      JOB_TASK_XML,      "XML parser task");
  TaskRegisterTask(NULL,          task_handle_ipc0, TASK_IPC0,     JOB_TASK_IPC0,     "IPC task core0");
  #if USE_GUI_INTERFACE 
    TaskRegisterTask(NULL,        task_handle_lvgl0, TASK_LVGL0,    JOB_TASK_LVGL,  "LVGL task Core0");
  #endif
#endif

#if defined(RP2040_M0_1) || defined ( CORE1_SIM )
  TaskRegisterTask(NULL,          task_handle_ipc1, TASK_IPC1,     JOB_TASK_IPC1,     "IPC task core1");
  #if USE_LVGL > 0
    TaskRegisterTask(task_init_lvgl1, task_handle_lvgl1, TASK_LVGL1,    JOB_TASK_LVGL,  "LVGL task Core1");
  #endif
#endif
}