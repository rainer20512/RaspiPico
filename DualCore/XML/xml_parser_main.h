
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
#include "../../GUI/gui_editdef.h"

struct Parser_Item;

typedef bool ( *ParserFunc  ) (char *token, uint32_t tokenlen, uint32_t state );
typedef void ( *ParserOnPop ) ( struct Parser_Item *toPop);

/******************************************************************************
 * Structure to store the actual parser state , ie
 * - actual parser function
 *****************************************************************************/
typedef struct Parser_Item{
  ParserFunc pActual;     /* actual parsing function */
  uint32_t state;         /* optional state within that function */
  const GUI_Edit_T *edit; /* edit receipe for actual item, or NULL if no props */
  const char *exitword;   /* optional exit word to exit the actual parse level */
  ParserOnPop OnExit;     /* callback fn _before_ item is popped from stack    */
#if DEBUG_PARSER > 0
  const char *name;       /* user friendly name of that parsing element */
#endif 
} Parser_Item_T;

/******************************************************************************
 * Structure to build a parser stack, ie a stack of open parser functions
 *****************************************************************************/
typedef struct Parser_Stack {
  Parser_Item_T parser_state;
  struct Parser_Stack *next;
} Parser_Stack_T;

Parser_Stack_T *xml_parser_push ( Parser_Item_T *src ); 
bool            xml_parser_pop  (Parser_Item_T *current,  Parser_Item_T *dest );
#define XML_STACK_EMPTY() (xml_parser_tos==NULL)

void xml_parser_init  (void);
void xml_parse        ( char *token, uint32_t tokenlength );
void xml_set_actual   ( ParserFunc f, uint32_t st, const GUI_Edit_T *edit, const char *ew, const char *n );
#if DEBUG_PARSER > 0
    #define SET_ACTUAL(f,st,ed,ex,n)  xml_set_actual(f,st,ed,ex,n)
#else
    #define SET_ACTUAL(f,st,ed,ex,n)  xml_set_actual(f,st,ed,ex,NULL)
#endif

bool task_init_xml    (void); 
void task_handle_xml  (uint32_t param);

/* Circular buffer for all parser input */
extern CircBuffT in;

/* Parser Stack */
extern Parser_Stack_T *xml_parser_tos;

/* actual parser element */
extern Parser_Item_T actual;

#endif /* _XML_PARSER_MAIN_H_  */