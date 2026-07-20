/**
  ******************************************************************************
  * @file    xml_parser_funcs.c
  * @author  Rainer 
  * @brief   functions to parse specific XML elements
  ******************************************************************************
  *
  ******************************************************************************
  * @addtogroup XML
  * @{
  */
#include <ctype.h>
#include "config/config.h"
#include <system/util.h>
#include "debug/debug_helper.h"
#include "parser_specs.h"
#include "xml_parser_main.h"
#include "xml_feeder.h"
#include "../../GUI/gui_edit.h"
#include "../../GUI/gui_ops.h"


#if 0
#define FLAG_CLOSEBRACKET       (1 <<  0)    /* Flag for "starttoken had '>'" at end */

#define SET_FLAG(f)             actual.flags |= f;
#define CLR_FLAG(f)             actual.flags &= ~(f)
#define IS_FLAG_SET(f)          (actual.flags & f)
#endif

#define ATTR_MAXVALUELEN        64


/* Helper structure to store one name/value pair when parsing attribute lists */
typedef struct {
  char name [ID_MAXNAMELEN];
  char value[ATTR_MAXVALUELEN];
  uint8_t namelen;
  uint8_t valuelen;
} Parser_Attr_T;

/* global variable for one /name/value pair, will be allocated by xml_parser_init */
Parser_Attr_T *act_attr = NULL;

/* forward declarations ******************************************************/
bool parse_prolog       ( char *token, uint32_t tokenlength, uint32_t state );
bool parse_screen    ( char *token, uint32_t tokenlength, uint32_t state );
bool parse_lvgl_elem    ( char *token, uint32_t tokenlength, uint32_t state );
bool parse_attributes   ( char *token, uint32_t tokenlength, uint32_t state );

/* item parse exit callbacks */
void exit_lvgl_elem     ( Parser_Item_T *toPop);
/******************************************************************************
 * @brief print a vector with length len
 *        quotation marks are handeled as every other character
 * @retval index of character in token [beginning with 0] or -1 if not found
 *****************************************************************************/
void DumpToken(const char* token, uint32_t tokenlength) 
{
    for( ; tokenlength--; token++) {
        DEBUG_PUTC(*token);
    }
}



/******************************************************************************
 * @brief copy either name or value field into act_attr
 * src, srclen : source vector and length
 * option      : 1 = copy to name field; 2= copy to value field
 * @retval token
 *****************************************************************************/
 void AttrCopy(uint8_t *src, uint32_t srclen, uint32_t option )
 {
    uint32_t  copylen = MIN(srclen, option==1 ? ID_MAXNAMELEN : ATTR_MAXVALUELEN);
    uint8_t *lenptr = ( option == 1 ? &act_attr->namelen : &act_attr->valuelen );
    if ( copylen < srclen ) {
        #if DEBUG_PARSER > 0
            DEBUG_PRINTF("AttrCopy: %s truncated\n", option==1 ? "Name" : "Value" );
        #endif 
    }
  
    memmove( ( option == 1 ? act_attr->name : act_attr->value ), src, copylen );
    *lenptr = copylen;
    #if DEBUG_PARSER > 1
        if ( option == 2 )
        DEBUG_PRINTF("AttrCopy:"); DumpToken(act_attr->name, act_attr->namelen);
        DEBUG_PUTC('='); DumpToken(act_attr->value, act_attr->valuelen);
        DEBUG_PUTC('\n'); 
    #endif 

}

/******************************************************************************
 * @brief Update the GUI element in "edit" with the name/value combination
 *        in act_attr
 * @param edit  - GUI element description and workspace to use
 * @param act_attr modul global variable with name/value
 * @retval token
 *****************************************************************************/
