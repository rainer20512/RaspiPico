
/**
  ******************************************************************************
  * @file    xml_feeder.h
  * @author  Rainer
  * @brief   functions to feed input into XML parser
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


#endif /* _XML_FEEDER_H_  */