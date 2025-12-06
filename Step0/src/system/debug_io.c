/**
  ******************************************************************************
  * @file    debug_io.c
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
// #include "debug_io.h"

#include "system/circbuf.h"
#include "system/util.h"

#include "pico/stdio.h"
#include "pico/time.h"


/* Private define ------------------------------------------------------------*/

/* size of RX/TX buffers, sizes must be power of two  */
#define OUTBUF_SIZE 2048   
#define INBUF_SIZE  64

/* Private variables ---------------------------------------------------------*/
static uint8_t bOngoingTransfer = 0;    /* indicates an ongoing transfer, if != 0 */
static uint8_t bExpandCrToCrlf = 0;     /* expand LF to CRLF */
static uint8_t bDelayedFlush = 0;       /* Keep in mind an delayed flush */

/**** 003 **** 
 * Whenever BDMA is used ( here: for LPUART1 ), the DMA memory hasto reside
 * in SRAM4. 
 */
static CircBuffT o;
static LinBuffT i; 

/* RHB tbd */
#define DMAMEM

static DMAMEM uint8_t outbuf[OUTBUF_SIZE];
static DMAMEM uint8_t inbuf[INBUF_SIZE];

/* Private macro -------------------------------------------------------------*/

/* forward declarations  -----------------------------------------------------*/
void DebugOutputCompleteCB ( uint32_t size );

/* Private functions  ---------------------------------------------------------*/


static void UsartStartTx(uint8_t *data, uint32_t txSize)
{
  for ( uint32_t i=0; i< txSize; i++ ) {
    stdio_putchar(*(data++));
  }
  DebugOutputCompleteCB ( txSize );
}


/* Start transfer to output device */
static void Debug_OutputTransfer(uint32_t from, uint32_t to )
{
  /* Make sure, there is no other active transfer */
  assert_param(!bOngoingTransfer);
  /* Make sure, there is no wraparound between 'from' and 'to'  */
  assert_param(from <= to);

  bOngoingTransfer = 1;
  uint32_t transfer_size = to - from;

  UsartStartTx(o.buf+o.rdptr, transfer_size);

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
    store_hexXXXX(o.rdptr);
    store_chr('/');
    store_hexXXXX(o.wrptr);
    store_chr(' ');
  }
#endif


/*
 * Callback when transfer complete *
 * - reset ongoing-transfer-flag
 * - increment txptr
 * - check for wraparound and continue transfer in case of wraparound 
 */
void DebugOutputCompleteCB ( uint32_t size )
{
  bOngoingTransfer = 0;
  o.rdptr = CBUFPTR_INCR(o, rdptr, size);

  /* If we kept in mind an delayed flush, initiate it now */
  if ( bDelayedFlush ) {
    bDelayedFlush = 0;
    TaskNotify(TASK_LOG);
  }

}


/* Copy the content of the output buffer to output device */
void task_handle_out(uint32_t arg)
{
    UNUSED(arg);
   
   /* Don't flush when empty */
   if ( CBUF_EMPTY(o) ) return;

   /* Check for ongoing transfer. If so, keep in mind to flush afterwards */ 
   if ( bOngoingTransfer ) {
     bDelayedFlush = 1;
     return;
   }

  /*
   * If the buffer content wraps around, only transfer up to the end of the circular buffer
   * and keep in mind to transfer from begin later
   */
  if ( CBUF_WRAPAROUND(o) ) {
     bDelayedFlush = 1;
     Debug_OutputTransfer(o.rdptr, OUTBUF_SIZE );
   } else {
     Debug_OutputTransfer(o.rdptr, o.wrptr );
   }
}

/*
 * redefine system function __putchar so that standard io functions can be used to produce debug output
 */
/* int __putchar(int ch ) { */
int __putchar(int ch, __printf_tag_ptr uu) {
  // if ( bExpandCrToCrlf && (char)ch == '\n' ) __putchar('\r', uu);
  // if ( bExpandCrToCrlf && (char)ch == '\n' ) __putchar('\r'); */

  /*
   * - if there is room left in outbuf, copy character to outbuf and start transfer, if character is '\n'
   * - otherwise replace the last written character with * to indicate overflow and start transfer
   */

  if ( !CircBuff_Put(&o, ch ) ) {
      TaskNotify(TASK_LOG);
  } else { 
    if ( ch == '\n') {
      TaskNotify(TASK_LOG);
    }
  }
     
  return ch;
}


/**
* @brief  Callback when input character(s) available
* @param  UartCOMx: UART handle
* @retval None
*/
void Debug_RxCharAvailCB(void *param) 
{
    TaskNotify(TASK_COM);
}


bool task_init_io(void) 
{
    CircBuff_Init       (&o, OUTBUF_SIZE, outbuf);
    LinBuff_Init        (&i, INBUF_SIZE,  inbuf );
    stdio_set_chars_available_callback(Debug_RxCharAvailCB, NULL);

    stdio_printf("Redirecting stdout to UART\n");
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
      LBUF_DEL(i);
    } else {
      /* store character, if not '\r' */
      if ( ch == '\r' ) { ch='\n'; }
      if ( ( ch == '\n' || ch == 0x04 ) ) { 
          /* CR or CTRl-D: interpret input */
          Interpret_line(&i);
      } else {
          LinBuff_Putc(&i, ch);
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
  char inbuf[MAX_INBUF];
  int rc,i;
  /* read input characters until there are no more */
  do {
    rc = stdio_getchar_timeout_us(0);
    /* rc < 0 -> no more characters */
    if (rc < 0) break;
    DebugHandleInputChar((unsigned char)rc);
  }while(1);
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
            if ( CircBuff_PutStr(&o, (uint8_t *)data, len ) < len )
                TaskNotify(TASK_LOG);

            return true;
        }


        /******************************************************************************
         * Write CRLF and transfer to file
         *****************************************************************************/
        void Console_CRLF(void)
        {
            if ( bExpandCrToCrlf ) CircBuff_Put(&o, '\r');
            CircBuff_Put(&o, '\n');
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
