/**
  ******************************************************************************
  * @file    debug_io0.c
  * @author  Rainer
  * @brief   Implementation of a circular output buffer, that will automatically
  *          be written to DebugUart
  *
  ******************************************************************************
  *
  ******************************************************************************
  */

/** @addtogroup DEBUG_OUTBUF
  * @{
  */

#include "config/config.h"
#include "debug.h"
#include "task/minitask.h"



#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "debug/debug_helper.h"
// #include "debug_io0.h"

#include "system/circbuf.h"
#include "system/util.h"
#include "system/profiling.h"

#include "dev/uarts.h"
#include "pico/time.h"

#define USE_UART_DMA  1
/* Private define ------------------------------------------------------------*/

/* size of RX/TX buffers, sizes must be power of two  */
#define OUTBUF0_SIZE 2048   
#define INBUF0_SIZE  64

/* Private variables ---------------------------------------------------------*/
static uint8_t bOngoingTransfer0 = 0;    /* indicates an ongoing transfer, if != 0 */
static uint8_t bExpandCrToCrlf0 = 0;     /* expand LF to CRLF */
static uint8_t bDelayedFlush0 = 0;       /* Keep in mind an delayed flush */

/**** 003 **** 
 * Whenever BDMA is used ( here: for LPUART1 ), the DMA memory hasto reside
 * in SRAM4. 
 */
static CircBuffT o0;
static LinBuffT i0; 

/* RHB tbd */
#define DMAMEM

static DMAMEM uint8_t outbuf[OUTBUF0_SIZE];
static DMAMEM uint8_t inbuf[INBUF0_SIZE];
static int uart_dma_chan;
static uint32_t tx_transfer_size;     // size of currently sent dma block

/* Private macro -------------------------------------------------------------*/

/* forward declarations  -----------------------------------------------------*/
static void DebugOutputCompleteCB ( uint32_t size );

/* Private functions  ---------------------------------------------------------*/
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hardware/uart.h"
#include "system/ipc_msg.h"

#define PFX CONCAT(DREQ_UART,DEBUG_UART)
#define UART_TX_DMA_CHANNEL CONCAT(PFX,_TX)

#if USE_UART_DMA > 0
#if RP2040_M0_0 
  void DMA_TX0_handler(void)
  {
    ProfilerPush(JOB_IRQ_DMA);
    if (dma_channel_get_irq0_status(uart_dma_chan)) {
      dma_channel_acknowledge_irq0 (uart_dma_chan);
      DebugOutputCompleteCB ( tx_transfer_size );
    }
    ProfilerPop();
  }
#endif

#if RP2040_M0_1
  void DMA_TX1_handler(void)
  {
    ProfilerPush(JOB_IRQ_DMA);
    if (dma_channel_get_irq1_status(uart_dma_chan)) {
      dma_channel_acknowledge_irq1 (uart_dma_chan);
      DebugOutputCompleteCB ( tx_transfer_size );
    }
    ProfilerPop();
  }
#endif


  static bool uart_setup_dma_channel(void)
  {
      #if RP2040_M0_0
        uart_dma_chan = dma_claim_unused_channel(true);
      #elif RP2040_M0_1
        uart_dma_chan = core1_bootinfo.c1_uart_dma_chan;
      #endif
      dma_channel_config c = dma_channel_get_default_config(uart_dma_chan);
      channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
      channel_config_set_write_increment(&c, false);
      channel_config_set_dreq(&c, UART_TX_DMA_CHANNEL);

      dma_channel_configure(
          uart_dma_chan,
          &c,
          &uart_get_hw(DEBUG_UART_INSTANCE())->dr, // Write address (only need to set this once)
          NULL,             // read address will be set later
          0,                // Transfer size will be set later
          false             // Don't start yet
      );

      #if   RP2040_M0_0
            // Tell the DMA to raise IRQ line 0 when the channel finishes a block
            dma_channel_set_irq0_enabled(uart_dma_chan, true);
            // Configure the processor to run dma_handler() when DMA IRQ 0 is asserted
            irq_set_exclusive_handler(DMA_IRQ_0, DMA_TX0_handler);
            irq_set_enabled(DMA_IRQ_0, true);
      #elif RP2040_M0_1 || defined(CORE1_SIM)
            // Tell the DMA to raise IRQ line 1 when the channel finishes a block
            dma_channel_set_irq1_enabled(uart_dma_chan, true);
            // Configure the processor to run dma_handler() when DMA IRQ 0 is asserted
            irq_set_exclusive_handler(DMA_IRQ_1, DMA_TX1_handler);
            irq_set_enabled(DMA_IRQ_1, true);
      #endif
  }

  static void UsartStartTx(uint8_t *data, uint32_t txSize)
  {
    /* setup and start dma */
    dma_channel_set_read_addr   (uart_dma_chan, data, false);
    dma_channel_set_trans_count (uart_dma_chan, txSize, true);
  }
