/*
 ******************************************************************************
 * @file    lv_4wire_spi.h 
 * @author  Rainer
 * @brief   low level functions for 4-wire-spi
 *
 ******************************************************************************
 */

#include "config/config.h"

#include "pico/time.h"

#include "./lv_4wire_spi.h"


void LV_DRV_DELAY_MS(uint32_t wait_ms)
{
    sleep_ms(wait_ms);
}

void LV_DRV_DISP_SPI_WR_ARRAY(const uint8_t *arr, uint32_t len)
{
    while ( len-- ) {
        LV_DRV_DISP_SPI_WR_BYTE(*(arr++));
    }
}