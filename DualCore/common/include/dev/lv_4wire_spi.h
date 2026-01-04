/*
 ******************************************************************************
 * @file    lv_4wire_spi.h
 * @author  Rainer
 * @brief   low level functions for 4-wire-spi
 *
 ******************************************************************************
 */

#include "config/config.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

#if !defined(DISP_SPI_NUM) 
    #error "No SPI-Handle for GC9A01 defined!"
#endif

#if DISP_SPI_NUM == 0
  #define LV_DRV_DISP_SPI_CS(lvl)                 gpio_put(PICO_DEFAULT_SPI0_CSN_PIN, lvl)
  #define LV_DRV_DISP_RST(lvl)                    gpio_put(PICO_DEFAULT_SP10_RESET_PIN, lvl);
  #define LV_DRV_DISP_CMD_DATA(lvl)               gpio_put(PICO_DEFAULT_SPI0_DNC_PIN, lvl);
  #define LV_DRV_DISP_SPI_WR_BYTE(byte)           spi_write_blocking (spi0, &byte, 1)
#elif DISP_SPI_NUM == 1
  #define LV_DRV_DISP_SPI_CS(lvl)                 gpio_put(PICO_DEFAULT_SPI1_CSN_PIN, lvl)
  #define LV_DRV_DISP_RST(lvl)                    gpio_put(PICO_DEFAULT_SPI1_RESET_PIN, lvl);
  #define LV_DRV_DISP_CMD_DATA(lvl)               gpio_put(PICO_DEFAULT_SPI1_DNC_PIN, lvl);
  #define LV_DRV_DISP_SPI_WR_BYTE(byte)           spi_write_blocking (spi1, &byte, 1)
#else
  #error "No SPI definitions for LVGL"
#endif

/* Reading from display not possible - no SDO line */
#define LV_DRV_DISP_SPI_RD_VECTOR(vector, len)  spi_error()

void spi_error(void);
bool spi_setup_dma(void);
typedef void (pfn_spi_done_cb) (void);
void LV_DRV_DELAY_MS(uint32_t wait_ms);
void LV_DRV_DISP_SPI_WR_BYTE_ARRAY(const uint8_t *arr, uint32_t len, pfn_spi_done_cb done_cb);
void LV_DRV_DISP_SPI_WR_WORD_ARRAY(uint16_t *arr, uint32_t len, pfn_spi_done_cb done_cb);
