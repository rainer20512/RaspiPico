/**
  ******************************************************************************
  * @file    debug_util.c
  * @author  Rainer
  * @brief   functions for debugging/dumping GPIO and EXTI Settings
  *
  ******************************************************************************
  *
  ******************************************************************************
  */

/** @addtogroup DEBUG_UTILS
  * @{
  */

#include "config/config.h"

#if DEBUG_FEATURES > 0

#include <string.h>

#include "debug/debug_helper.h"
#include "hardware/dma.h"
#include "system/dma_irq.h"

/*
 *************************************************************
 * defines
 *************************************************************
 */

/*
 *************************************************************
 * local helper functions 
 *************************************************************
 */



#define MAX_REQNUM 64
const char * const dma_req_name[MAX_REQNUM]= { 
    /*.              .              .              .              .              .              .              . */
      "PIO0_TX0",    "PIO0_TX1",    "PIO0_TX2",    "PIO0_TX3",    "PIO0_RX0",    "PIO0_RX1",    "PIO0_RX2",    "PIO0_RX3",        
      "PIO1_TX0",    "PIO1_TX1",    "PIO1_TX2",    "PIO1_TX3",    "PIO1_RX0",    "PIO1_RX1",    "PIO1_RX2",    "PIO1_RX3",        
      "SPI0_TX",     "SPI0_RX",     "SPI1_TX",     "SPI1_RX",     "UART0_TX",    "UART0_RX",    "UART1_TX",    "UART1_RX",
      "PWM_WRAP0",   "PWM_WRAP1",   "PWM_WRAP2",   "PWM_WRAP3",   "PWM_WRAP4",   "PWM_WRAP5",   "PWM_WRAP6",   "PWM_WRAP7",   
      "I2C0_TX",     "I2C0_RX",     "I2C1_TX",     "I2C1_RX",     "ADC",         "XIP_STREAM",  "XPI_SSITX",   "XPI_SSIRX",   
      "UNDEF",       "UNDEF",       "UNDEF",       "UNDEF",       "UNDEF",       "UNDEF",       "UNDEF",       "UNDEF",
      "UNDEF",       "UNDEF",       "UNDEF",       "UNDEF",       "UNDEF",       "UNDEF",       "UNDEF",       "UNDEF",
      "UNDEF",       "UNDEF",       "UNDEF",       "TIMER0",      "TIMER1",      "TIMER2",      "TIMER3",      "PERMANT.",     
    };

/*
 *************************************************************
 * @brief Fill in the NVIC IRQ name
 * @param sel    - NVIC irq number
 * @param retbuf - return buffer to be written to
 * @param maxlen - length of retbuf on retrun, right padded with blanks
 * @note retbuf must provide at least maxlen+1 bytes !!!
 *************************************************************
 */
static void DBG_get_dma_req_name(int32_t dmanum, char *retbuf, const uint32_t maxlen)
{
  const char *p;

  memset(retbuf, ' ', maxlen );
  if ( dmanum < (int32_t)(sizeof(dma_req_name)/sizeof(char *)) ) {
    p = dma_req_name[dmanum];
    /* Copy Name to return buffer */
    strncpy(retbuf, p, strlen(p) );
  } else {
    /* write directly to return buffer */
    snprintf(retbuf, maxlen, "?#%03d?", dmanum); 
    /* and overwrite the terminating \0 with blank to adjust length of string */
    retbuf[strlen(retbuf)] = ' ' ;
  }

  /* append terminating \0 in any case */
  retbuf[maxlen] = '\0';
}

/*-----------------------------------------------------------------------------
 * Dump active DMA channels
 *---------------------------------------------------------------------------*/
void DBG_dump_dma_config(void)
{
    #define DMA_CHNMAX    12    /* RP2040 has 12 DMA channels */

    DEBUG_PRINTF  ("Status of DMA Controller ------------------------------------------------\n");

    
    int oldIndent = DBG_setIndentRel(+2);
    uint32_t mask,i, ctrl;
    #define REQSTRLEN   10
    char reqname[REQSTRLEN+1];
      DEBUG_PRINTF("Ch  Rd addr Inc?  Wr addr Inc?  Request   Irq  P Irq  P\n");
           /*       12 0x12345678 +  0x12345678 +  0123456789 Irq0 * Irq1 *  */
    for ( i=0, mask=1; i <  DMA_CHNMAX; i++, mask <<=1 ) {
      ctrl = dma_hw->ch[i].ctrl_trig;
      DEBUG_PRINTF("%02d ",i);
      if ( ( ctrl & DMA_CH0_CTRL_TRIG_EN_BITS ) == 0) {
        DEBUG_PRINTF("disabled");
      } else {
        DBG_get_dma_req_name((ctrl & DMA_CH0_CTRL_TRIG_TREQ_SEL_BITS) >> DMA_CH0_CTRL_TRIG_TREQ_SEL_LSB, reqname, REQSTRLEN);
        DEBUG_PRINTF("0x%08x %c  0x%08x %c  %s %s %c %s %c", 
          dma_hw->ch[i].read_addr,  ctrl & DMA_CH0_CTRL_TRIG_INCR_READ_BITS ? '+':'=',
          dma_hw->ch[i].write_addr, ctrl & DMA_CH0_CTRL_TRIG_INCR_WRITE_BITS ? '+':'=',
          reqname,
          dma_hw->inte0 & mask ? "Irq0":"    ", dma_hw->ints0 & mask ? '*':' ',
          dma_hw->inte1 & mask ? "Irq1":"    ", dma_hw->ints1 & mask ? '*':' '
        );
        /* Ensure, that my dma channel handler knows about this source */
        if ( !dma_channel_assigned(i) ) DEBUG_PRINTF(" ***Unknown to DMA-coordinator***");
      }
      DEBUG_PRINTF("\n");
    }

    DBG_setIndentAbs(oldIndent);
}


#endif // #if DEBUG_FEATURES > 0

/**
  * @}
  */