void AttrUpdate( const GUI_Edit_T *edit )
{
    if (!edit) {
        #if DEBUG_PARSER > 0
            DEBUG_PRINTF("Err: AttrUpdate: No Edit data!!");
        #endif
        return;
    }

    int32_t idx = GetReceipeIdxByName( edit, act_attr->name, act_attr->namelen);
    if ( idx < 0 ) {
        output_puts("Unknown Property Name: >");
        output_putsl(act_attr->name, act_attr->namelen);
        output_putch('<');output_putch('\n');
        #if DEBUG_PARSER > 0
            DEBUG_PRINTF("Unknown Property Name: >");
            DumpToken(act_attr->name, act_attr->namelen);
            DEBUG_PUTC('<');DEBUG_PUTC('\n');
        #endif
        return;
    }

    if( !GUI_Edit_SetItem(act_attr->value, act_attr->valuelen, edit, (uint32_t)idx ) ) {
        #if DEBUG_PARSER > 0
            DEBUG_PRINTF("Invalid value >");
            DumpToken(act_attr->value, act_attr->valuelen);
            DEBUG_PRINTF("< for property >");
            DumpToken(act_attr->name, act_attr->namelen);
            DEBUG_PUTC('<');DEBUG_PUTC('\n');
        #endif
        output_puts("Invalid value >");
        output_putsl(act_attr->value, act_attr->valuelen);
        output_puts("< for property >");
        output_putsl(act_attr->name, act_attr->namelen);
        output_putch('<');output_putch('\n');
    }
}

/******************************************************************************
 * @brief convert whole token to lowercase
 *        Text within quotes is not converted
 * @retval token
 *****************************************************************************/
static char* TokenToLower(char* token, uint32_t tokenlength) 
{
  uint32_t qm_count;  /* number of found quotation marks */
  #define OUTSIDE_QUOTES()  (( qm_count & 1) == 0 )

    for(char *p=token; tokenlength--; p++) {
        if ( *p == '"' ) qm_count++;
        if (OUTSIDE_QUOTES()) *p=tolower(*p);
    }
    return token;
}
/******************************************************************************
 * @brief find <nth> occurence of <c> in token.
 * @param bIgnoreQuotes if bIgnoreQuotes=false, then Text within quotes is skipped
 * @retval index of character in token [beginning with 0] or -1 if not found
 *****************************************************************************/
static int32_t FindInToken(char* token, uint32_t tokenlength, const char c, uint32_t nth, bool bIgnoreQuotes) 
{
    uint32_t qm_count;  /* number of found quotation marks */
    #define OUTSIDE_QUOTES()  (( qm_count & 1) == 0 )

    /* nth has to be > 0 ! */
    if ( !nth ) return -1;

    for(int32_t i=0; tokenlength--; i++, token++) {
        if ( *(token) == '"' ) qm_count++;
        if (bIgnoreQuotes || OUTSIDE_QUOTES()) {
            if ( *token == c ) 
              if ( !--nth  ) return i;
        }
    }
    return -1;
}


/******************************************************************************
 * @brief check last char of token for closing bracket >
 *        and one character before for being "escapechar"
 *        if found, remove from token and set closebracket flag in actual
 *        if not found, return false
 * @retval token
 *****************************************************************************/
static bool CheckEscapedCloseBracket(char* token, uint32_t *tokenlength, const char escapechar) 
{
  if ( *tokenlength > 1 && *(token+*tokenlength-1) == CLOSEBRACKET && *(token+*tokenlength-2) == escapechar ) {
      *tokenlength-=2;
      return true;
  } 
  return false;
}

/******************************************************************************
 * @brief check last char of token for closing bracket >
 *        if found, remove from token and set closebracket flag in actual
 *        if not found, return false
 * @retval token
 *****************************************************************************/
static bool CheckCloseBracket(char* token, uint32_t *tokenlength) {
  if ( *(token+*tokenlength-1) == CLOSEBRACKET ) {
      (*tokenlength)--;
      // not used SET_FLAG(FLAG_CLOSEBRACKET) 
      return true;
  } 
  return false;
}

