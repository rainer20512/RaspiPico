/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Pin definitions for original RasPi Pico board
#include "pico.h"
#include "boards/pico.h"  
#include <stdio.h>
#include "pico/stdlib.h"
#include "system/status.h"
#include "system/profiling.h"
#include "task/minitask.h"
#include "hardware/sync.h"

#include <stdlib.h>

// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif

#if DEBUG_MODE 
  uint32_t console_debuglevel;
  uint32_t fatfs_debuglevel;
#endif

void Init_DefineTasks(void);

/*
int __putchar(int c, __printf_tag_ptr u) {
  (void)(u);
  return stdio_putchar(c);
}
*/


#if UNIQUEID
extern uint32_t * __fast_load_start__;
extern uint32_t * __fast_start__;
extern uint32_t * __fast_end__;

bool check_fastrun ( void ) {

uint32_t *src = __fast_load_start__;
uint32_t *dest = __fast_start__;

  while ( dest < __fast_end__ ) {
    if ( *dest != *src ) {
      stdio_printf("diff at 0x%08x: src=%08x, dest=%08x\n", dest, *src, *dest);
      return false;
    } 
    dest++; src++;
  }
  stdio_puts("Fastrun segment ok");  
  return true;
}

#include "pico/unique_id.h"
static void dump_unique_id(void) {
    pico_unique_board_id_t board_id;
    pico_get_unique_board_id(&board_id);

    printf("Unique identifier:");
    for (int i = 0; i < PICO_UNIQUE_BOARD_ID_SIZE_BYTES; ++i) {
        printf(" %02x", board_id.id[i]);
    }
    printf("\n");

}
#endif

static uint32_t cnt;

bool Init_Spi1 ( void );

#include "../lvgl/lv_conf.h"
#include "../lvgl/src/display/lv_display.h"
#include "../lvgl/src/drivers/lv_drivers.h" 
#include "../lvgl/src/drivers/display/gc9a01/lv_gc9a01.h"
#include "system/util.h"

void lv_init(void);
void lv_example_anim_2(void);

bool task_init_lvgl(void)
{
    Init_Spi1();
    lv_init();
    lv_tick_set_cb(get_ms_since_start);
    lv_display_t * disp = lv_gc9a01_create(240, 240, LV_LCD_FLAG_MIRROR_Y );
    lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
    //lv_example_get_started_1();
    lv_example_anim_2();
    // lv_xml_register_component_from_data();
    TaskNotify(TASK_LVGL);
    return true;
}


void Lvgl_TimerCB ( uint32_t arg)
{
   UNUSED(arg);
   TaskNotify(TASK_LVGL);
}


void task_handle_lvgl( uint32_t arg )
{
    UNUSED(arg);
    uint32_t time_till_next = lv_timer_handler();

    #if 0
        if(time_till_next == LV_NO_TIMER_READY) time_till_next = LV_DEF_REFR_PERIOD; /*handle LV_NO_TIMER_READY. Another option is to `sleep` for longer*/
        // os_delay_ms(time_till_next);
        MsTimerSetAbs ( MILLISEC_TO_TIMERUNIT(time_till_next), Lvgl_TimerCB, 0 );
     #endif
}  


int main() {
    alarm_pool_init_default();
    stdio_init_all();

    ProfilerInitTo(JOB_TASK_INIT);

    Init_DefineTasks();
    TaskInitAll();

#if UNIQUEID
    check_fastrun ();
    dump_unique_id();
#endif
#ifndef PICO_DEFAULT_LED_PIN
  #error "No default LED pin!"
#endif

    ProfilerSwitchTo(JOB_TASK_MAIN);  

    cnt = 0;
    pin_toggle_nowait( PICO_DEFAULT_LED_PIN, LED_DELAY_MS, 15 );
    while (true) {
//        stdio_putchar('c');
//        stdio_printf("%06d Hello, world!\n", cnt++);
        TaskRunAll();
        if (!TaskIsRunableTask() )  {
          ProfilerPush(JOB_SLEEP);
          __wfi();
          ProfilerPop();
        }
    }
}
