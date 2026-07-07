#include "config/config.h"
#if USE_GUI_INTERFACE > 0
#include "debug/debug_helper.h"
	
#include <string.h>
#include <stdio.h>
#include "cmdline.h"

#include "../GUI/variant.h"
#include "../GUI/gui_def.h"
#include "../GUI/gui_edit.h"
#include "../GUI/gui_lists.h"




Variant_T V_tempval;

/* module global variable to copy string data out of inbuf */
char tempstr[GUI_MAX_NAMELEN];

const GUI_Edit_T *act_edit;   /* object's edit receipe  */
OnExitFn         OnExitEdit;  /* Callback on Exit of Editing */

const GUI_Edit_TypeSpec_T GUI_TypeSpec[GUI_MAXELEM] = GUI_TYPE_SPECS;



// ---- Forward declaration of user input handler in GI edit mode -------------
void handle_gui_edit_input ( char *cmdline, size_t len );


/******************************************************************************
 * Edit one GUI-Element
 * data     = ptr to complete gui structure
 * editelem = description of element to dump
 * textlen  = length to right pad the edittext with blanks, 0=no padding
 ******************************************************************************/
static void GUI_edit_dump_one ( uint8_t *bytes, const Edit_Receipe_T *editelem, bool used, uint32_t textlen ) 
{
    List_Elem_T *ll_elem;
    GUI_Edit_Enum  search_elem;

    printf(editelem->elem_name);
    if ( textlen > 0 ) {
      size_t namelen = strlen(editelem->elem_name);
      /* right pad with blanks */
      if ( textlen > namelen ) {
        for ( textlen=textlen-namelen; textlen; textlen-- ) putchar(' ');
      }
    }
    putchar(' ');putchar('=');putchar(' ');

    /* First check for Unset */
    if ( !used ) { printf("<Unset>"); return; }

    /* copy data and output with specified format */
    /* GUI String requires special handling       */
    V_Init(&V_tempval);
    switch ( editelem->elem_type ) {
      case GUI_STRING:
           /* In case of string store ptr to string, it is NULL terminated */
           V_tempval.str.text = (char *)bytes+editelem->elem_offset;
           printf("%s", V_tempval.str.text );
           break;
      case GUI_STYLE:
      case GUI_FONT:
           /* In case of Style or Font find it in GUI item list and store the name*/
           search_elem = ( editelem->elem_type == GUI_STYLE ? GUI_ELEM_STYLE : GUI_ELEM_FONT );
           ll_elem = LL_find_by_type_n_obj( GUI_item_list, search_elem, *(void **)(bytes+editelem->elem_offset) );
           V_tempval.font.fontname.text = (char *)ll_elem->ll_name;
           printf("%s", V_tempval.font.fontname.text );
           /* In case of Fonts,also get the fontsize */
           if ( search_elem == GUI_ELEM_FONT ) {
              V_tempval.font.fontsize = ((GUI_Font_T*)ll_elem->ll_entry)->fontsize;
              printf(" %u", V_tempval.font.fontsize );
          }
          break;
      case GUI_UINT8:
      case GUI_UINT16:
      case GUI_RGB888:
      case GUI_UINT32:
      case GUI_INT8:
      case GUI_INT16:
      case GUI_INT32:
          /* handle all tpye of numbers by just copying the raw bytes to tempval */
          memmove(V_tempval.u8, bytes+editelem->elem_offset, GET_GUI_ELEM_LEN(editelem->elem_type) );
          /* And print with different formats/ lengths */
          switch(editelem->elem_type) {
            case GUI_UINT8:
              printf("%u", V_Get_U8(&V_tempval) );
              break;
            case GUI_UINT16:
              printf("%u", V_Get_U16(&V_tempval) );
              break;
            case GUI_RGB888:
              printf("0x%06x", V_Get_Rgb(&V_tempval)&0xFFFFFF );
              break;
            case GUI_UINT32:
              printf("%u", V_Get_U32(&V_tempval) );
              break;
            case GUI_INT8:
              printf("%d", V_Get_I8(&V_tempval) );
              break;
            case GUI_INT16:
              printf("%d", V_Get_I16(&V_tempval) );
              break;
            case GUI_INT32:
              printf("%d", V_Get_I32(&V_tempval));
              break;
          } //inner switch
          break;
      default:
        printf("<No output format defined for %d",editelem->elem_type);
        return;
    } // outer switch;
}

/******************************************************************************
 * return the max strlen of all edit elements
 ******************************************************************************/
