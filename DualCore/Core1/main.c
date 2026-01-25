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
#include "system/ipc.h"
#include "system/profiling.h"
#include "task/minitask.h"
#include "hardware/sync.h"
#include "dev/uarts.h"
#include "debug/debug_helper.h"

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

#include "system/util.h"
#include "system/ipc_msg.h"
#include "dev/GC9A01.h"


void lv_init(void);
void lv_example_anim_2(void);
extern void LL_Blink(uint32_t nrofblinks, uint32_t delayms );

#if (USE_SPI1 > 0 || USE_SPI0 > 0 ) && USE_LVGL > 0
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

#if 0
int main(void) 
{
    IPC_Init_Core0();
    alarm_pool_init_default();
    #if DEBUG_UART == 0
      uart0_init();
    #elif DEBUG_UART == 1
      uart1_init();      
    #else
      #error "No debug uart assigned"
    #endif
    spi_init_all();
    GC9A01_hard_reset();

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
#endif

bool rx_chars_available(void);

int main(void) 
{
    LL_Blink(8,50);
    bool ret;
    IPC_Init_Core1();
    Core1_Read_BootInfo();
    
    #if DEBUG_UART == 0
      ret = uart0_init();
    #elif DEBUG_UART == 1
      ret =uart1_init();      
    #else
      #error "No debug uart assigned"
    #endif
    spi_init_all();

    if (!ret) LL_Blink(200,30);

    puts("Core1 up and running.");

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
    LL_Blink(1,250);

#if USE_LVGL > 0
    // Trigger the refresh-loop of LVGL
    TaskNotify(TASK_LVGL);
#endif

    while (true) {
        TaskRunAll();
        if (!TaskIsRunableTask() )  {
          ProfilerPush(JOB_SLEEP); 
          __wfi();
          ProfilerPop();
        } 
    }
}

