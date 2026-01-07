#include "config/config.h"
#include "system/status.h"
#include "debug/debug_helper.h"

#include "hardware/gpio.h"
#include "pico/time.h"

/* Flag for "one pin is currently claimed by toggle etc" */
static uint8_t            bPinClaimed=0;
static uint32_t           blinkcount;
static uint8_t            curr_state;
static uint8_t            curr_pin;
static repeating_timer_t  toggle_timer;

// Perform initialisation
void pico_pin_init(uint8_t pin, uint8_t dir) 
{
    gpio_init(pin);
    gpio_set_dir(pin, dir);
}

// Turn the led on or off
void pico_set_pin(uint8_t pin, bool pin_on) {
    gpio_put(pin, pin_on);
}

uint8_t pin_save_cfg( uint8_t pin ) {
  if ( bPinClaimed ) return false;
  //RHB todo save config
  bPinClaimed = 1;
}
uint8_t pin_restore_cfg( uint8_t pin ) {
  if ( !bPinClaimed ) return false;
  //RHB todo restore config
  bPinClaimed = 0;
}

/* active wait r/a 100 ms per tick */
void pico_active_wait ( uint32_t ticks )
{
  uint32_t cnt;
  while ( ticks-- ) {
    for ( cnt = 0; cnt < 5000; cnt++) {
      asm("mov r0,r0");
    }
  }
}

void pin_toggle_wait( uint8_t pin, uint32_t delay_ms, uint32_t cycles )
{
  if ( !cycles || !delay_ms ) return;

  if (!pin_save_cfg( pin ) ) {
    DEBUG_PRINTF("Another pin currently claimed");
  }
  pico_pin_init(pin, GPIO_OUT);
  while (cycles--) {
    pico_set_pin(pin, true);
    sleep_ms(delay_ms);
    pico_set_pin(pin, false);
    sleep_ms(delay_ms);
  }

  pin_restore_cfg(pin);
}


bool blink_callback(repeating_timer_t *rt) 
{
  uint8_t new_state = curr_state ? 0 : 1;
  if ( --blinkcount == 0 ) {
    // finished toggling
    cancel_repeating_timer(rt);
    pin_restore_cfg(curr_pin);
    return false;
  } else {
    pico_set_pin(curr_pin, new_state);
    curr_state = new_state;
    return true; // keep repeating
  }
}

void pin_toggle_nowait( uint8_t pin, uint32_t delay_ms, uint32_t cycles )
{
  if ( !cycles || !delay_ms ) return;

  if (!pin_save_cfg( pin ) ) {
    DEBUG_PRINTF("Another pin currently claimed");
  }
  pico_pin_init(pin, GPIO_OUT);
  blinkcount = cycles*2+1; // each toggle counts, so double the cycles
  curr_pin = pin;
  curr_state = (uint8_t)gpio_get(pin);
  if (!add_repeating_timer_ms(delay_ms, blink_callback, NULL, &toggle_timer)) {
      DEBUG_PRINTF("Failed to add blink timer\n");
      pin_restore_cfg(pin);
      return;
  }

}


