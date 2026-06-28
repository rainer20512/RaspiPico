/*
 ******************************************************************************
 * @file    i2cx.h
 * @author  Rainer
 * @brief   low level functions i2c0/i2c1
 *
 ******************************************************************************
 */
#ifndef __I2CX_H_
#define __I2CX_H_
#include "config/config.h"

#include "config/config.h"
#include "pico/types.h"

#define PICO_DEFAULT_I2C0_SDA_PIN 20
#define PICO_DEFAULT_I2C0_SCL_PIN 21
#define I2C0_BAUDRATE             100000

#define PICO_DEFAULT_I2C1_SDA_PIN 2
#define PICO_DEFAULT_I2C1_SCL_PIN 3
#define I2C1_BAUDRATE             100000


void i2c_init_all       (void);
void i2c_error          (void);
bool i2c_setup_dma      (void);
void i2c_enable_dma     (bool bEna);
bool i2c_is_dma_enabled (void);

#endif /* __I2CX_H_ */