/**
  ******************************************************************************
  * @file    cmdline.c
  * @author  Rainer 
  * @brief   Functions to handle / execute command line commands
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


static uint8_t bSpi_Initialized;
bool Init_Spi1 ( void ) {

#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_DNC_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN) || !defined(PICO_DEFAULT_SPI_RESET_PIN)
    #warning SPI1 not configured!
    puts("Default SPI1 pins were not defined");
    bSpi_Initialized = false;
    return;
#endif


    // 500kHz, TX only
    spi_init(spi_default, 500 * 1000);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);

    // Make the SPI pins available to picotool
    bi_decl(bi_3pins_with_func(PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI));

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    gpio_set_pulls(PICO_DEFAULT_SPI_CSN_PIN,1,0);
    // Make the CS pin available to picotool
    bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI_CSN_PIN, "SPI CS")); 
 

    // D/nC 
    gpio_init(PICO_DEFAULT_SPI_DNC_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_DNC_PIN, GPIO_OUT);
    // Make the CS pin available to picotool
    bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI_DNC_PIN, "SPI DnC")); 


    // Reset
    gpio_init(PICO_DEFAULT_SPI_RESET_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_RESET_PIN, GPIO_OUT);
    // Make the CS pin available to picotool
    bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI_RESET_PIN, "SPI Reset")); 


    return true;
}



/**
  * @}
  */
