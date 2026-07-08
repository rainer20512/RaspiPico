/*
 ******************************************************************************
 * All neccessary steps to initialize and integrate LVGL to your project
 *
 * Author: Rainer Jan 2026
 * 
 ******************************************************************************
 */

#include "system/status.h"
#include "system/profiling.h"
#include "task/minitask.h"
#include "hardware/sync.h"

#include <stdlib.h>


#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif


void Init_DefineTasks(void);

/*
int __putchar(int c, __printf_tag_ptr u) {
  (void)(u);
  return stdio_putchar(c);
}
*/



static uint32_t cnt;

#include "../../lvgl/lv_conf.h"
#include "../../lvgl/src/display/lv_display.h"
#include "../../lvgl/src/drivers/lv_drivers.h" 
#include "../../lvgl/src/drivers/display/gc9a01/lv_gc9a01.h"
#include "system/util.h"
#include "debug/debug_helper.h"
#include "dev/GC9A01.h"
#include "pico/time.h"



#include "../../lvgl/lvgl.h"
/* style with no border an 0 radius */
lv_style_t my_style;
repeating_timer_t lvgl_update_timer;


bool task_init_lvgl1(void)
{
    uint16_t temp = (uint16_t)get_ms_since_start();
    GC9A01_hard_reset();
    lv_init();
    lv_tick_set_cb(get_ms_since_start);
    lv_display_t * disp = lv_gc9a01_create(240, 240,LV_LCD_FLAG_BGR | LV_LCD_FLAG_MIRROR_Y );
    lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_180);

    /* Set BG color to black */
    // lv_disp_set_bg_color( disp, lv_color_hex(0x000000) );
    
    lv_obj_t * scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);  

    temp = (uint16_t)get_ms_since_start() - temp;
    DEBUG_PRINTF("Init LVGL took %dms to start...\n", temp);
    // TaskNotify(TASK_LVGL1);
    return true;
}


bool Lvgl_TimerCB ( repeating_timer_t *my_timer )
{
   UNUSED(my_timer);
   TaskNotify(TASK_LVGL1);
   /* cancel repeating timer */
   return false;
}

void task_handle_lvgl1( uint32_t arg )
{
    UNUSED(arg);
    uint32_t time_till_next = lv_timer_handler();

    #if 1
        if(time_till_next == LV_NO_TIMER_READY) time_till_next = LV_DEF_REFR_PERIOD; /*handle LV_NO_TIMER_READY. Another option is to `sleep` for longer*/
        // os_delay_ms(time_till_next);
        add_repeating_timer_ms (time_till_next, Lvgl_TimerCB, NULL, &lvgl_update_timer);
        // MsTimerSetAbs ( MILLISEC_TO_TIMERUNIT(time_till_next), Lvgl_TimerCB, 0 );
    #else
       TaskNotify(TASK_LVGL1);
    #endif
}  


#if defined(RP2040_M0_0) && USE_GUI_INTERFACE
  #include "../../GUI/gui_ops.h"
  static repeating_timer_t  wait_timer; /* timer for initial wait */

  /*-----------------------------------------------------------------------------
   * Query Fontinfo from Core1 
   * The query has do be delayed bsc imm after scheduler start,there are other
   * tasks that are willing to run. Immediate query would congest these tasks
   * recommended time: at least 1 sec
   *---------------------------------------------------------------------------*/
  static bool lvgl_wait_cb(repeating_timer_t *rt) 
  {
    DEBUG_PRINTF("Querying Fontinfo...\n");
    GUI_Init_Ops_Core0();
    return false; // cancel timer in any case
  }

  void task_handle_lvgl0( uint32_t arg )
  {
    UNUSED(arg);
    add_repeating_timer_ms(1000,lvgl_wait_cb,NULL, &wait_timer );
  }   
#endif
 


