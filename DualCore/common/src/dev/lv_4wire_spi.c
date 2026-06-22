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

#define LV_USE_SPI_DMA    1

static pfn_spi_done_cb *tx_done_cb;

#if LV_USE_SPI_DMA > 0

#include "hardware/dma.h"
#include "system/dma_irq.h"
#include "system/ipc_msg.h"
#include "hardware/uart.h"
#include "system/profiling.h"
#include "debug/debug_helper.h"

#ifndef DISP_SPI_NUM
  #error "DISP_SPI_NUM not defined"
#elif DISP_SPI_NUM == 0
  #define SPI_TX_DMA_CHANNEL  DREQ_SPI0_TX
  #define SPI_HW              spi0
#elif DISP_SPI_NUM == 1
  #define SPI_TX_DMA_CHANNEL  DREQ_SPI1_TX
  #define SPI_HW              spi1
  #define SPI_ACK             dma_channel_acknowledge_irq1
#else
  #error "no assignemnt for DISP_SPI_NUM"
#endif

static int spi_dma_chan;
static bool bSpiDMA = false;         /* initially, SIP DMA is off */

void SPI_TX_handler(void)
{
  ProfilerPush(JOB_IRQ_DMA);
  dma_channel_acknowledge_irq(spi_dma_chan);
  dma_channel_irq_enable( spi_dma_chan, false);


  // Drain RX FIFO, then wait for shifting to finish (which may be *after*
  // TX FIFO drains), then drain RX FIFO again
  while (spi_is_readable(SPI_HW))
      (void)spi_get_hw(SPI_HW)->dr;
  while (spi_get_hw(SPI_HW)->sr & SPI_SSPSR_BSY_BITS)
      tight_loop_contents();
  while (spi_is_readable(SPI_HW))
      (void)spi_get_hw(SPI_HW)->dr;

  // Don't leave overrun flag set
  spi_get_hw(SPI_HW)->icr = SPI_SSPICR_RORIC_BITS;

  tx_done_cb();
  // uint32_t ctrl = dma_get_channel_config(spi_dma_chan).ctrl;
  // DEBUG_PRINTF(" ctrl=%08x\n", ctrl);
  ProfilerPop();
}

bool spi_setup_dma(void)
{
#if RP2040_M0_0
    spi_dma_chan = dma_claim_unused_channel(true);
#elif RP2040_M0_1
    spi_dma_chan = core1_bootinfo.c1_spi_dma_chan;
      #endif
    dma_channel_config c = dma_channel_get_default_config(spi_dma_chan);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
    channel_config_set_write_increment(&c, false);
    channel_config_set_dreq(&c, SPI_TX_DMA_CHANNEL);

    dma_channel_configure(
        spi_dma_chan,
        &c,
        &spi_get_hw(SPI_HW)->dr, // Write address (only need to set this once)
        NULL,             // read address will be set later
        0,                // Transfer size will be set later
        false             // Don't start yet
    );


    // Configure the processor to run SPI_TX_handler() when DMA IRQ is asserted
    dma_channel_set_handler(spi_dma_chan, SPI_TX_handler);
    dma_channel_irq_enable(spi_dma_chan, true);

    return true;
}

void spi_enable_dma( bool bEna )
{
  bSpiDMA = bEna;
}

bool spi_is_dma_enabled (void)
{
  return bSpiDMA;
}

static void StartTxDma(const uint8_t *data, uint32_t txSize)
{
  /* clear "old" dma Interrupts and enable spi dma interrupt */
  dma_channel_acknowledge_irq(spi_dma_chan);
  dma_channel_irq_enable( spi_dma_chan, true);
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

  if ( bSpiDMA ) {
    StartTxDma(arr, len);
  } else {
    /* Non DMA transfer */
    while ( len-- ) {
        LV_DRV_DISP_SPI_WR_BYTE(*(arr++));
    }
    done_cb();
  }
}

/* 
 * Write word vector. Pay attention when underlying data are not word 
 * bcs hi byte is sent first, ie byte order is swapped
 * Use LV_DRV_DISP_SPI_WR_BYTE_ARRAY instead, when byte order has to be preserved
 * ATTENTION: the passed word array is modified by byte swapping!
 */
void LV_DRV_DISP_SPI_WR_WORD_ARRAY(uint16_t *arr, uint32_t len, pfn_spi_done_cb done_cb)
{
  uint32_t i;
  uint16_t *p;

  assert(done_cb);
  tx_done_cb = done_cb;

  /* Swap bytes in all words, ie convert from small to big endian */
  
  for ( i=0, p=arr; i < len; i++, p++ ) {
    *p = __builtin_bswap16(*p);
  }

  /* Send all bytes, non DMA transfer */
  LV_DRV_DISP_SPI_WR_BYTE_ARRAY((const uint8_t *)arr, len*2, done_cb );
}