#else 
  static void UsartStartTx(uint8_t *data, uint32_t txSize)
  {
    #if DEBUG_UART == 1
        uart1_out_chars((const char *)data, txSize );
    #else
        uart0_out_chars((const char *)data, txSize );
    #endif
    DebugOutputCompleteCB ( txSize );
  }
#endif

/* Start transfer to output device */
static void Debug_OutputTransfer(uint32_t from, uint32_t to )
{
  /* Make sure, there is no other active transfer */
  assert_param(!bOngoingTransfer0);
  /* Make sure, there is no wraparound between 'from' and 'to'  */
  assert_param(from <= to);

  bOngoingTransfer0 = 1;
  tx_transfer_size = to - from;

  UsartStartTx(o0.buf+o0.rdptr, tx_transfer_size);

}


/* Public functions  ---------------------------------------------------------*/


#if DEBUG_SLEEP_STOP > 0
  /*********************************************************************************
    * @brief  Dumps current values of outbuf read an write pointers to 
    * @param  None
    * @retval None
    * @note   NO NORMAL DEBUG OUTPUT in this function or childs, 
    *         use special functions only!*
    ********************************************************************************/
  void DebugOutbufGetPtrs(void)
  {
    store_hexXXXX(o0.rdptr);
    store_chr('/');
    store_hexXXXX(o0.wrptr);
    store_chr(' ');
  }
#endif


/*
 * Callback when transfer complete *
 * - reset ongoing-transfer-flag
 * - increment txptr
 * - check for wraparound and continue transfer in case of wraparound 
 */
static void DebugOutputCompleteCB ( uint32_t size )
{
  bOngoingTransfer0 = 0;
  o0.rdptr = CBUFPTR_INCR(o0, rdptr, size);

  /* If we kept in mind an delayed flush, initiate it now */
  if ( bDelayedFlush0 ) {
    bDelayedFlush0 = 0;
    TaskNotify(TASK_LOG);
  }

}


/* Copy the content of the output buffer to output device */
void task_handle_out(uint32_t arg)
{
    UNUSED(arg);
   
   /* Don't flush when empty */
   if ( CBUF_EMPTY(o0) ) return;

   /* Check for ongoing transfer. If so, keep in mind to flush afterwards */ 
   if ( bOngoingTransfer0 ) {
     bDelayedFlush0 = 1;
     return;
   }

  /*
   * If the buffer content wraps around, only transfer up to the end of the circular buffer
   * and keep in mind to transfer from begin later
   */
  if ( CBUF_WRAPAROUND(o0) ) {
     bDelayedFlush0 = 1;
     Debug_OutputTransfer(o0.rdptr, OUTBUF0_SIZE );
   } else {
     Debug_OutputTransfer(o0.rdptr, o0.wrptr );
   }
}

/*
 * redefine system function __putchar so that standard io functions can be used to produce debug output
 */
/* int __putchar(int ch ) { */
int __putchar(int ch, __printf_tag_ptr uu) {
  if ( bExpandCrToCrlf0 && (char)ch == '\n' ) __putchar('\r', uu);
  // if ( bExpandCrToCrlf0 && (char)ch == '\n' ) __putchar('\r'); */

  /*
   * - if there is room left in outbuf, copy character to outbuf and start transfer, if character is '\n'
   * - otherwise replace the last written character with * to indicate overflow and start transfer
   */

  if ( !CircBuff_Put(&o0, ch ) ) {
      TaskNotify(TASK_LOG);
  } else { 
    if ( ch == '\n') {
      TaskNotify(TASK_LOG);
    }
  }
     
  return ch;
}

extern void LL_Blink(uint32_t nrofblinks, uint32_t delayms );

/**
* @brief  Callback when input character(s) available
* @param  UartCOMx: UART handle
* @retval None
*/
void Debug_RxCharAvailCB(void) 
{
    #if RP2040_M0_1
      LL_Blink(4, 80);
    #endif
    ProfilerPush(JOB_IRQ_UART);
    TaskNotify(TASK_COM);
    ProfilerPop();
}


