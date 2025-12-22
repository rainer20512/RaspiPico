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

#include "dev/lv_4wire_spi.h"
#include "debug/debug_helper.h"

#define LV_USE_SPI_DMA    0

static pfn_spi_done_cb *tx_done_cb;

#if LV_USE_SPI_DMA > 0

#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hardware/uart.h"
#include "system/profiling.h"
#include "debug/debug_helper.h"

#define PFX CONCAT(DREQ_SPI,DISP_SPI_NUM)
#define SPI_TX_DMA_CHANNEL CONCAT(PFX,_TX)

static int spi_dma_chan;

void SPI_TX_handler(void)
{
  ProfilerPush(JOB_IRQ_SPI);
  dma_channel_acknowledge_irq1 (spi_dma_chan);
  // SPI_tx_CompleteCB ( tx_transfer_size );
  tx_done_cb();
  uint32_t ctrl = dma_get_channel_config(spi_dma_chan).ctrl;
  DEBUG_PRINTF(" ctrl=%08x", ctrl);
  ProfilerPop();
}

bool spi_setup_dma(void)
{
    spi_dma_chan = dma_claim_unused_channel(true);
    dma_channel_config c = dma_channel_get_default_config(spi_dma_chan);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
    channel_config_set_write_increment(&c, false);
    channel_config_set_dreq(&c, SPI_TX_DMA_CHANNEL);

    dma_channel_configure(
        spi_dma_chan,
        &c,
        &spi_get_hw(spi_default)->dr, // Write address (only need to set this once)
        NULL,             // read address will be set later
        0,                // Transfer size will be set later
        false             // Don't start yet
    );

    // Tell the DMA to raise IRQ line 0 when the channel finishes a block
    dma_channel_set_irq1_enabled(spi_dma_chan, true);

    // Configure the processor to run dma_handler() when DMA IRQ 0 is asserted
    irq_set_exclusive_handler(DMA_IRQ_1, SPI_TX_handler);
    irq_set_enabled(DMA_IRQ_1, true);

    return true;
}

static void StartTxDma(const uint8_t *data, uint32_t txSize)
{
  /* setup and start dma */
  dma_channel_set_read_addr   (spi_dma_chan, data, false);
  dma_channel_set_trans_count (spi_dma_chan, txSize, true);
}
#else
bool spi_setup_dma(void) {}
#endif /* LV_USE_SPI_DMA */

void spi_error(void)
{
  DEBUG_PUTS("Error - Reading from display not possible: No SDO line!");
}

void LV_DRV_DELAY_MS(uint32_t wait_ms)
{
    sleep_ms(wait_ms);
}

/* 
 * Write byte vector. Pay attention when underlying data are not bytes bcs little
 * endian order is not reversed.
 * Use LV_DRV_DISP_SPI_WR_WORD_ARRAY instead, when endianess is to be preserved
 */
void LV_DRV_DISP_SPI_WR_BYTE_ARRAY(const uint8_t *arr, uint32_t len, pfn_spi_done_cb done_cb)
{
  assert(done_cb);
  tx_done_cb = done_cb;

  #if LV_USE_SPI_DMA > 0
    StartTxDma(arr, len);
  #else
    /* Non DMA transfer */
    while ( len-- ) {
        LV_DRV_DISP_SPI_WR_BYTE(*(arr++));
    }
    done_cb();
  #endif
}

static void LV_DRV_DISP_SPI_WR_WORD ( uint16_t word ) 
{
  union myword_u {
                    uint16_t w;
                    uint8_t  b[2];
  } myword;
  myword.w = word;
  LV_DRV_DISP_SPI_WR_BYTE(myword.b[1]);
  LV_DRV_DISP_SPI_WR_BYTE(myword.b[0]);
}

/* 
 * Write word vector. Pay attention when underlying data are not word 
 * bcs hi byte is sent first, ie byte order is swapped
 * Use LV_DRV_DISP_SPI_WR_BYTE_ARRAY instead, when byte order has to be preserved
 */
void LV_DRV_DISP_SPI_WR_WORD_ARRAY(const uint16_t *arr, uint32_t len, pfn_spi_done_cb done_cb)
{
  assert(done_cb);
  tx_done_cb = done_cb;

  #if LV_USE_SPI_DMA > 0
    StartTxDma(arr, len);
  #else
    /* Non DMA transfer */
    while ( len-- ) {
        LV_DRV_DISP_SPI_WR_WORD(*(arr++));
    }
    done_cb();
  #endif
}
