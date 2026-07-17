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
#define OUTBUFSIZE    128    

#define RXFLAG_PARSER_RESET		0x10  	/* Tell I2C slave to reset its parser */
#define RXFLAG_QRY_STATUS    	0x11  	/* Tell i2C slave to send its status in next i2c slave read */
#define RXFLAG_RAW_DATA      	0x55  	/* Tell i2c slave, that package contains raw data */
#define WRAPPER_EXCESS_BYTES	2       /* number of bytes added by length and flag Bytes */

/* Answer flag bits for "query status"  */
#define I2C_SLAVEFLAG_BUSY      (1<<0)	/* i2c slave is busy                  */
#define I2C_SLAVEFLAG_HASDATA   (1<<1)	/* i2c slave has additional raw data  */	

const char test[]     = "01234567891023456789";
//const char test[]     = "012345678910234567892023456789302345678940234567895023456789";

typedef struct{
   uint8_t inlen;                   /* total size of packet, length and flags byte excluded! */
   uint8_t inflags;                 /* flag bytes */
   uint8_t inbytes[INBUFSIZE];      /* raw transfer data */
} I2C_In_WrapperT;
 
typedef struct{
   uint8_t outlen;					/* total size of packet, length and flags byte excluded! */
   uint8_t outflags;				/* flag bytes */
   uint8_t outbytes[OUTBUFSIZE];	/* raw transfer data */
} I2C_Out_WrapperT;

static I2C_In_WrapperT  i2c_in;                 /* Buffer for incoming i2c Data    */
static I2C_Out_WrapperT i2c_out;                /* buffer for outgoing i2c data    */
static uint8_t *inbuf_raw=(uint8_t *)&i2c_in;   /* pointer to i2c_in raw data      */
static uint8_t *outbuf_raw=(uint8_t *)&i2c_out; /* pointer to i2c_out raw data     */

static uint32_t iptr=0;
static uint8_t  oi2c=0;              /* output ptr for i2c routines      */
static uint8_t orawlen;              /* raw i2c output length            */
static uint8_t optr;                 /* write offset in i2c_out.outbytes */ 

#define TERMINATION_CHAR  0x04
#define INBUF_INIT()      iptr=0
#define INBUF_FULL()	  (iptr >= INBUFSIZE + WRAPPER_EXCESS_BYTES)
#define INBUF_COMPLETE()  (iptr == i2c_in.inlen + WRAPPER_EXCESS_BYTES) 
#define INBUF_EMPTY()	  (iptr == 0)

#define OUTBUF_INIT()     optr=oi2c=orawlen=0;
#define OUTBUF_FULL()	  (optr >= OUTBUFSIZE)
#define OUTBUF_EMPTY()	  (optr == 0)
#define OUTBUF_FREE()     (OUTBUFSIZE-optr)

/******************************************************************************
 * @brief master is requesting slave status: Assemble it and prepare for
 *        i2c slave read 
 * @note  executed in interrupt context
 *****************************************************************************/
void send_slave_flags(void)
{
    i2c_out.outlen = i2c_out.outflags = 0;
    if ( CircBuff_PeekToken(&in) ) i2c_out.outflags |= I2C_SLAVEFLAG_BUSY;
    // RHB todo if ( !OUTBUF_EMPTY() )    i2c_out.outflags |= I2C_SLAVEFLAG_HASDATA;
}

/******************************************************************************
 * @brief Got i2c dat with raw data in it. copy to circular input buffer
 *        and notify parser
 * @note  executed in interrupt context
 *****************************************************************************/
void inbuf_flush(void)
{
    if ( INBUF_EMPTY() ) return;
    uint32_t written = CircBuff_PutStr(&in, i2c_in.inbytes, i2c_in.inlen);
    if ( written < i2c_in.inlen ) {
      DEBUG_PRINTF("Parser input overrun, %d chars lost\n", iptr-written);
    } else {
      #if DEBUG_PARSER > 1 
          DEBUG_PRINTF("Parser input: wrote %d\n", written);
      #endif
    }
    TaskNotify(TASK_XML);
}

/******************************************************************************
 * @brief Input sequence is complete, handle it by examinating flag bits
 * @note  executed in interrupt context
 *****************************************************************************/
void handle_inbuf(void)
{
    // DEBUG_PRINTF("HI=x%02x",i2c_in.inflags);
    switch ( i2c_in.inflags ) {
      case RXFLAG_PARSER_RESET: 
        i2c_feeder_init();
        break;
      case RXFLAG_QRY_STATUS:
        send_slave_flags();
        break;
      case RXFLAG_RAW_DATA:
        inbuf_flush();
        break;
      default:
          DEBUG_PRINTF("handle_inbuf: unknown packet type 0x%02x\n", i2c_in.inflags);
    } /* switch */
    INBUF_INIT();
}

/******************************************************************************
 * @brief Got one i2c input character, store it and check for sequence complete
 * @note  executed in interrupt context
 *****************************************************************************/
void inbuf_put(char c)
{
    /* caller must ensure, there is space left in sendbuf */
    inbuf_raw[iptr++] = c;
    if ( INBUF_COMPLETE() || INBUF_FULL() ) handle_inbuf();
    if ( c == TERMINATION_CHAR )            handle_inbuf();
}

void outbuf_putch(char c)
{
  if ( OUTBUF_FULL() ) return;
  i2c_out.outbytes[optr++]=c;
}

void output_putsl( const char *in, uint8_t len )
{
  uint8_t maxlen = ( len > OUTBUF_FREE() ? OUTBUF_FREE() : len );
  memmove(i2c_out.outbytes+optr, in, maxlen);
  optr +=maxlen;
}

void output_puts(const char *in)
{
  output_putsl(in, strlen(in) );
}

void i2c_send(i2c_inst_t *i2c)
{
    size_t maxout;          /* actual i2c buffer free space */
    uint8_t actsize;        /* number of write bytes pending to transfer */
    uint8_t cursize;       /* size of actual i2c transfer */
    char c;

    /* when we start, store string data len to length field and determine total transfer len initially */
    if ( orawlen == 0 ) {
        i2c_out.outlen = optr;
        orawlen = i2c_out.outlen+WRAPPER_EXCESS_BYTES;
        #if DEBUG_PARSER > 1
          DEBUG_PRINTF("Prepared %d bytes to send\n",orawlen);
        #endif
   }

    actsize = orawlen - oi2c;
    while ( actsize ) {
        maxout  = i2c_get_write_available (i2c); 
        cursize = MIN(maxout, actsize);
        for ( uint32_t i = 0; i < cursize ; i++ ) {
            c = outbuf_raw[oi2c++];
            i2c_write_byte_raw(i2c, c);
            // DEBUG_PUTC('D');print_hexXX(c);
        }
        actsize-=cursize;
    }
    OUTBUF_INIT();   
}

void i2c_feeder_init()
{
  INBUF_INIT();
  CircBuff_SetEmpty(&in);
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
        i2c_send(i2c);
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