/******************************************************************************
 * @brief Check whether token has an exact match in "alternatives"
 *
 * @param  token        - actual input token
 * @param  tokenlength  - length of token
 * @param  alternatives - NULL terminated array of possible strings
 * @param  match        - the matching word is stored here if not NULL
 * @retval number of matching alternative, stating with 1 or 0 for "not found"
 * @note   if match is found, the matching word is stored to match
 *****************************************************************************/
static uint32_t CheckWord(char* token, uint32_t tokenlength, const char *alternatives[], const char **match) 
{
    for ( uint32_t i = 0; alternatives[i]; i++ ) {
       if ( strncmp(token, alternatives[i], tokenlength) == 0 ) {
          /* store matching word */
          if (match) *match=alternatives[i];
          return i+1;
      }
    }

    return 0;
}
/******************************************************************************
 * @brief Check whether token matches one specific word with or w/o '/'
 * @param  token        - actual input token
 * @param  tokenlength  - length of token
 * @param  oneword     - NULL terminated word to find in LC
 * @param  bEscaped     - if true, check for '/' at the beginning
 * @retval true if found and false if not matching
 * @note   this routine expects the closing bracket being checked and removed
 *         before
 *****************************************************************************/
static bool CheckOneWord(char* token, uint32_t tokenlength, const char *oneword)
{
  return  strncmp(token, oneword, tokenlength) == 0;
}

/******************************************************************************
 * @brief  Check whether token matches one specific word 
 * @       escaped with cEscape
 * @param  token        - actual input token
 * @param  tokenlength  - length of token
 * @param  oneword      - NULL terminated word to find in LC
 * @param  cEscape      - escape character, usually '/' or '?'
 * @retval true if found and false if not matching
 *****************************************************************************/
static uint32_t CheckExitWord(char* token, uint32_t tokenlength, const char *exitword, const char cEscape)
{
    /* check for escape char first */
    if (*token != cEscape ) return false; 
    ++token;--tokenlength;

    /* check for exitword */
    return  CheckOneWord(token, tokenlength, exitword);
}

bool ExitLevel(void)
{
    if ( XML_STACK_EMPTY() ) {
        #if DEBUG_PARSER > 0
            DEBUG_PRINTF("xml_parse: parse finished at top level\n");
        #endif 
        return false;
    } 
    xml_parser_pop(&actual, &actual);
    return true;
}

/******************************************************************************
 * @brief OnExit function when screen attributes have been successfully parsed:
 *        Immediately set the screen parameters to the specified values
 * @param  toPop - current parser stack that will be popped after OnExit call
 *****************************************************************************/
void exit_screen ( Parser_Item_T *toPop)
{
#if DEBUG_PARSER > 0
  DEBUG_PRINTF("ExitScreen of %s:\n", toPop->name);
#endif
  const GUI_Edit_T *edit = toPop->edit;
  if (!edit)  {
      #if DEBUG_PARSER > 0
          DEBUG_PRINTF("No edit\n");
      #endif
  } else {
      GUI_edit_dump_all(edit, true );
      // RHB ToDo 
     GUI_new_or_update_entry_Core0(edit->workspace,  edit->gui_elem_type );
  }
}


/******************************************************************************
 * @brief OnExit function when lvgl elem has been successfully parsed:
 *        Create or Update the corresponding GUI element
 * @param  toPop - current parser stack that will be popped after OnExit call
 *****************************************************************************/
void exit_lvgl_elem ( Parser_Item_T *toPop)
{
#if DEBUG_PARSER > 0
  DEBUG_PRINTF("Exit LVGL-Elem of %s:\n", toPop->name);
#endif
  const GUI_Edit_T *edit = toPop->edit;
  if (!edit)  {
      #if DEBUG_PARSER > 0
          DEBUG_PRINTF("No edit\n");
      #endif
  } else {
      // GUI_edit_dump_all(edit, true );
      GUI_new_or_update_entry_Core0(edit->workspace, edit->gui_elem_type );

  }
}
/******************************************************************************
 * @brief parse lvgl element
 * @param  token       - actual input token
 * @param  tokenlength - length of token
 * @retval 
 *****************************************************************************/
