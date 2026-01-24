/*
 ******************************************************************************
 * All neccessary steps to initialize and integrate LVGL to yout project
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

#include "../lvgl/lv_conf.h"
#include "../lvgl/src/display/lv_display.h"
#include "../lvgl/src/drivers/lv_drivers.h" 
#include "../lvgl/src/drivers/display/gc9a01/lv_gc9a01.h"
#include "system/util.h"

#include "dev/GC9A01.h"
#include "pico/time.h"


void lv_init(void);
void lv_example_anim_2(void);


#include "../lvgl/lvgl.h"
/* style with no border an 0 radius */
lv_style_t my_style;
repeating_timer_t lvgl_update_timer;

void lv_example_scale_3(void);

bool task_init_lvgl(void)
{
    GC9A01_hard_reset();
    lv_init();
    lv_tick_set_cb(get_ms_since_start);
    lv_display_t * disp = lv_gc9a01_create(240, 240,LV_LCD_FLAG_BGR | LV_LCD_FLAG_MIRROR_Y );
    lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_180);

    /* Init Style */
    lv_style_init(&my_style);
    lv_style_set_border_width(&my_style, 0);
    lv_style_set_radius(&my_style, 0);


//    lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
    //lv_example_get_started_1();
//    lv_example_anim_2();
    // lv_example_scale_3();
    // lv_xml_register_component_from_data();
    TaskNotify(TASK_LVGL);
    return true;
}


bool Lvgl_TimerCB ( repeating_timer_t *my_timer )
{
   UNUSED(my_timer);
   TaskNotify(TASK_LVGL);
   /* cancel repeating timer */
   return false;
}

void task_handle_lvgl( uint32_t arg )
{
    UNUSED(arg);
    uint32_t time_till_next = lv_timer_handler();

    #if 1
        if(time_till_next == LV_NO_TIMER_READY) time_till_next = LV_DEF_REFR_PERIOD; /*handle LV_NO_TIMER_READY. Another option is to `sleep` for longer*/
        // os_delay_ms(time_till_next);
        add_repeating_timer_ms (time_till_next, Lvgl_TimerCB, NULL, &lvgl_update_timer);
        // MsTimerSetAbs ( MILLISEC_TO_TIMERUNIT(time_till_next), Lvgl_TimerCB, 0 );
    #else
       TaskNotify(TASK_LVGL);
    #endif
}  


