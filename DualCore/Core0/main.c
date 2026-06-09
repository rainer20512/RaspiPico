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
#include "system/ipc.h"
#include "system/ipc_msg.h"
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


#include "system/util.h"

#include "dev/GC9A01.h"
#include "dev/uarts.h"

extern void LL_Blink(uint32_t nrofblinks, uint32_t delayms );
void lv_init(void);
void lv_example_anim_2(void);

#if ( USE_SPI1 > 0 || USE_SPI0 > 0 ) && USE_LVGL > 0
  void spi_init_all ( void )
  {
    #if USE_SPI0 > 0
      bool Init_Spi0 ( void );
      Init_Spi0();
    #endif  
    #if USE_SPI1 > 0
      bool Init_Spi1 ( void );
      Init_Spi1();
    #endif  
  }
#else
  #define spi_init_all() 
#endif

bool rx_chars_available(void);

int main() 
{
    bool ret;

    IPC_Init_Core0();
    alarm_pool_init_default();
    #if CORE0_UART == 0
      ret = uart0_init();
    #elif CORE0_UART == 1
      ret =uart1_init();      
    #else
      #error "No debug uart assigned"
    #endif
    if (!ret) LL_Blink(200,25);

    spi_init_all();
    GC9A01_hard_reset();

    ProfilerInitTo(JOB_TASK_INIT);

    Init_DefineTasks();
    TaskInitAll();

    /* Autostart Core1 */
    #if CORE1_AUTOSTART > 0 && !defined(CORE1_SIM)
       IPC_Start_Core1(1);
       /* Give Core1 some time to boot, at least 0,5s 
        * Be sure not to slow core1 boot down by additional debug blinks! */
       sleep_ms(500);
       printf("Start & Setup of Core1 %s\n",Core0_Init_IPC_Comm(NULL,NULL) ? "ok" :"failed" );
    #endif

#if UNIQUEID
    check_fastrun ();
    dump_unique_id();
#endif
#ifndef PICO_DEFAULT_LED_PIN
  #error "No default LED pin!"
#endif

    ProfilerSwitchTo(JOB_TASK_MAIN);  

#if USE_LVGL > 0
    // Trigger the refresh-loop of LVGL
    TaskNotify(TASK_LVGL);
#endif

    // pin_toggle_nowait( PICO_DEFAULT_LED_PIN, LED_DELAY_MS, 15 );
    while (true) {
        TaskRunAll();
        if (!TaskIsRunableTask() )  {
          ProfilerPush(JOB_SLEEP);
          __wfi();
          ProfilerPop();
        }
    }
}