bool parse_lvgl_elem ( char *token, uint32_t tokenlength, uint32_t state )
{
    const char *last_exitword;

    /* need at least 2 chars */
    if ( tokenlength < 1 ) return true;

    TokenToLower(token, tokenlength);
    /* Must have an opening bracket first */
    if ( *token != OPENBRACKET ) return true;

    /* In the following we assume, we have an '<' first */
    uint32_t reducedlength = tokenlength-1; 
    if (CheckCloseBracket(token+1, &reducedlength)) {
        /* when a closing bracket is found, we check for exitword, if set */
        if ( actual.exitword && CheckExitWord(token+1, reducedlength, actual.exitword, ESCAPESLASH ) )  {
            return ExitLevel();
        } else {
          /* found a simple closing bracket, so we have found the initital element, continue parsing */
          return true;
        }

    } else {
        /* no exit word, no closing brackets */
        /* now check for elementame w/o closing brackets, that means: parse attribut list for that element */
        if ( CheckOneWord(token+1, reducedlength, actual.exitword) ) {
            /* forward edit receipe, keep actual state, push it and start parsing attributes */
            const GUI_Edit_T *keep = actual.edit;
            xml_parser_push(&actual);
            SET_ACTUAL(parse_attributes, 0, keep, NULL, "attributes" );
            /* Set callback when parsing of attributes is done */
            actual.OnExit = exit_lvgl_elem;
            return true;
        }
    }

  /* all lvgl elements do not have subitems */
   return true; 
} 

/******************************************************************************
 * @brief parse component
 * @param  token       - actual input token
 * @param  tokenlength - length of token
 * @retval 
 *****************************************************************************/
bool parse_screen ( char *token, uint32_t tokenlength, uint32_t state )
{
    const char *last_exitword;

    /* need at least 2 chars */
    if ( tokenlength < 1 ) return true;

    TokenToLower(token, tokenlength);
    /* Must have an opening bracket first */
    if ( *token != OPENBRACKET ) return true;

    /* In the following we assume, we have an '<' first */
    uint32_t reducedlength = tokenlength-1; 
    if (CheckCloseBracket(token+1, &reducedlength)) {
        /* when a closing bracket is found, we check for exitword, if set */
        if ( actual.exitword && CheckExitWord(token+1, reducedlength, actual.exitword, ESCAPESLASH ) )  {
            return ExitLevel();
        } else {
          /* found a simple closing bracket, so we have found the initital element, continue parsing */
          return true;
        }
    } else {
        /* no exit word, no closing brackets */
        /* now check for elementame w/o closing brackets, that means: parse attribut list for that element */
        if ( CheckOneWord(token+1, reducedlength, actual.exitword) ) {
            /* forward edit receipe, keep actual state, push it and start parsing attributes */
            const GUI_Edit_T *keep = actual.edit;
            xml_parser_push(&actual);
            SET_ACTUAL(parse_attributes, 0, keep, NULL, "attributes" );
            /* Set callback when parsing of attributes is done */
            actual.OnExit = exit_screen;
            return true;
        }
    }

    /* finally check for all subitems */
    uint32_t idx = CheckWord(token+1, reducedlength, xml_component, &last_exitword);
    if ( !idx ) {
        /* not found */
        output_puts("screen: unhandeled subitem: ");
        output_putsl(token+1, reducedlength);
        output_putch('\n');
        return true;
    }

    /* Any allowed component item */
    /* reset state to initial, push actual state 
    /* then store exit word and start parsing item */
    actual.state = 0;
    xml_parser_push(&actual);
    /* Get edit receipe, there should be one */
    const GUI_Edit_T *edit = FindEditInfoByName( token+1, reducedlength);
    if (!edit) {
        #if DEBUG_PARSER > 0
                DEBUG_PRINTF("Subitem w/o Receipe: ");
                DumpToken(token+1, reducedlength);
                DEBUG_PUTC('\n');
        #endif 
    }
    SET_ACTUAL(parse_lvgl_elem, 0, edit, last_exitword, xml_component[idx-1] );
    /* Reset all data fields GUI element */
    GUI_Edit_SetUsedBits(edit, 0, 0);
    xml_parse(token, tokenlength);
    return true; 
} 
/******************************************************************************
 * @brief Parse optional prolog and root element
 * @param  token       - actual input token
 * @param  tokenlength - length of token
 * @retval 
 *****************************************************************************/
