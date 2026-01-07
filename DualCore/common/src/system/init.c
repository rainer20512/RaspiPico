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

#if USE_USB > 0
    void U2U_InitTask(void);
    void U2U_RunTask(uint32_t arg);
#endif

#if USE_LVGL > 0
    bool task_init_lvgl(void);
    void task_handle_lvgl(uint32_t arg);
#endif

void task_handle_out  (uint32_t);
bool task_init_io     (void);

/******************************************************************************
 * Initially put all tasks to task list. 
 * This code portion is heavily contaminated by #ifdef's. If you add 
 * addtional devices, THIS is the place to add the initialization code 
 *****************************************************************************/
void Init_DefineTasks(void)
{
#ifdef RP2040_M0_0
  TaskRegisterTask(task_init_rtc, task_handle_rtc, TASK_RTC,      JOB_TASK_RTC,      "RTC task");
#endif
  TaskRegisterTask(CMD_Init,      task_handle_com, TASK_COM,      JOB_TASK_DBGIO,    "Debug input");
  TaskRegisterTask(task_init_io,  task_handle_out, TASK_LOG,      JOB_TASK_DBGIO,    "Debug output");  
#ifdef RP2040_M0_0
  TaskRegisterTask(NULL,          task_periodic,   TASK_PERIODIC, JOB_TASK_PERIODIC, "periodic task");
#endif
#if USE_LVGL > 0
  TaskRegisterTask(task_init_lvgl, task_handle_lvgl, TASK_LVGL,    JOB_TASK_LVGL,     "LVGL task");
#endif
}