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

#if !defined(spi_default) 
    #error "No SPI-Handle for GC9A01 defined!"
#endif

#define LV_DRV_DISP_SPI_CS(lvl)             gpio_put(PICO_DEFAULT_SPI_CSN_PIN, lvl)
#define LV_DRV_DISP_RST(lvl)                gpio_put(PICO_DEFAULT_SPI_RESET_PIN, lvl);
#define LV_DRV_DISP_CMD_DATA(lvl)           gpio_put(PICO_DEFAULT_SPI_DNC_PIN, lvl);
#define LV_DRV_DISP_SPI_WR_BYTE(byte)       spi_write_blocking (spi_default, &byte, 1)

bool spi_setup_dma(void);
typedef void (pfn_spi_done_cb) (void);
void LV_DRV_DELAY_MS(uint32_t wait_ms);
void LV_DRV_DISP_SPI_WR_ARRAY(const uint8_t *arr, uint32_t len, pfn_spi_done_cb done_cb);
