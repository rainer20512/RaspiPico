/*****************************************************************************
 * Copyright (c) 2021 Rowley Associates Limited.                             *
 *                                                                           *
 * This file may be distributed under the terms of the License Agreement     *
 * provided with this software.                                              *
 *                                                                           *
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE   *
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. *
 *                                                                           *
 *****************************************************************************/

/*****************************************************************************
 *                         Preprocessor Definitions                          *
 *                         ------------------------                          *
 * NO_STACK_INIT                                                             *
 *                                                                           *
 *   If defined, the stack pointer will not be initialised.                  *
 *                                                                           *
 * NO_VTOR_CONFIG                                                            *
 *                                                                           *
 *   If defined, the vector table offset register will not be configured.    *
 *                                                                           *
 * MEMORY_INIT                                                               *
 *                                                                           *
 *   If defined, the MemoryInit() function will be called. By default        *
 *   MemoryInit() is called after SystemInit() to enable an external memory  *
 *   controller.                                                             *
 *                                                                           *
 * STACK_INIT_VAL                                                            *
 *                                                                           *
 *   If defined, specifies the initial stack pointer value. If undefined,    *
 *   the stack pointer will be initialised to point to the end of the        *
 *   RAM segment.                                                            *
 *                                                                           *
 * VECTORS_IN_RAM                                                            *
 *                                                                           *
 *   If defined, the exception vectors will be copied from Flash to RAM.     *
 *                                                                           *
 *****************************************************************************/

  .syntax unified

  .global Reset_Handler
  .extern __vectors
  .extern LL_Blink

  .section .init, "ax"
  .thumb_func

  .equ VTOR_REG, 0xE000ED08


#ifndef STACK_INIT_VAL
#define STACK_INIT_VAL __RAM_segment_end__
#endif

Reset_Handler:

#ifndef NO_STACK_INIT
  /* Initialize main stack */
  ldr r0, =STACK_INIT_VAL
  ldr r1, =0x7
  bics r0, r1
  mov sp, r0
#endif
  
/* Should work on Core1 */
/***********
#ifdef RP2040_M0_1
  movs r0,#2
  movs r1,#250
  bl LL_Blink
#endif
***********/

/* RHB inserted: Preset all RAM with an uniform pattern */
#ifdef DO_RAM_PRESET
  // ldr  r2, RAM_PRESET_VALUE
  ldr  r2, const
  // movt r2, RAM_PRESET_VALUE >> 16

  ldr r0, =__RAM_segment_start__
  ldr r1, =__RAM_segment_end__
r00:
  cmp r0, r1
  beq e00
  str r2, [r0]
  adds r0, r0, #4
  b r00
const:
  .word RAM_PRESET_VALUE
e00:
/* RHB inserted: Scratch_X and _Y only inited by Core 0 */
#ifdef RP2040_M0_0  
  ldr r0, =__SCRATCH_X_segment_start__
  ldr r1, =__SCRATCH_X_segment_end__
r01:
  cmp r0, r1
  beq e01
  str r2, [r0]
  adds r0, r0, #4
  b r01
e01:
  ldr r0, =__SCRATCH_Y_segment_start__
  ldr r1, =__SCRATCH_Y_segment_end__
r02:
  cmp r0, r1
  beq e02
  str r2, [r0]
  adds r0, r0, #4
  b r02
#endif
e02:
#endif
/* RHB End of insertion */

#ifdef MEMORY_INIT
  ldr r0, =MemoryInit
  blx r0
#endif

#ifdef VECTORS_IN_RAM
  /* Copy exception vectors into RAM */
  ldr r0, =__vectors_start__
  ldr r1, =__vectors_end__
  ldr r2, =__vectors_ram_start__
1:
  cmp r0, r1
  beq 2f
  ldr r3, [r0]
  str r3, [r2]
  adds r0, r0, #4
  adds r2, r2, #4
  b 1b
2:
#endif

#ifndef NO_VTOR_CONFIG
  /* Configure vector table offset register */
  ldr r0, =VTOR_REG
#ifdef VECTORS_IN_RAM
  ldr r1, =_vectors_ram
#else
  ldr r1, =__vectors
#endif
  str r1, [r0]
#endif

  /* Jump to program start */
  b _start