bool task_init_io(void) 
{
    CircBuff_Init         (&o0, OUTBUF0_SIZE, outbuf);
    LinBuff_Init          (&i0, INBUF0_SIZE,  inbuf );
    bExpandCrToCrlf0 = true;
    #if USE_UART_DMA > 0
        uart_setup_dma_channel();
    #endif
    #if DEBUG_UART == 0
      uart0_set_rxchars_callback(Debug_RxCharAvailCB);
    #elif DEBUG_UART == 1
      uart1_set_rxchars_callback(Debug_RxCharAvailCB);
    #endif
    printf("Running on core %d, redirecting stdout to Uart%d\n", pico_get_coreID(),DEBUG_UART);
}

void Interpret_line(LinBuffT *inbuf);

void DebugHandleInputChar ( unsigned char ch ) 
{
    /* Handle CRLF, Echo & flush*/
    // if ( ch == '\r' ) DEBUG_PUTC('\n');

    DEBUG_PUTC(ch);
    TaskNotify(TASK_LOG);

    /* Del = remove last character */
    if ( ch == 0x07f ) { 
      LBUF_DEL(i0);
    } else {
      /* store character, if not '\r' */
      if ( ch == '\r' ) { ch='\n'; }
      if ( ( ch == '\n' || ch == 0x04 ) ) { 
          /* CR or CTRl-D: interpret input */
          Interpret_line(&i0);
      } else {
          LinBuff_Putc(&i0, ch);
      }
    }
}

/*
 * Task handler for input characters
 */
void task_handle_com( uint32_t param)
{
  #define MAX_INBUF 4
  #define TIMEOUT_US  4
  // char inbuf[MAX_INBUF];
  char c;
  int rc,i;
  /* read input characters until there are no more */
  do {
    #if DEBUG_UART == 0
      rc = uart0_in_chars(&c, 1);
    #elif DEBUG_UART == 1
      rc = uart1_in_chars(&c, 1);
    #endif
    /* rc < 0 -> no more characters */
    if (rc < 0) break;
    DebugHandleInputChar((unsigned char)c);
  }while(1);
}


/*
 * Returns true, if input chars are available
 */
bool rx_chars_available(void)
{
  #if DEBUG_UART == 0
    return uart0_rxchars_available();
  #elif DEBUG_UART == 1
    return uart1_rxchars_available();
  #endif

}

void dbg_printf( const char* format, ... ) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end( args );
}


/*----------------------------------------------------------------------------*
 * logger functions
 *---------------------------------------------------------------------------*/


#if DEBUG_FEATURES > 0 && LOGTO_CONSOLE > 0
    #if DEBUG_DEBUGIO == 0
        /******************************************************************************
         * Write data of lenght <len> to LogFile, no CRLF appended 
         * true is returned on success,
         * false is returned on any failure
         *****************************************************************************/
        uint8_t Console_Write(const char *data, uint32_t len )
        {
            /* write ti buffer and start transfer, if full */
            if ( CircBuff_PutStr(&o0, (uint8_t *)data, len ) < len )
                TaskNotify(TASK_LOG);

            return true;
        }


        /******************************************************************************
         * Write CRLF and transfer to file
         *****************************************************************************/
        void Console_CRLF(void)
        {
            if ( bExpandCrToCrlf0 ) CircBuff_Put(&o0, '\r');
            CircBuff_Put(&o0, '\n');
            TaskNotify(TASK_LOG);
        }



        /******************************************************************************
         * Write NULL-terminated string to LogFile, append CRLF and start write to file 
         *****************************************************************************/
        uint8_t Console_Write_CRLF(const char *data, uint32_t len)
        {
            uint8_t ret = Console_Write(data, len);
            if ( ret ) Console_CRLF();
            return ret;

        }
     #else
        /******************************************************************************
         * Write data of lenght <len> to LogFile, no CRLF appended 
         * true is returned on success,
         * false is returned on any failure
         *****************************************************************************/
        uint8_t Console_Write(const char *data, uint32_t len )
        {
            for ( ;len > 0; len-- ) {
                DEBUG_PUTC(*(data++));
            }

            return true;
        }



        /******************************************************************************
         * Write NULL-terminated string to LogFile, append CRLF and start write to file 
         *****************************************************************************/
        uint8_t Console_Write_CRLF(const char *data, uint32_t len)
        {
            uint8_t ret = Console_Write(data, len);
            if ( ret ) Console_CRLF();
            return ret;

        }


        /******************************************************************************
         * Write CRLF and transfer to file
         *****************************************************************************/
        void Console_CRLF(void)
        {
            CRLF();
        }
     #endif
#endif



/**
  * @}
  */
