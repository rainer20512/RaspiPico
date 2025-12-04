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

#include "pico.h"
#include "pico/asm_helper.S"

#include "pico/platform/cpu_regs.h"

#include "hardware/regs/addressmap.h"
#include "hardware/regs/sio.h"
#include "pico/binary_info/defs.h"
// #include "boot/picobin.h"
#include "pico/bootrom.h"

.syntax unified

// ELF entry point:
.type _entry_point,%function
.thumb_func
.global _entry_point
_entry_point:

#if PICO_NO_FLASH
    // on the NO_FLASH case, we do not do a rest thru bootrom below, so the RCP may or may not have been initialized:
    //
    // in the normal (e.g. UF2 download etc. case) we will have passed thru bootrom initialization, but if
    // a NO_FLASH binary is loaded by the debugger, and run directly after a reset, then we won't have.
    //
    // we must therefore initialize the RCP if it hasn't already been

#if HAS_REDUNDANCY_COPROCESSOR
    // just enable the RCP which is fine if it already was (we assume no other co-processors are enabled at this point to save space)
    ldr r0, = PPB_BASE + M33_CPACR_OFFSET
    movs r1, #ARM_CPU_PREFIXED(CPACR_CP7_BITS)
    str r1, [r0]
    // only initialize canary seeds if they haven't been (as to do so twice is a fault)
    mrc p7, #1, apsr_nzcv, c0, c0, #0
    bmi 1f
    // i dont think it much matters what we initialized to, as to have gotten here we must have not
    // gone thru the bootrom (which a secure boot would have)
    mcrr p7, #8, r0, r0, c0
    mcrr p7, #8, r0, r0, c1
    sev
1:
#endif
#if !__ARM_ARCH_6M__
    // Make sure stack limit is 0 if we came in thru the debugger; we do not know what it should be
    movs r0, #0
    msr msplim, r0
#endif

    ldr r0, =__vectors
    // Vector through our own table (SP, VTOR will not have been set up at
    // this point). Same path for debugger entry and bootloader entry.
#else
    // Debugger tried to run code after loading, so SSI is in 03h-only mode.
    // Go back through bootrom + boot2 to properly initialise flash.
    ldr r0, =BOOTROM_VTABLE_OFFSET
#endif

_enter_vtable_in_r0:
    ldr r1, =(PPB_BASE + ARM_CPU_PREFIXED(VTOR_OFFSET))
    str r0, [r1]
    ldmia r0!, {r1, r2}
    msr msp, r1
    bx r2

  .global Reset_Handler
  .extern __vectors

  .section .init, "ax"
  .thumb_func

  .equ VTOR_REG, 0xE000ED08

#ifndef STACK_INIT_VAL
#define STACK_INIT_VAL __RAM_segment_end__
#endif

Reset_Handler:

    // Note if we entered thru here on core 0, then we should have gone thru bootrom, so SP (and MSPLIM) on Armv8-M
    // should already be set

    // Only core 0 should run the C runtime startup code; core 1 is normally
    // sleeping in the bootrom at this point but check to be sure (e.g. if
    // debugger put core 1 at the ELF entry point for some reason)
    ldr r0, =(SIO_BASE + SIO_CPUID_OFFSET)
    ldr r0, [r0]
#if __ARM_ARCH_6M__
    cmp r0, #0
    beq 1f
#else
    cbz r0, 1f
#endif

hold_non_core0_in_bootrom:
    // Send back to the ROM to wait for core 0 to launch it.
    ldr r0, =BOOTROM_VTABLE_OFFSET
    bl _enter_vtable_in_r0
1:

#if !PICO_RP2040 && PICO_EMBED_XIP_SETUP && !PICO_NO_FLASH
    // Execute boot2 on the core 0 stack (it also gets copied into BOOTRAM due
    // to inclusion in the data copy table below). Note the reference
    // to __boot2_entry_point here is what prevents the .boot2 section from
    // being garbage-collected.
_copy_xip_setup:
    ldr r1, =__boot2_entry_point
    mov r3, sp
    add sp, #-256
    mov r2, sp
    bl data_cpy
_call_xip_setup:
    mov r0, sp
    adds r0, #1
    blx r0
    add sp, #256
#endif

#ifndef NO_STACK_INIT
  /* Initialize main stack */
  ldr r0, =STACK_INIT_VAL
  ldr r1, =0x7
  bics r0, r1
  mov sp, r0
#endif

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


