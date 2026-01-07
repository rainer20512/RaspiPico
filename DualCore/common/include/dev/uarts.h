/*
 ******************************************************************************
 * @file    uart.h
 * @author  Rainer
 * @brief   low level functions for uart0 / uart1
 *
 ******************************************************************************
 */

#ifndef __UART_H__
#define __UART_H__

#include "config/config.h"
#include "pico/types.h"

#define PICO_DEFAULT_UART0_TX_PIN 0
#define PICO_DEFAULT_UART0_RX_PIN 1
#define UART0_BAUDRATE            115200

#define PICO_DEFAULT_UART1_TX_PIN 4
#define PICO_DEFAULT_UART1_RX_PIN 5
#define UART1_BAUDRATE            115200

#ifdef RP2040_M0_0
  #ifndef CORE0_UART
    #error "UART for Core0 is not set"
  #endif
#else
  #ifndef CORE0_UART
    #error "UART for Core1 is not set"
  #endif
#endif

#define CORE0_UART_INSTANCE()     __CONCAT(uart,CORE0_UART)

#if USE_UART0 > 0
  bool uart0_init                   (void);
  void uart0_set_rxchars_callback   (void (*fn)(void) ); 
  void uart0_out_chars              (const char *buf, int length);
  int  uart0_in_chars               (char *buf, int length);
  bool uart0_rxchars_available      (void);
#endif


#if USE_UART1 > 0
  bool uart1_init(void);
  void uart1_set_rxchars_callback   (void (*fn)(void) ); 
  void uart1_out_chars              (const char *buf, int length);
  int  uart1_in_chars               (char *buf, int length);
  bool uart1_rxchars_available      (void);
#endif

#endif /* __UART_H__ */
