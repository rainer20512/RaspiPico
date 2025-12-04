/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico.h"
#include "pico/asm_helper.S"

#include "pico/platform/cpu_regs.h"

#include "hardware/regs/addressmap.h"
#include "hardware/regs/sio.h"
#include "pico/bootrom.h"

// PICO_CONFIG: PICO_CRT0_NEAR_CALLS, Whether calls from crt0 into the binary are near (<16M away) - ignored for PICO_COPY_TO_RAM, default=0, type=bool, group=pico_crt0
#ifndef PICO_CRT0_NEAR_CALLS
#define PICO_CRT0_NEAR_CALLS 0
#endif

#ifdef NDEBUG
#ifndef COLLAPSE_IRQS
#define COLLAPSE_IRQS
#endif
#endif

pico_default_asm_setup

// PICO_CONFIG: PICO_CRT0_NO_RESET_SECTION, Omit .reset section contents containing the startup code. This must be set if you want to replace the startup code while still keeping the rest of pico_crt0 as the reset section define here is not garbage collected, type=bool, default=0, advanced=true, group=pico_crt0
#ifndef PICO_CRT0_NO_RESET_SECTION
#define PICO_CRT0_NO_RESET_SECTION 0
#endif

#ifdef PICO_NO_STORED_VECTOR_TABLE
#warning PICO_NO_STORED_VECTOR_TABLE is no longer used. PICO_MINIMAL_STORED_VECTOR_TABLE is not identical but usually serves the same purpose
#endif

.section .vectors, "ax"
.align 2


.global __vectors, __VECTOR_TABLE, __vectors_end
__vectors:
/* RHB chnged: Adopted to Rowley Crossworks -------------
.word __StackTop
.word _reset_handler
-------------------------------------------------------*/
.word __stack_end__
.word Reset_Handler
#if PICO_MINIMAL_STORED_VECTOR_TABLE
.word isr_invalid   // NMI
.word isr_invalid   // HardFault
#else
.word isr_nmi
.word isr_hardfault
#if PICO_RP2040
.word isr_invalid // Reserved, should never fire
.word isr_invalid // Reserved, should never fire
.word isr_invalid // Reserved, should never fire
.word isr_invalid // Reserved, should never fire
#else
.word isr_memmanage
.word isr_busfault
.word isr_usagefault
.word isr_securefault
#endif
.word isr_invalid // Reserved, should never fire
.word isr_invalid // Reserved, should never fire
.word isr_invalid // Reserved, should never fire
.word isr_svcall
#if PICO_RP2040
.word isr_invalid // Reserved, should never fire
#else
.word isr_debugmonitor
#endif
.word isr_invalid // Reserved, should never fire
.word isr_pendsv
.word isr_systick

.macro if_irq_word num func
.if \num < PICO_NUM_VTABLE_IRQS
.word \func
.endif
.endm