bool parse_root ( char *token, uint32_t tokenlength, uint32_t state )
{
    const char *last_exitword;

    /* need at least 2 chars */
    if ( tokenlength < 1 ) return true;

    TokenToLower(token, tokenlength);
    /* Must have an opening bracket first */
    if ( *token != OPENBRACKET ) return true;

    /* In the following we assume, we have an '<' first */
    uint32_t reducedlength = tokenlength-1; 
    if (CheckCloseBracket(token+1, &reducedlength)) {
        /* when a closing bracket is found, we check for exitword, if set */
        if ( actual.exitword && CheckExitWord(token+1, reducedlength, actual.exitword, ESCAPESLASH) )  {
            return ExitLevel();
        }
    }
    
    /* Now check for allowed subitems */
    switch( CheckWord(token+1, reducedlength, xml_root, &last_exitword)  ) {
      case 1:
         /* screen */
         /* reset state to initial, push actual state 
         /* then store exit word and start parsing component */
        actual.state = 0;
        xml_parser_push(&actual);
        SET_ACTUAL(parse_screen, 0, &edit_screen, last_exitword, "screen" );
        /* Reset all default data fields of screen variable */
        GUI_Edit_SetUsedBits(&edit_screen, 0, 0);
        xml_parse(token, tokenlength);
        break;
      case 2:
        /* XML prolog */
         /* reset state to initial, push actual state 
         /* then store exit word and start parsing component */
        actual.state = 0;
        xml_parser_push(&actual);
        SET_ACTUAL(parse_screen, 0, NULL, last_exitword, "prolog" );
        xml_parse(token, tokenlength);
        break;
      default:
        output_puts("found unhandeled item >");
        output_putsl(token+1, reducedlength);
        output_putch('<');output_putch('\n');
#if DEBUG_PARSER > 0
        DEBUG_PRINTF("found unhandeled item >");
        DumpToken(token+1, reducedlength);
        DEBUG_PUTC('<');DEBUG_PUTC('\n');

#endif 
   } /* case */
   return true; 
} 

/******************************************************************************
 * @brief Parse optional prolog by skipping everything until closung bracket
 * @param  token       - actual input token
 * @param  tokenlength - length of token
 * @retval 
 *****************************************************************************/
bool parse_prolog ( char *token, uint32_t tokenlength, uint32_t state )
{
    const char *last_exitword;

    /* need at least 2 chars */
    if ( tokenlength < 1 ) return true;
    TokenToLower(token, tokenlength);
    /* Must have an opening bracket first */
    if ( *token != OPENBRACKET ) return true;

    /* In the following we assume, we have an '<' first */
    uint32_t reducedlength = tokenlength-1; 
    if (CheckCloseBracket(token+1, &reducedlength)) {
        /* when a closing bracket is found, we check for exitword, if set */
        if ( actual.exitword && CheckExitWord(token+1, reducedlength, actual.exitword, ESCAPEPROLOG) )  {
            return ExitLevel();
        } else {
          /* found a simple closing bracket, so we have found the initital element, continue parsing */
          return true;
        }

    }

    /* skip everything else */
   return true; 
} 

/******************************************************************************
 * @brief parse attributes in the format <attr>="<value>"
 * @param  token       - actual input token
 * @param  tokenlength - length of token
 * @retval 
 *****************************************************************************/
