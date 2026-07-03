
/**
  ******************************************************************************
  * @file    parser_specs.h
  * @author  Rainer
  * @brief   Specification / rules for the XML parser
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PARSER_SPECS_H_
#define _PARSER_SPECS_H_

#define OPENBRACKET     '<'
#define CLOSEBRACKET    '>'
#define ESCAPEPROLOG    '?'
#define ESCAPESLASH     '/'

/* Name of the root element, only one element plus optional prolog */
const char *xml_root[]      = {"component", "?xml", NULL };
/* Component may contain styles, labels, arcs */
const char *xml_component[] = {"style", "label", "arc", NULL };

/* Component may contain styles, labels, arcs */
const char *xml_style[] = {"name", "width", "height", NULL };


#endif /* _PARSER_SPECS_H_  */