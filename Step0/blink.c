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
    while (true) {
        pin_toggle_wait( PICO_DEFAULT_LED_PIN, LED_DELAY_MS, 1 );
//        stdio_putchar('c');
//        stdio_printf("%06d Hello, world!\n", cnt++);
        TaskRunAll();
    }
}
