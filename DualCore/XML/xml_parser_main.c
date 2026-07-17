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
#include "system/util.h"
#include "dev/i2cx.h"
#include "parser_specs.h"
#include "xml_feeder.h"
#include "xml_parser_main.h"

/* size of Parser input circualr buffer, must be power of 2 */
#define PARSER_CIRCBUF_SIZE         256
#define PARSER_TOKEN_SIZE           32
/* underlying linear buffer of circular buffer */
static uint8_t buff[PARSER_CIRCBUF_SIZE];
static uint8_t tokenbuff[PARSER_TOKEN_SIZE];

/* Circular buffer for all parser input */
CircBuffT in;

/* Top of parser stack */
Parser_Stack_T *xml_parser_tos=NULL;

/* actual parser element */
Parser_Item_T actual;


/* Root may contain component(s) plus optional prolog */
const char *xml_root[]      = XML_ROOT_SET;

/* Component may contain styles, labels, arcs, NULL terminated array */
const char *xml_component[] = XML_COMPONENT_SET;

/******************************************************************************
 * @brief Push actual parser state to stack. Needed mem is allocated from heap
 * @param  src: ptr to parser state to be pushed
 * @retval new parser TOS or NULL when malloc failed
 * @note   global TOP pointer "xml_parser_tos" will be actualized, too
 *****************************************************************************/
Parser_Stack_T * xml_parser_push ( Parser_Item_T *src ) 
{
    Parser_Stack_T *new_tos = my_malloc(sizeof(Parser_Stack_T));
    if ( !new_tos ) {
      DEBUG_PRINTF("xml_parser_push: unable to alloc %d bytes\n",sizeof(Parser_Stack_T) );
      return NULL;
    }
    new_tos->next = xml_parser_tos;
    new_tos->parser_state = *src;

    return xml_parser_tos = new_tos;
}

/******************************************************************************
 * @brief Pops top element from xml parser stack and restores that state to dest
 * @param  dest: ptr to parser state to be restored from stack, or NULL
 *               then the element is popped w/o being restored
 * @retval false when stack is emtpy, tru in all other cases
 * @note   global TOP pointer "xml_parser_tos" will be actualized, too.
 *         heap mem is freed
 *****************************************************************************/
bool xml_parser_pop  ( Parser_Item_T *current, Parser_Item_T *dest )
{
  /* if we have an actual OnExit, then call it now */
  if (current && current->OnExit ) current->OnExit( current );

  if ( XML_STACK_EMPTY() ) return false;

  /* get TOS, copy it to dest, move TOS to nex, then delete old TOS */
  Parser_Stack_T *oldtos = xml_parser_tos;
  if (dest) *dest = oldtos->parser_state;
  xml_parser_tos = oldtos->next;
  my_free(oldtos);
  return true;
}

/******************************************************************************
 * @brief  set all field of global var Parser_Item_T actual
 *****************************************************************************/
void xml_set_actual( ParserFunc f, uint32_t st, const GUI_Edit_T *edit, const char *ew, const char *n )
{
    actual.pActual  = f;
    actual.state    = st;
    actual.edit     = edit;
    actual.exitword = ew;
    /* OnExit has to be set separately */
    actual.OnExit   = NULL;
#if DEBUG_PARSER > 0
    actual.name     = n;
#else
    UNUSED(n);
#endif
}

bool task_init_xml(void)
{
    CircBuff_Init(&in, PARSER_CIRCBUF_SIZE, buff);

    /* initialize and arm I2C interface */
    i2c_init_all();
    i2c_slave_init(i2c1, 0x24, &i2c_slave_handler);
    xml_parser_init();


  return true;
}

void xml_parse ( char *token, uint32_t tokenlength )
{
    #if DEBUG_PARSER > 1 
        DEBUG_PRINTF("XML-Parse elem:%s, state: %d, token: %s\n", actual.name, actual.state, token);
    #endif
  if (actual.pActual) {
    actual.pActual(token, tokenlength, actual.state);
  } else {
  #if DEBUG_PARSER > 0 
      DEBUG_PRINTF("Err: xml_parse: No parser set!\n");
  #endif
  }
}

void task_handle_xml  (uint32_t param)
{
  uint32_t tokenlength;
  UNUSED(param);
  while ( tokenlength = CircBuff_GetToken(&in, tokenbuff, PARSER_TOKEN_SIZE-1)) {
     /* append terminator, just for safety */
     tokenbuff[tokenlength]='\0';
    #if DEBUG_PARSER > 2 
        DEBUG_PRINTF("XML token: %s\n", tokenbuff );
    #endif
    xml_parse(tokenbuff, tokenlength);
  }
}
  /**
  * @}
  */