// we include a lot of these to allow for different number of IRQs.
// if_irq_word will only include IRQs that are valid, but we can't
// use a macro loop because isr_irqx MUST appear in the source
// as CMSIS rename exceptions #defines it to another value
if_irq_word 0 isr_irq0
if_irq_word 1 isr_irq1
if_irq_word 2 isr_irq2
if_irq_word 3 isr_irq3
if_irq_word 4 isr_irq4
if_irq_word 5 isr_irq5
if_irq_word 6 isr_irq6
if_irq_word 7 isr_irq7
if_irq_word 8 isr_irq8
if_irq_word 9 isr_irq9
if_irq_word 10 isr_irq10
if_irq_word 11 isr_irq11
if_irq_word 12 isr_irq12
if_irq_word 13 isr_irq13
if_irq_word 14 isr_irq14
if_irq_word 15 isr_irq15
if_irq_word 16 isr_irq16
if_irq_word 17 isr_irq17
if_irq_word 18 isr_irq18
if_irq_word 19 isr_irq19
if_irq_word 20 isr_irq20
if_irq_word 21 isr_irq21
if_irq_word 22 isr_irq22
if_irq_word 23 isr_irq23
if_irq_word 24 isr_irq24
if_irq_word 25 isr_irq25
if_irq_word 26 isr_irq26
if_irq_word 27 isr_irq27
if_irq_word 28 isr_irq28
if_irq_word 29 isr_irq29
if_irq_word 30 isr_irq30
if_irq_word 31 isr_irq31
if_irq_word 32 isr_irq32
if_irq_word 33 isr_irq33
if_irq_word 34 isr_irq34
if_irq_word 35 isr_irq35
if_irq_word 36 isr_irq36
if_irq_word 37 isr_irq37
if_irq_word 38 isr_irq38
if_irq_word 39 isr_irq39
if_irq_word 40 isr_irq40
if_irq_word 41 isr_irq41
if_irq_word 42 isr_irq42
if_irq_word 43 isr_irq43
if_irq_word 44 isr_irq44
if_irq_word 45 isr_irq45
if_irq_word 46 isr_irq46
if_irq_word 47 isr_irq47
if_irq_word 48 isr_irq48
if_irq_word 49 isr_irq49
if_irq_word 50 isr_irq50
if_irq_word 51 isr_irq51
if_irq_word 52 isr_irq52
if_irq_word 53 isr_irq53
if_irq_word 54 isr_irq54
if_irq_word 55 isr_irq55
if_irq_word 56 isr_irq56
if_irq_word 57 isr_irq57
if_irq_word 58 isr_irq58
if_irq_word 59 isr_irq59
if_irq_word 60 isr_irq60
if_irq_word 61 isr_irq61
if_irq_word 62 isr_irq62
if_irq_word 63 isr_irq63
if_irq_word 64 isr_irq64
if_irq_word 65 isr_irq65
if_irq_word 66 isr_irq66
if_irq_word 67 isr_irq67
if_irq_word 68 isr_irq68
if_irq_word 69 isr_irq69
if_irq_word 70 isr_irq70
if_irq_word 71 isr_irq71
if_irq_word 72 isr_irq72
if_irq_word 73 isr_irq73
if_irq_word 74 isr_irq74
if_irq_word 75 isr_irq75
if_irq_word 76 isr_irq76
if_irq_word 77 isr_irq77
if_irq_word 78 isr_irq78
if_irq_word 79 isr_irq79
#if PICO_NUM_VTABLE_IRQS > 80
#error more IRQ entries required
#endif
#endif  // #if !PICO_MINIMAL_STORED_VECTOR_TABLE
__vectors_end:

// all default exception handlers do nothing, and we can check for them being set to our
// default values by seeing if they point to somewhere between __defaults_isrs_start and __default_isrs_end
.global __default_isrs_start
__default_isrs_start:

// Declare a weak symbol for each ISR.
// By default, they will fall through to the undefined IRQ handler below (breakpoint),
// but can be overridden by C functions with correct name.

.macro decl_isr_bkpt name
.weak \name
.type \name,%function
.thumb_func
\name:
    bkpt #0
.endm

.altmacro
.macro decl_isr name
#if !PICO_MINIMAL_STORED_VECTOR_TABLE | PICO_NO_FLASH
// We declare a weak label, so user can override
.weak \name
#else
// We declare a strong global label, so user can't override (their version would not automatically be used)
.global \name
#endif
.type \name,%function
.thumb_func
\name:
.endm

.macro if_irq_decl num func
.if \num < PICO_NUM_VTABLE_IRQS
decl_isr \func
.elseif \num < NUM_IRQS
// We declare a strong global label, so user can't override (their version would not automatically be used)
.global \func
.type \func,%function
.thumb_func
\func:
.endif
.endm

