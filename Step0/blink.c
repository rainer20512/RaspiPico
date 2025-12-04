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

#include <stdlib.h>

// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif

int __putchar(int c, __printf_tag_ptr u) {
  (void)(u);
  return stdio_putchar(c);
}

// Perform initialisation
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // For Pico W devices we need to initialise the driver etc
    return cyw43_arch_init();
#endif
}

// Turn the led on or off
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    // Just set the GPIO on or off
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // Ask the wifi "driver" to set the GPIO on or off
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

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

    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);     
    

#if UNIQUEID
    check_fastrun ();
    dump_unique_id();
#endif
    cnt = 0;
    while (true) {
        pico_set_led(true);
        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        sleep_ms(LED_DELAY_MS);
//        stdio_putchar('c');
        stdio_printf("%06d Hello, world!\n", cnt++);
    }
}