bool parse_attributes ( char *token, uint32_t tokenlength, uint32_t state )
{
    uint32_t reflen=2;
    
    /* first search for escaped closing bracket _at the beginning_ */
    if ( CheckEscapedCloseBracket(token, &reflen, ESCAPESLASH) ) {
        /* when /> is found, we need to pop two levels: attribute and parent lvgl elem */
        ExitLevel(); 
        return ExitLevel();
    }

    /* Thereafter for simple closing bracket */
    reflen=1;
    if (CheckCloseBracket(token, &reflen)) {
      return ExitLevel();
    }

    /* therafter search for "=" within token */
    if ( tokenlength > 1 ) {
        int32_t eqpos = FindInToken(token, tokenlength, '=', 1,false);
        if ( eqpos >= 0 ) {
            /* if found handle three parts name,'=', value individually */
            /* first part: name, if present */
            if ( eqpos > 0 ) parse_attributes(token, eqpos, actual.state);
            /* second part: equal sign, it is present, we found it */
            parse_attributes(token+eqpos, 1, actual.state);
            /* third part: value, if present */
            if ( eqpos < tokenlength-1 ) parse_attributes(token+eqpos+1, tokenlength-eqpos-1, actual.state);
            return true;
        }
    }
    /* now handle content according to global state, don't use local value due do recursive call */
    /* and frequent update in this routine */
    switch ( actual.state ) {
      case 0:
        /* handle attribute name */
        TokenToLower(token, tokenlength);
        AttrCopy(token, tokenlength, 1 );
        actual.state++;
        break;
      case 1:
        /* check, that we have a single '=', if not, expect another name next */
        if ( *token == '=' && tokenlength == 1 )
          actual.state++;
        else
          actual.state = 0;
        break;
      case 2:
        /* Handle value: it is case sensitive, so do not convert to upper */
        /* it is enclosed in "" so check for them and remove              */
        actual.state = 0;
        if ( tokenlength > 2  &&  *token == '"' ) {
            int32_t qmpos = FindInToken(token+1, tokenlength-1, '"',1 ,true);
            if ( qmpos >= 0 ) {
              /* Copy attribute value */
              AttrCopy(token+1, qmpos, 2);
              /* And set(update attribute value */
              AttrUpdate(actual.edit);
            }
            /* something behind second '"' ? */
            if ( qmpos < tokenlength - 2 ) parse_attributes(token+1+qmpos+1, tokenlength-qmpos-2, actual.state);
        }
        /* when landing here, sytax rules for attribute value are violated. Continue with attribute name */
        break;
#if DEBUG_PARSER > 0
      default:
        DEBUG_PRINTF("attribute: illegal state %d\n");
#endif 
    } /* inner case */
   return true; 
} 


/******************************************************************************
 * @brief Return XML parser version str
 *****************************************************************************/
const char *xml_parser_version(void)
{
  return XML_PARSER_VERSION;
}


/******************************************************************************
 * @brief Setup/initialize parser to parse optional prolog root element
 *****************************************************************************/
void xml_parser_init(void) 
{  
   /* clear stack from previous parse runs */
   while(!XML_STACK_EMPTY() ) xml_parser_pop(NULL, NULL);
  
   if ( !act_attr ) act_attr = my_malloc(sizeof(Parser_Attr_T));

   /* setup to parse root element */
   SET_ACTUAL(parse_root, 0, NULL, NULL, "Root" );
   i2c_feeder_init();
   output_printf("XML-Parser V%s on LVGL V%s\n",xml_parser_version(), LVGL_VersinfoStr0);
}

/******************************************************************************
 * @brief Setup/initialize parser to parse optional prolog root element
 *****************************************************************************/
void xml_parser_deinit(void) 
{  
   /* clear stack from previous parse runs */
   while(!XML_STACK_EMPTY() ) xml_parser_pop(NULL, NULL);
  
   if ( act_attr ) { my_free(act_attr); act_attr=NULL; }
   

   /* setup to parse root element */
   SET_ACTUAL(parse_root, 0, NULL, NULL, "Root" );
   i2c_feeder_init();
}


  /**
  * @}
  */