if_irq_decl 0 isr_irq0
if_irq_decl 1 isr_irq1
if_irq_decl 2 isr_irq2
if_irq_decl 3 isr_irq3
if_irq_decl 4 isr_irq4
if_irq_decl 5 isr_irq5
if_irq_decl 6 isr_irq6
if_irq_decl 7 isr_irq7
if_irq_decl 8 isr_irq8
if_irq_decl 9 isr_irq9
if_irq_decl 10 isr_irq10
if_irq_decl 11 isr_irq11
if_irq_decl 12 isr_irq12
if_irq_decl 13 isr_irq13
if_irq_decl 14 isr_irq14
if_irq_decl 15 isr_irq15
if_irq_decl 16 isr_irq16
if_irq_decl 17 isr_irq17
if_irq_decl 18 isr_irq18
if_irq_decl 19 isr_irq19
if_irq_decl 20 isr_irq20
if_irq_decl 21 isr_irq21
if_irq_decl 22 isr_irq22
if_irq_decl 23 isr_irq23
if_irq_decl 24 isr_irq24
if_irq_decl 25 isr_irq25
if_irq_decl 26 isr_irq26
if_irq_decl 27 isr_irq27
if_irq_decl 28 isr_irq28
if_irq_decl 29 isr_irq29
if_irq_decl 30 isr_irq30
if_irq_decl 31 isr_irq31
if_irq_decl 32 isr_irq32
if_irq_decl 33 isr_irq33
if_irq_decl 34 isr_irq34
if_irq_decl 35 isr_irq35
if_irq_decl 36 isr_irq36
if_irq_decl 37 isr_irq37
if_irq_decl 38 isr_irq38
if_irq_decl 39 isr_irq39
if_irq_decl 40 isr_irq40
if_irq_decl 41 isr_irq41
if_irq_decl 42 isr_irq42
if_irq_decl 43 isr_irq43
if_irq_decl 44 isr_irq44
if_irq_decl 45 isr_irq45
if_irq_decl 46 isr_irq46
if_irq_decl 47 isr_irq47
if_irq_decl 48 isr_irq48
if_irq_decl 49 isr_irq49
if_irq_decl 50 isr_irq50
if_irq_decl 51 isr_irq51
if_irq_decl 52 isr_irq52
if_irq_decl 53 isr_irq53
if_irq_decl 54 isr_irq54
if_irq_decl 55 isr_irq55
if_irq_decl 56 isr_irq56
if_irq_decl 57 isr_irq57
if_irq_decl 58 isr_irq58
if_irq_decl 59 isr_irq59
if_irq_decl 60 isr_irq60
if_irq_decl 61 isr_irq61
if_irq_decl 62 isr_irq62
if_irq_decl 63 isr_irq63
if_irq_decl 64 isr_irq64
if_irq_decl 65 isr_irq65
if_irq_decl 66 isr_irq66
if_irq_decl 67 isr_irq67
if_irq_decl 68 isr_irq68
if_irq_decl 69 isr_irq69
if_irq_decl 70 isr_irq70
if_irq_decl 71 isr_irq71
if_irq_decl 72 isr_irq72
if_irq_decl 73 isr_irq73
if_irq_decl 74 isr_irq74
if_irq_decl 75 isr_irq75
if_irq_decl 76 isr_irq76
if_irq_decl 77 isr_irq77
if_irq_decl 78 isr_irq78
if_irq_decl 79 isr_irq79
#if PICO_NUM_VTABLE_IRQS > 80
#error more IRQ entries required
#endif
#if !PICO_RP2040
// since these are disabled by default, map them all to __unhandled_user_irq (will have
// a negative number
decl_isr isr_memmanage
decl_isr isr_busfault
decl_isr isr_usagefault
decl_isr isr_securefault
decl_isr isr_debugmonitor
#endif

// All unhandled USER IRQs fall through to here.
// Additionally, if the Armv9-M MemManage/BusFault/UsageFault/SecureFault/DebugMonitor exceptions
// are enabled, but the handlers are not defined, then unhandled_user_irq_num_in_r0 will
// also be reached, but with a negative exception number (e.g. MemManage == -12)
.global __unhandled_user_irq
.thumb_func
__unhandled_user_irq:
// if we include the implementation if there could be a valid IRQ hanler in the vtable that uses it
#if !(PICO_NO_RAM_VECTOR_TABLE && PICO_MINIMAL_STORED_VECTOR_TABLE)
    mrs  r0, ipsr
    subs r0, #16
.global unhandled_user_irq_num_in_r0
unhandled_user_irq_num_in_r0:
#endif
    // note the next instruction is a breakpoint too, however we have a 2 byte alignment hole
    // and it is preferrable to have distinct labels, to inform the user what has happened in the debugger.
    bkpt #0

decl_isr_bkpt isr_invalid
#if !PICO_MINIMAL_STORED_VECTOR_TABLE
// these are separated out into individual BKPT instructions with label for clarity
decl_isr_bkpt isr_nmi
decl_isr_bkpt isr_hardfault
decl_isr_bkpt isr_svcall
decl_isr_bkpt isr_pendsv
decl_isr_bkpt isr_systick
#endif

.global __default_isrs_end
__default_isrs_end:
