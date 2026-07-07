/**
  ******************************************************************************
  * @file    xml_feeder.c
  * @author  Rainer 
  * @brief   functions to feed input into XML parser
  *          We first collect INBUFSIZE chars in temporary buffer.
  *          When full or <EOF> is received, content will be copied to
  *          parser input circular buffer and parser will be notified
    ******************************************************************************
  *
  ******************************************************************************
  * @addtogroup XML
  * @{
  */

#include "config/config.h"

#include "debug/debug_helper.h"
#include "task/minitask.h"
#include "xml_parser_main.h"
#include "xml_feeder.h"

#define INBUFSIZE     64
#define OUTBUFSIZE    127     /* first char is used to store len */
const char test[]     = "01234567891023456789";
//const char test[]     = "012345678910234567892023456789302345678940234567895023456789";
char inbuf[INBUFSIZE];
uint8_t i2coutbuf[OUTBUFSIZE+1] = {0};

uint32_t iptr=0;
uint8_t  oi2c=0;              /* output ptr for i2c routines */
uint8_t *optr =   i2coutbuf;
uint8_t *outbuf = i2coutbuf+1;

#define TERMINATION_CHAR  0x04
#define INBUF_INIT()      iptr=0
#define INBUF_FULL()	  (iptr >= INBUFSIZE)
#define INBUF_EMPTY()	  (iptr == 0)

#define OUTBUF_INIT()     *optr=0;oi2c=0
#define OUTBUF_FULL()	  (*optr >= OUTBUFSIZE)
#define OUTBUF_EMPTY()	  (*optr == 0)
#define OUTBUF_FREE()     (OUTBUFSIZE-*optr)

void inbuf_flush(void)
{
    if ( INBUF_EMPTY() ) return;
    uint32_t written = CircBuff_PutStr(&in, inbuf, iptr);
    if ( written < iptr ) {
      DEBUG_PRINTF("Parser input overrun, %d chars lost\n", iptr-written);
    } else {
      #if DEBUG_PARSER > 1 
          DEBUG_PRINTF("Parser input: wrote %d\n", written);
      #endif
    }
    INBUF_INIT();
    TaskNotify(TASK_XML);
}

void inbuf_put(char c)
{
		/* caller must ensure, there is space left in sendbuf */
		inbuf[iptr++] = c;
        if ( INBUF_FULL() ) inbuf_flush();

        if ( c == TERMINATION_CHAR ) inbuf_flush();
}

void outbuf_putch(char c)
{
  if ( OUTBUF_FULL() ) return;
  outbuf[(*optr)++]=c;
}

void output_putsl( const char *in, uint8_t len )
{
  uint8_t maxlen = ( len > OUTBUF_FREE() ? OUTBUF_FREE() : len );
  memmove(outbuf+*optr, in, maxlen);
  *optr +=maxlen;
}

void output_puts(const char *in)
{
  output_putsl(in, strlen(in) );
}

void i2c_out(i2c_inst_t *i2c)
{
    size_t maxout;          /* actual i2c buffer free space */
    uint8_t actsize;        /* number of write bytes pending to transfer */
    uint8_t cursize;       /* size of actual i2c transfer */
    char c;

    /* when we start, write length byte only */
    if ( oi2c == 0 ) {
        c = i2coutbuf[oi2c++];
        i2c_write_byte_raw(i2c, c);
        //DEBUG_PUTC('S');print_hexXX(c);
    } else {
        actsize = *optr - oi2c+1;
        while ( actsize ) {
            maxout  = i2c_get_write_available (i2c); 
            cursize = MIN(maxout, actsize);
            for ( uint32_t i = 0; i < cursize ; i++ ) {
                c = i2coutbuf[oi2c++];
                i2c_write_byte_raw(i2c, c);
                // DEBUG_PUTC('D');print_hexXX(c);
            }
            actsize-=cursize;
        }
        OUTBUF_INIT();   
        output_puts(test);
    }
}

void i2c_feeder_init()
{
  INBUF_INIT();
  OUTBUF_INIT();
  output_puts(test);
}

/******************************************************************************
 * callback for IC2 transactions, will be executed in interrupt context, so
 * keep it smart
 *****************************************************************************/
void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
    switch (event) {
    case I2C_SLAVE_RECEIVE: // master has written some data
        inbuf_put(i2c_read_byte_raw(i2c));
        break;
    case I2C_SLAVE_REQUEST: // master is requesting data
        i2c_out(i2c);
        break;
    case I2C_SLAVE_FINISH: // master has signalled Stop / Restart
        break;
    default:
        break;
    }
}
  /**
  * @}
  */

