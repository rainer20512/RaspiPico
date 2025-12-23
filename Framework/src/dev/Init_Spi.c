/**
  ******************************************************************************
  * @file    Init_Spi.c
  * @author  Rainer 
  * @brief   Initialize SPI Interface(s)
  ******************************************************************************
  *
  ******************************************************************************
  * @addtogroup Spi
  * @{
  */

#include "config/config.h"

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"


#if USE_SPI0 > 0
  static uint8_t bSpi0_Initialized = false;
  bool Init_Spi0 ( void ) {

  #if !defined(spi0) || !defined(PICO_DEFAULT_SPI0_SCK_PIN) || !defined(PICO_DEFAULT_SPI0_TX_PIN) || !defined(PICO_DEFAULT_SPI0_DNC_PIN) || !defined(PICO_DEFAULT_SPI0_CSN_PIN) || !defined(PICO_DEFAULT_SPI0_RESET_PIN)
      #warning SPI0 not configured!
      puts("Default SPI0 pins were not defined");
      return false;
  #endif


      // 500kHz, TX only
      spi_init(spi0, 1000 * 1000);
      gpio_set_function(PICO_DEFAULT_SPI0_SCK_PIN, GPIO_FUNC_SPI);
      gpio_set_function(PICO_DEFAULT_SPI0_TX_PIN, GPIO_FUNC_SPI);

      // Make the SPI pins available to picotool
      bi_decl(bi_3pins_with_func(PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI));

      // Chip select is active-low, so we'll initialise it to a driven-high state
      gpio_init(PICO_DEFAULT_SPI0_CSN_PIN);
      gpio_set_dir(PICO_DEFAULT_SPI0_CSN_PIN, GPIO_OUT);
      gpio_put(PICO_DEFAULT_SPI0_CSN_PIN, 1);
      gpio_set_pulls(PICO_DEFAULT_SPI0_CSN_PIN,1,0);
      // Make the CS pin available to picotool
      bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI0_CSN_PIN, "SPI0 CS")); 
 

      // D/nC 
      gpio_init(PICO_DEFAULT_SPI0_DNC_PIN);
      gpio_set_dir(PICO_DEFAULT_SPI0_DNC_PIN, GPIO_OUT);
      // Make the CS pin available to picotool
      bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI_DNC_PIN, "SPI0 DnC")); 


      // Reset
      gpio_init(PICO_DEFAULT_SPI0_RESET_PIN);
      gpio_set_dir(PICO_DEFAULT_SPI0_RESET_PIN, GPIO_OUT);
      // Make the CS pin available to picotool
      bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI0_RESET_PIN, "SPI0 Reset")); 


      return true;
  }
#endif /* USE_SPI0 */

#if USE_SPI1 > 0
  static uint8_t bSpi1_Initialized = false;
  bool Init_Spi1 ( void ) {

  #if !defined(spi1) || !defined(PICO_DEFAULT_SPI1_SCK_PIN) || !defined(PICO_DEFAULT_SPI1_TX_PIN) || !defined(PICO_DEFAULT_SPI1_DNC_PIN) || !defined(PICO_DEFAULT_SPI1_CSN_PIN) || !defined(PICO_DEFAULT_SPI1_RESET_PIN)
      #warning SPI1 not configured!
      puts("Default SPI1 pins were not defined");
      return false;
  #endif


      // 500kHz, TX only
      spi_init(spi1, 8000 * 1000);
      gpio_set_function(PICO_DEFAULT_SPI1_SCK_PIN, GPIO_FUNC_SPI);
      gpio_set_function(PICO_DEFAULT_SPI1_TX_PIN, GPIO_FUNC_SPI);

      // Make the SPI pins available to picotool
      bi_decl(bi_3pins_with_func(PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI));

      // Chip select is active-low, so we'll initialise it to a driven-high state
      gpio_init(PICO_DEFAULT_SPI1_CSN_PIN);
      gpio_set_dir(PICO_DEFAULT_SPI1_CSN_PIN, GPIO_OUT);
      gpio_put(PICO_DEFAULT_SPI1_CSN_PIN, 1);
      gpio_set_pulls(PICO_DEFAULT_SPI1_CSN_PIN,1,0);
      // Make the CS pin available to picotool
      bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI1_CSN_PIN, "SPI1 CS")); 
 

      // D/nC 
      gpio_init(PICO_DEFAULT_SPI1_DNC_PIN);
      gpio_set_dir(PICO_DEFAULT_SPI1_DNC_PIN, GPIO_OUT);
      // Make the CS pin available to picotool
      bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI_DNC_PIN, "SPI1 DnC")); 


      // Reset
      gpio_init(PICO_DEFAULT_SPI1_RESET_PIN);
      gpio_set_dir(PICO_DEFAULT_SPI1_RESET_PIN, GPIO_OUT);
      // Make the CS pin available to picotool
      bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI1_RESET_PIN, "SPI1 Reset")); 


      return true;
  }
#endif /* USE_SPI1 */


/**
  * @}
  */
