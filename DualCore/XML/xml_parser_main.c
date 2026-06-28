/**
  ******************************************************************************
  * @file    xml_parser_main.c
  * @author  Rainer 
  * @brief   XML parser toplevel functions / variables
  ******************************************************************************
  *
  ******************************************************************************
  * @addtogroup XML
  * @{
  */

#include "config/config.h"
#include "debug/debug_helper.h"
#include "system/circbuf.h"
#include "dev/i2cx.h"
#include "xml_feeder.h"

/* size of Parser input circualr buffer, must be power of 2 */
#define PARSER_CIRCBUF_SIZE         128
#define PARSER_TOKEN_SIZE           32
/* underlying linear buffer of circular buffer */
static uint8_t buff[PARSER_CIRCBUF_SIZE];
static uint8_t tokenbuff[PARSER_TOKEN_SIZE];

/* Circular buffer for all parser input */
CircBuffT in;


bool task_init_xml(void)
{
    CircBuff_Init(&in, PARSER_CIRCBUF_SIZE, buff);

    /* initialize and arm I2C interface */
    i2c_init_all();
    i2c_slave_init(i2c1, 0x24, &i2c_slave_handler);


  return true;
}

void task_handle_xml  (uint32_t param)
{
  uint32_t tokenlength;
  UNUSED(param);
  while ( tokenlength = CircBuff_GetToken(&in, tokenbuff, PARSER_TOKEN_SIZE-1)) {
     tokenbuff[tokenlength]='\0';
     DEBUG_PRINTF("XML token: %s\n", tokenbuff );
  }
}
  /**
  * @}
  */

