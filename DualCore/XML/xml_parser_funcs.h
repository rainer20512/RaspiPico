
/**
  ******************************************************************************
  * @file    xml_parser_funcs.h
  * @author  Rainer
  * @brief   functions to parse specific XML elements
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _XML_PARSER_FUNCS_H_
#define _XML_PARSER_FUNCS_H_
#include "config/config.h"

#include "xml_parser_main.h"
void        DumpToken         (const char* token, uint32_t tokenlength); 
void        xml_parser_init   (void);
const char *xml_parser_version(void);

#endif /* _XML_PARSER_FUNCS_H */