static uint32_t GUI_edit_get_maxtextlen ( const GUI_Edit_T *editdata )
{
  uint32_t ret=0;
  uint32_t l;
  for ( uint32_t i=0; i < editdata->count; i++ ) {
    l = strlen(editdata->receipe[i].elem_name);
    if ( l > ret ) ret=l;
  }
  return ret;
}

/******************************************************************************
 * @brief  Apply an "and" and "or" mask to one GUI element data items "used" flags 
 * @param  editdata - edit receipe for GUI element
 * @param  andMask  - AND mask to apply 
 * @param  orMask   - OR mask to apply
 * @retval          resulting flag arry is returned 
 ******************************************************************************/
uint32_t GUI_Edit_SetUsedBits(const GUI_Edit_T *editdata, uint32_t andMask, uint32_t orMask)
{
  /* Get position of 'used' field */
  uint32_t *usedptr = (uint32_t *)(editdata->workspace + editdata->used_ofs );
  /* apply masks */
  *usedptr = *usedptr & andMask | orMask;
  return *usedptr;
}
/******************************************************************************
 * @brief  Dump a complete GUI-Element,
 * @param  data     - raw element data
 * @param  editdata - edit receipe for raw data
 * @param  padded   - if true, data display is left aligned to description
 ******************************************************************************/
void GUI_edit_dump_all(const GUI_Edit_T *editdata, bool padded )
{
  uint32_t padlen;
  bool used;
  /* set padding length, if desired */
  if ( padded ) padlen = GUI_edit_get_maxtextlen(editdata);

  /* Dump all data */
  uint8_t *data = editdata->workspace;
  for ( uint32_t i=0; i < editdata->count; i++ ) {
    /* get the used bit */
    used = *(uint32_t *)(data + editdata->used_ofs ) & ( 1 << i );
    printf("%2d: ",i);
    GUI_edit_dump_one(data, &editdata->receipe[i], used, padlen); 
    putchar('\n');
  }
}

/******************************************************************************
 * @brief set the prompt when in GUI edit mode 
 ******************************************************************************/
void GUI_Edit_Prompt(void)
{
  printf("GUI Edit %s>",EditNames[act_edit->gui_elem_type]);
}


/******************************************************************************
 * @brief Start editing of any GUI object
 * @param anyobj - ptr to raw object data
 * @param edit   - edit receipe for that obj
 ******************************************************************************/
void GUI_Edit( const GUI_Edit_T *edit, OnExitFn OnExit  )
{
  /* Store edit pattern and data vector */
  act_edit   = edit;
  OnExitEdit = OnExit;

  /* First, dump the whole element */
  GUI_edit_dump_all(act_edit, true );

  /* change Input Handler */
  CMD_Handler_Push(handle_gui_edit_input, GUI_Edit_Prompt);

  /* Edit mode will be quit by editmode handler futher down */
  /* For now, we are done here */
}

/******************************************************************************
 * @brief Help text for help command in GUI edit mode
 ******************************************************************************/
void GUI_PrintHelp(void)
{
  puts("<ord> <val> [ <ord> <val> , ... ] to alter values\n"\
       "'print' to dump current value\n"
       "'exit' or'x' to exit edit mode\n"\
       "'s(ave)' to store current structure in global item list\n"\
       "'l(ist)' to list all elements of current type in item list\n"\
       "'g(et) <name>' to load entry <name> into editor\n"\
       "'h' or '?' for help");
}

/*********************************************************************************
 * @brief  Terminate GUI element editing
 ********************************************************************************/
void GUI_Handler_Pop ( void )
{
    if ( OnExitEdit ) OnExitEdit();
    /* Restore previous input handler */
    putchar('\n');
    CMD_Handler_Pop();
}

#if 0
#define min(a,b)    ((a)<(b)?a:b)
/*********************************************************************************
 * @brief  copy the string tempval ( by char array/length ) to Null terminated
 *         destination
 *         destination _must_ be of minimal length GUI_MAX_NAMELEN
 * @param  dest - buffer of minimal length GUI_MAX_NAMELEN
 ********************************************************************************/
static char *GUI_tempval_to_str(char *dest)
{
      /* copy string into data structure */
      size_t len = min(GUI_MAX_NAMELEN-1, V_tempval.strfont.len);
      strncpy(dest, V_tempval.strfont.text, len);
      /* incoming string is (vector, length) type so append \0 in any case */
      *(dest+len)='\0';
      return dest;
}
#endif
/*********************************************************************************
 * @brief  update the "idx"th element of actual data element
 *         new value is stored in V_tempval union
 * @param  bIsNull - true, if new value is <Unset> 
 ********************************************************************************/
