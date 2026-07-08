
/*****************************************************************************
 * Low level Pico LED blink routine, used as a poor mans debug at early boot
 * stages
 * Does not need an initilized stack
 * but will override registers
 *****************************************************************************/

.syntax unified

/******************************************************************************
 * Blink the pico LED <nrofcycles> with <coarsems> on/off duration
 * R0 = number of cycles   
 * R1 = delay in coarse ms 
 * NOTE: Will work earliest when clock have been initialized 
 *       (by ConstructorsInit)
 *****************************************************************************/
.global LL_Blink


#define PICO_LED      25
#define PICO_LED_MASK ( 1 << PICO_LED )
#define FUNCSEL_SIO   5
#define FUNCSEL_MASK  0x1f

#include "hardware/regs/addressmap.h"
#include "hardware/regs/sio.h"
#include "hardware/regs/io_bank0.h"

.section .init, "ax"
.thumb_func
.extern pico_pin_init
.extern pico_set_pin
.extern pico_active_wait

LL_Blink:
  /* R0 = number of cycles
  /* R1 = delay in coarse ms */
  push {r0,r1,r4,r5,r6,lr}
  /* Pin Init (Function=SIO, output pin */
  movs r0, #PICO_LED
  movs r1, #1
  bl pico_pin_init

  
  /* Number of cycles to r4 */
  pop {r4} 
loop:
  cmp r4, #0
  beq done

  /* Set to 1 */
  movs r0, #PICO_LED
  movs r1, #1
  bl pico_set_pin

  /* wait ca 200 ms */
  pop {r0}
  push {r0}
  bl pico_active_wait
  
  /* Set to 0 */
  movs r0, #PICO_LED
  movs r1, #0
  bl pico_set_pin

  /* wait ca 200 ms */
  pop {r0}
  push {r0}
  bl pico_active_wait
  subs r4, r4, #1
  b loop

done:
  pop {r1,r4,r5,r6,pc}

