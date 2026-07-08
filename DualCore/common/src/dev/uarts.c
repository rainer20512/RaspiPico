/*
 ******************************************************************************
 * @file    uart.c
 * @author  Rainer
 * @brief   low level functions for uart1 / uart1
 *
 ******************************************************************************
 */

#include "config/config.h"
#include "dev/uarts.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/irq.h"



/******************************************************************************
 * Initialize an hardware uart with a specific baudrate and 8N1
 * No IRQs, no DMA
 *****************************************************************************/
static bool uart_init_full(struct uart_inst *uart, uint baud_rate, int tx_pin, int rx_pin) {
    if (tx_pin >= 0) gpio_set_function((uint)tx_pin, UART_FUNCSEL_NUM(uart, tx_pin));
    if (rx_pin >= 0) gpio_set_function((uint)rx_pin, UART_FUNCSEL_NUM(uart, rx_pin));
    return uart_init(uart, baud_rate);
}

#if USE_UART0 > 0

  static void (*rx0_chars_available_callback)(void) = NULL;
  #ifndef UART0_BAUDRATE
  #define UART0_BAUDRATE PICO_DEFAULT_UART_BAUD_RATE
  #endif

  bool uart0_init(void)
  {
    return uart_init_full(uart0, UART0_BAUDRATE, PICO_DEFAULT_UART0_TX_PIN, PICO_DEFAULT_UART0_RX_PIN);
  }

  static void on_uart0_rx(void) {
      if (rx0_chars_available_callback) {
          // Interrupts will go off until the uart is read, so disable them
          uart_set_irqs_enabled(uart0, false, false);
          rx0_chars_available_callback();
      }
  }

  void uart0_set_rxchars_callback(void (*fn)(void) ) 
  {
      uint irq_num = UART_IRQ_NUM(uart0);
      if (fn && !rx0_chars_available_callback) {
          rx0_chars_available_callback = fn;
          irq_set_exclusive_handler(irq_num, on_uart0_rx);
          irq_set_enabled(irq_num, true);
          uart_set_irqs_enabled(uart0, true, false);
      } else if (!fn && rx0_chars_available_callback) {
          uart_set_irqs_enabled(uart0, false, false);
          irq_set_enabled(irq_num, false);
          irq_remove_handler(irq_num, on_uart0_rx);
          rx0_chars_available_callback = NULL;
      }
  }

  void uart0_out_chars(const char *buf, int length) {
      for (int i = 0; i <length; i++) {
          uart_putc(uart0, buf[i]);
      }
  }

  int uart0_in_chars(char *buf, int length) {
      int i=0;
      while (i<length && uart_is_readable(uart0)) {
          buf[i++] = uart_getc(uart0);
      }
      if (rx0_chars_available_callback) {
          // Re-enable interrupts after reading a character
          uart_set_irqs_enabled(uart0, true, false);
      }
      return i ? i : PICO_ERROR_NO_DATA;
  }

  bool uart0_rxchars_available(void)
  {
    return uart_is_readable(uart0);
  }

#endif /* USE_UART0 */

#if USE_UART1 > 0
static void (*rx1_chars_available_callback)(void) = NULL;

#ifndef UART1_BAUDRATE
#define UART1_BAUDRATE PICO_DEFAULT_UART_BAUD_RATE
#endif

bool uart1_init(void)
{
  return uart_init_full(uart1, UART1_BAUDRATE, PICO_DEFAULT_UART1_TX_PIN, PICO_DEFAULT_UART1_RX_PIN);
}


  static void on_uart1_rx(void) {
      if (rx1_chars_available_callback) {
          // Interrupts will go off until the uart is read, so disable them
          uart_set_irqs_enabled(uart1, false, false);
          rx1_chars_available_callback();
      }
  }

  void uart1_set_rxchars_callback(void (*fn)(void) ) 
  {
      uint irq_num = UART_IRQ_NUM(uart1);
      if (fn && !rx1_chars_available_callback) {
          rx1_chars_available_callback = fn;
          irq_set_exclusive_handler(irq_num, on_uart1_rx);
          irq_set_enabled(irq_num, true);
          uart_set_irqs_enabled(uart1, true, false);
      } else if (!fn && rx1_chars_available_callback) {
          uart_set_irqs_enabled(uart1, false, false);
          irq_set_enabled(irq_num, false);
          irq_remove_handler(irq_num, on_uart1_rx);
          rx1_chars_available_callback = NULL;
      }
  }

  void uart1_out_chars(const char *buf, int length) {
      for (int i = 0; i <length; i++) {
          uart_putc(uart1, buf[i]);
      }
  }

  int uart1_in_chars(char *buf, int length) {
      int i=0;
      while (i<length && uart_is_readable(uart1)) {
          buf[i++] = uart_getc(uart1);
      }
      if (rx1_chars_available_callback) {
          // Re-enable interrupts after reading a character
          uart_set_irqs_enabled(uart1, true, false);
      }
      return i ? i : PICO_ERROR_NO_DATA;
  }

  bool uart1_rxchars_available(void)
  {
    return uart_is_readable(uart1);
  }

#endif /* USE_UART1 */