static bool GUI_Edit_update( const GUI_Edit_T *edit, uint32_t idx, bool bIsUnset )
{

    /* get reference to "used" bits */
    uint32_t *used = (uint32_t *)(edit->workspace + edit->used_ofs);

    /* Handle <Unset> first */
    if ( bIsUnset ) {
       *used &= ~( 1 << idx );
      return true;
    } else {
       *used |= ( 1 << idx );
    }

    /* get the referenced edit element */
    const Edit_Receipe_T *editelem = &edit->receipe[idx];

    /* Get the correct storage position of data */
    uint8_t *datapos = edit->workspace + editelem->elem_offset;
    lv_style_t **keks = (lv_style_t **)datapos;
    GUI_Edit_Enum search_elem;
    uint32_t search_idx;
    List_Elem_T *ll_elem;
    bool bNumeric;
    switch ( editelem->elem_type ) {
      case GUI_STRING:
        /* copy string into data structure directly */
        V_to_cstr(&V_tempval, (char *)datapos, GUI_MAX_NAMELEN);
        break;
      case GUI_STYLE:
      case GUI_FONT:
        /* Fonts and styles may be secified in two ways: by name and size  or position in list ( starting with 1 ) */
        search_elem = ( editelem->elem_type == GUI_STYLE ? GUI_ELEM_STYLE : GUI_ELEM_FONT );
        if ( CMD_is_numeric (V_tempval.font.fontname.text, V_tempval.font.fontname.len )) {
          /* Specified by number: convert to num and search for nth entry, LL_find_nth counts from 1 ... ! */
          ll_elem = LL_find_nth ( GUI_item_list,  search_elem, CMD_to_number(V_tempval.font.fontname.text, V_tempval.font.fontname.len ) + 1 );
        } else {
          /* Specified by name: Get String copy from inbuf in any case */
          V_to_cstr(&V_tempval, tempstr, GUI_MAX_NAMELEN);
          if ( editelem->elem_type == GUI_STYLE ) {
            /* find Style by name */
            ll_elem = LL_find_by_type_n_name(GUI_item_list, search_elem, tempstr);
          } else {
            /* find font by name and fontsize */
            ll_elem = LL_find_by_type_name_additional(GUI_item_list, search_elem, tempstr,V_tempval.font.fontsize);
          }
        }
        /* if found, copy lvgl obj ptr to structure, otherwise reset "used" bit */
        if ( ll_elem ) {
           *(void **)datapos = ll_elem->ll_lvgl_obj;
        } else {
           printf("%s %s not found\n",EditNames[search_elem],tempstr);
           *used &= ~( 1 << idx );
           return false;
        }
        break;
      case GUI_UINT8:
      case GUI_UINT16:
      case GUI_RGB888:
      case GUI_UINT32:
      case GUI_INT8:
      case GUI_INT16:
      case GUI_INT32:
        /* All Number formats: copy required number of bytes to data element */
        memmove(datapos, V_tempval.u8, GET_GUI_ELEM_LEN(editelem->elem_type) );
        /* Reverse byte order in RGB value */
        /* if ( editelem->elem_type == GUI_RGB888 ) {
          uint8_t c = *datapos; *datapos = *(datapos+2); *(datapos+2)=c;
        } */
        break;
      default:
        printf("No Update receipe for data type %d\n",editelem->elem_type );
        return false;
    } // case
    return true;
}

/******************************************************************************
 * @brief  List all globel GUI elements of actual edit type,
 * @param  editdata - edit receipe for raw data
 ******************************************************************************/
void GUI_list_entries(const GUI_Edit_Enum elemtype )
{
  List_Elem_T* ptr = GUI_item_list;
  uint32_t i=0;

  printf("All Elements of type %s in global list\n", EditNames[elemtype] );

  /* Iterate thr all elements of current edit type */
  while ( ptr = LL_iterate_by_type ( ptr, elemtype ) ) {
    printf("%2d: %s", i, ptr->ll_name);
    if ( elemtype == GUI_ELEM_FONT ) printf(" %u", ptr->ll_additional); 
    putchar('\n');
    i++;
    ptr = LL_next(ptr);
  }
}


/********************************************************************************
 * @brief load an GUI Element 
 ********************************************************************************/
