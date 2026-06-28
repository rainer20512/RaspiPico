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

#define INBUFSIZE 32

char inbuf[INBUFSIZE];
uint32_t iptr;

#define TERMINATION_CHAR  0x04
#define INBUF_INIT()      iptr=0
#define INBUF_FULL()	  (iptr >= INBUFSIZE)
#define INBUF_EMPTY()	  (iptr == 0)

void inbuf_flush(void)
{
    if ( INBUF_EMPTY() ) return;
    uint32_t written = CircBuff_PutStr(&in, inbuf, iptr);
    if ( written < iptr ) {
      DEBUG_PRINTF("Parser input overrun, %d chars lost\n", iptr-written);
    } else {
      DEBUG_PRINTF("Parser input: wrote %d\n", written);
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
        i2c_write_byte_raw(i2c, 42);
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

