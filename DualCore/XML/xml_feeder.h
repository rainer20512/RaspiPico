
/**
  ******************************************************************************
  * @file    xml_feeder.h
  * @author  Rainer
  * @brief   functions to feed input into XML parser and to write buffered output
  *          to I2C interface
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _XML_FEEDER_H_
#define _XML_FEEDER_H_
#include "config/config.h"

#include "pico/i2c_slave.h"

void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event);
void i2c_feeder_init  (void);

#define FAKEOUTPUT 0
#if FAKEOUTPUT > 0
  #define output_putch(c)
  #define output_putsl(i,l)
  #define output_puts(i)
  #define output_printf(f, ...)
#else
  void output_putch     (char c);
  void output_putsl     (const char *in, uint8_t len);
  void output_puts      (const char *in);
  int  output_printf    (const char *format, ...);
#endif

#endif /* _XML_FEEDER_H_  */