static void GUI_load_entry( char *word, size_t wordlen, const GUI_Edit_T *edit )
{
   V_Set_Str(&V_tempval, word, wordlen);
   V_to_cstr(&V_tempval, tempstr, GUI_MAX_NAMELEN);

  List_Elem_T *ll_elem = LL_find_by_type_n_name ( GUI_item_list, edit->gui_elem_type, tempstr );
  if ( !ll_elem ) {
    printf("Err: %s %s not in item list\n",EditNames[edit->gui_elem_type],tempstr);
  } else {
    /* copy element data to current obj data */
    memcpy_fast(edit->workspace, ll_elem->ll_entry, edit->total_size);
    printf("%s %s loaded\n",EditNames[edit->gui_elem_type],tempstr);
  }

}

/********************************************************************************
 * @brief returns true, if the given word is the word for NULL ( unset )
 ********************************************************************************/
static bool GUI_is_nullword( char *word, size_t wordlen ) 
{  
  /* check for 'NULL' */
  if ( CMD_compare_words(word, wordlen, "NULL", 4 ) > 0 ) return true;

  return false;
}


/********************************************************************************
 * @brief returns true, if the given word is the word for dump data 
 *        i.e. 'print' 
 ********************************************************************************/
static bool GUI_is_dumpword( char *word, size_t wordlen ) 
{  
  /* check for 'PRINT' */
  if ( CMD_compare_words(word, wordlen, "PRINT", 5 ) > 0 ) return true;

  return false;
}

/********************************************************************************
 * @brief returns true, if the given word is the word for exit 
 *        i.e. 'exit' or 'X'
 ********************************************************************************/
static bool GUI_is_exitword( char *word, size_t wordlen ) 
{
  /* check for 'X' */
  if ( wordlen == 1 && *word == 'X' ) return true;
  
  /* check for 'EXIT' */
  if ( CMD_compare_words(word, wordlen, "EXIT", 4 ) > 0 ) return true;

  return false;
}

/********************************************************************************
 * @brief returns true, if the given word is the word for save 
 *        i.e. 'save' or 'S'
 ********************************************************************************/
static bool GUI_is_saveword( char *word, size_t wordlen ) 
{
  /* check for 'S' */
  if ( wordlen == 1 && *word == 'S' ) return true;
  
  /* check for 'SAVE' */
  if ( CMD_compare_words(word, wordlen, "SAVE", 4 ) > 0 ) return true;

  return false;
}

/********************************************************************************
 * @brief returns true, if the given word is the word for list 
 *        i.e. 'list' or 'L'
 ********************************************************************************/
static bool GUI_is_listword( char *word, size_t wordlen ) 
{
  /* check for 'L' */
  if ( wordlen == 1 && *word == 'L' ) return true;
  
  /* check for 'SAVE' */
  if ( CMD_compare_words(word, wordlen, "LIST", 4 ) > 0 ) return true;

  return false;
}

/********************************************************************************
 * @brief returns true, if the given word is the word for list 
 *        i.e. 'list' or 'L'
 ********************************************************************************/
static bool GUI_is_getword( char *word, size_t wordlen ) 
{
  /* check for 'G' */
  if ( wordlen == 1 && *word == 'G' ) return true;
  
  /* check for 'GET' */
  if ( CMD_compare_words(word, wordlen, "GET", 3 ) > 0 ) return true;

  return false;
}

/*********************************************************************************
 * @brief  backend for "GUI_Edit_execute_XML" and GUI_Edit_execute_interactive"
 * @param  arg    - property value as string not neccessarily NULL terminated
 * @param  argsize - length of that string ( even if strin is NULL terminated
 * @param  idx     - index of item in editdata whose property is altered
 * @retval true, if property could be successful updated, falseid not
 ********************************************************************************/
bool GUI_Edit_SetItem(char *arg, size_t argsize, const GUI_Edit_T *edit, uint32_t idx )
{

    /* check idx for being in range*/
    if ( idx >= edit->count ) {
      printf("Edit SetItem: %d out of range 0 .. %d\n", idx, edit->count );
      return false;
    }
    /* First check for NULL */
    if ( GUI_is_nullword(arg, argsize) ) {
        /* and update raw element data */
       return GUI_Edit_update(edit, idx, true);
    }

    /* get the referenced edit element */
    const Edit_Receipe_T *editelem = &edit->receipe[idx];

   /* Thereafter check for number or string*/
    if ( editelem->elem_type == GUI_STRING || editelem->elem_type == GUI_STYLE || editelem->elem_type == GUI_FONT) {
        /* Strings, Fonts and Styles require a separate handling: pass reference to the string 
         * to Updater, updater will handle string accordingly
         */
         V_Set_Str(&V_tempval, arg, argsize);

         if (editelem->elem_type == GUI_FONT && ! V_Str_to_Font(&V_tempval) ) return false; 
    } else {
         V_Set_U32(&V_tempval, CMD_to_number( arg, argsize ));
    }

    /* and update raw element data */
    return GUI_Edit_update(edit, idx, false);


}

