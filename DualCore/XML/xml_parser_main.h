
/**
  ******************************************************************************
  * @file    xml_parser_main.h
  * @author  Rainer
  * @brief   XML parser toplevel functions / variables
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _XML_PARSER_MAIN_H_
#define _XML_PARSER_MAIN_H_
#include "config/config.h"
#include "system/circbuf.h"

/* Circular buffer for all parser input */
extern CircBuffT in;

bool task_init_xml    (void); 
void task_handle_xml  (uint32_t param);

#endif /* _XML_PARSER_MAIN_H_  */