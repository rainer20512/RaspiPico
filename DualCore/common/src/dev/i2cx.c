/**
  ******************************************************************************
  * @file    i2cx.c
  * @author  Rainer 
  * @brief   i2c0/i2c1 functions
  ******************************************************************************
  *
  ******************************************************************************
  * @addtogroup I2c
  * @{
  */

#include "config/config.h"
#if USE_I2C0 > 0 || USE_I2C1 > 0 
#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "pico/i2c_slave.h"

#include "dev/i2cx.h"

#if USE_I2C0 > 0
  bool Init_I2C0 ( void ) {

  #if !defined(i2c0) || !defined(PICO_DEFAULT_I2C0_SDA_PIN) || !defined(PICO_DEFAULT_I2C0_SCL_PIN) || !defined(I2C0_BAUDRATE)
      #warning I2C0 not configured!
      puts("Default I2C0 pins were not defined");
      return false;
  #endif

      i2c_init(i2c0, I2C0_BAUDRATE);
  
      gpio_set_function(PICO_DEFAULT_I2C0_SDA_PIN, GPIO_FUNC_I2C);
      gpio_set_function(PICO_DEFAULT_I2C0_SCL_PIN, GPIO_FUNC_I2C);
      /* Enable Pullups for both lines */
      gpio_pull_up(PICO_DEFAULT_I2C0_SDA_PIN);
      gpio_pull_up(PICO_DEFAULT_I2C0_SCL_PIN);

      // Make the I2C pins available to picotool
      bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C0_SDA_PIN, PICO_DEFAULT_I2C0_SCL_PIN, GPIO_FUNC_I2C));

      return true;
  }
#endif /* USE_SPI0 */

#if USE_I2C1 > 0
  bool Init_I2C1 ( void ) {

  #if !defined(i2c1) || !defined(PICO_DEFAULT_I2C1_SDA_PIN) || !defined(PICO_DEFAULT_I2C1_SCL_PIN) || !defined(I2C1_BAUDRATE)
      #warning I2C1 not configured!
      puts("Default I2C1 pins were not defined");
      return false;
  #endif

      i2c_init(i2c1, I2C1_BAUDRATE);
  
      gpio_set_function(PICO_DEFAULT_I2C1_SDA_PIN, GPIO_FUNC_I2C);
      gpio_set_function(PICO_DEFAULT_I2C1_SCL_PIN, GPIO_FUNC_I2C);
      /* Enable Pullups for both lines */
      gpio_pull_up(PICO_DEFAULT_I2C1_SDA_PIN);
      gpio_pull_up(PICO_DEFAULT_I2C1_SCL_PIN);

      // Make the I2C pins available to picotool
      bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C1_SDA_PIN, PICO_DEFAULT_I2C1_SCL_PIN, GPIO_FUNC_I2C));

      return true;
  }
#endif /* USE_I2C1 */

#if ( USE_I2C1 > 0 || USE_I2C0 > 0 )
  void i2c_init_all ( void )
  {
    #if USE_I2C0 > 0
      Init_I2C0();
    #endif  
    #if USE_I2C1 > 0
      Init_I2C1();
    #endif  
  }
#else
  #define I2C_init_all() 
#endif

#endif // USE_I2C0 > 0 || USE_I2C1 > 0 


/**
  * @}
  */