/*********************************************************************************
 * @brief  Execute command in GUI commandline edit mode. 
 * @param  word    - ptr to next unparsed word in input stream
 * @param  wordlen - length of next word
 * @param  idx     - numerical value of first ( already removed ) word in input stream
 * @retval true, if input could be parsed completely, false if not
 ********************************************************************************/
static bool GUI_Edit_execute_interactive ( char *word, size_t wordlen, uint32_t idx )
{

    /* read the new value for that entry */
    if ( CMD_argc() < 1 ) {
      printf("Missing value for entry %d\n", idx);
      return false;
    }

    /* get the referenced edit element */
    const Edit_Receipe_T *editelem = &act_edit->receipe[idx];

    CMD_get_one_word( &word, &wordlen );
    
    /* Check, whether a list of all options is required */
    if ( editelem->elem_type == GUI_STYLE || editelem->elem_type == GUI_FONT ) {
        /* in Case of Styles/Fonts: if Parameter is '?', list all styles/fonts */
        if ( wordlen == 1 && *word =='?' ) {
            GUI_Edit_Enum search_elem = ( editelem->elem_type == GUI_STYLE ? GUI_ELEM_STYLE : GUI_ELEM_FONT );
            GUI_list_entries(search_elem);
            return true;
        }
    }

    return GUI_Edit_SetItem(word, wordlen, act_edit, idx );
   }


/********************************************************************************
 * @brief  Handle the netg word in input stream. first, all special commands as
 *         exit, help or dump eg are checked
 *         Thereafter a number is expected as the element index that will be altered
 *         followed by the new data value. May be followed by more idx/data pairs
 * @note   word matching is executed on an uppercase base
 ********************************************************************************/
static void GUI_handle_word( char *word, size_t wordlen )
{
   /* first convert given word to UC */
   CMD_word_to_uc( word, wordlen );

   /* handle help command */
   if ( CMD_is_helpword( word, wordlen ) ) {
      putchar('\n');
      GUI_PrintHelp();
      return;
   }

   /* handle print command */
   if ( GUI_is_dumpword( word, wordlen ) ) {
      putchar('\n');
      GUI_edit_dump_all(act_edit, true );
      return;
   }

   /* handle exit command */
   if ( GUI_is_exitword( word, wordlen ) ) {
      GUI_Handler_Pop();
      return;
   }

   /* handle save command */
   if ( GUI_is_saveword( word, wordlen ) ) {
      putchar('\n');
      GUI_new_or_update_entry(act_edit->workspace, act_edit->gui_elem_type );
      GUI_list_entries(act_edit->gui_elem_type);
      return;
   }

   /* handle list command */
   if ( GUI_is_listword( word, wordlen ) ) {
      GUI_list_entries(act_edit->gui_elem_type);
      return;
   }

   /* handle get command */
   if ( GUI_is_getword( word, wordlen ) ) {
      /* read entryname to load */
      if ( CMD_argc() < 1 ) {
        printf("Missing entryname to load\n");
        return;
      }     
      CMD_get_one_word( &word, &wordlen );
      GUI_load_entry(word, wordlen, act_edit);
      return;
   }


   /* handle numeric command */
   if ( CMD_is_numeric( word, wordlen ) ) {
      uint32_t idx = CMD_to_number( word, wordlen );
      GUI_Edit_execute_interactive(word, wordlen, idx );
      putchar('\n');
      return;
   }

}


/******************************************************************************
 * GUI edit command line handler, will be called when user types CR 
 *****************************************************************************/
void handle_gui_edit_input ( char *cmdline, size_t len )
{

  char *word;
  size_t wordlen;
  if (!CMD_parse_line ( cmdline, len ) ) {
    printf("Too many words, max is %d\n", MAX_WORDS);
  }

  while ( act_word < word_num ) {
   CMD_get_one_word ( &word, &wordlen );
   GUI_handle_word(word, wordlen );
  }

}


#endif /* USE_LVGL > 0 */ 

