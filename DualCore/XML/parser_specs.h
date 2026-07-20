
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

#define OPENBRACKET           '<'
#define CLOSEBRACKET          '>'
#define ESCAPEPROLOG          '?'
#define ESCAPESLASH           '/'

/****************************************************************************** 
 * the follwoing names are used to identify valid gui elements in XML parser,
 * in edit-receipes and GUI edit. as XML contains LC only, these IDs have to
 * be LC, too. Be sure to only use this constants onyl when referring to GUI
 * elements by receipes name
 *****************************************************************************/ 
#define SCREEN_IDSTR          "screen"
#define STYLE_IDSTR           "style"
#define LABEL_IDSTR           "label"
#define ARC_IDSTR             "arc"
#define SCALE_IDSTR           "scale"
#define IMAGE_IDSTR           "image"

/****************************************************************************** 
 * Specify max length of a property or element name in XML
 * has an impact on RAM useage
 *****************************************************************************/ 
#define ID_MAXNAMELEN         16

#define XML_PARSER_VERSION    "1.0.1"

/* Root may contain component(s) plus optional prolog, NULL terminated array */
extern const char *xml_root[];
#define XML_ROOT_SET          {"screen", "?xml", NULL }

/* Component may contain styles, labels, arcs, NULL terminated array */
extern const char *xml_component[];
#define XML_COMPONENT_SET  { STYLE_IDSTR, LABEL_IDSTR, ARC_IDSTR, SCALE_IDSTR, IMAGE_IDSTR, NULL }

#endif /* _PARSER_SPECS_H_  */