#include "config/config.h"
#if USE_LVGL > 0
	
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include "cmdline.h"

#include "../GUI/gui_def.h"
#include "../GUI/gui_edit.h"
#include "../GUI/gui_lists.h"

/* Edit receipe for Style structure */
const  GUI_Edit_T edit_style = {
  .count         = 15,
  .gui_elem_type = GUI_ELEM_STYLE,
  /* the foloowing two items are used to find used bits and name when */
  /* structure will be converted to raw data in "gui_edit"            */
  .used_ofs      = offsetof(GUI_Style_T, used),
  .name_ofs      = offsetof(GUI_Style_T, name),
  .total_size    = sizeof  (GUI_Style_T),
  /* Element Order has to be the same as in corresponding "used"-bit set !!! */
  .gui_element   = { 
    { "Width",        GUI_UINT16, offsetof(GUI_Style_T, def_width) },
    { "Height",       GUI_UINT16, offsetof(GUI_Style_T, def_height) }, 
    { "Align",        GUI_UINT8,  offsetof(GUI_Style_T, align) }, 
    { "Backgr.opaq",  GUI_UINT8,  offsetof(GUI_Style_T, bgopa) }, 
    { "BorderWidth",  GUI_UINT8,  offsetof(GUI_Style_T, borderwidth) }, 
    { "BorderRadius", GUI_UINT8,  offsetof(GUI_Style_T, borderradius) }, 
    { "Shadow Width", GUI_UINT8,  offsetof(GUI_Style_T, shadow_width) }, 
    { "Shadow opaq",  GUI_UINT8,  offsetof(GUI_Style_T, shadow_opa) }, 
    { "Shadow xref",  GUI_UINT8,  offsetof(GUI_Style_T, sh_x) }, 
    { "Shadow yref",  GUI_UINT8,  offsetof(GUI_Style_T, sh_y) }, 
    { "BGColor",      GUI_RGB888, offsetof(GUI_Style_T, bgcolor) }, 
    { "BorderColor",  GUI_RGB888, offsetof(GUI_Style_T, bordercolor) }, 
    { "TextColor",    GUI_RGB888, offsetof(GUI_Style_T, txtcolor) }, 
    { "ShadowColor",  GUI_RGB888, offsetof(GUI_Style_T, shadowcolor) }, 
    { "StyleName",    GUI_STRING, offsetof(GUI_Style_T, name) }, 
  },
};

const  GUI_Edit_T edit_label = {
  .count         = 6,
  .gui_elem_type = GUI_ELEM_LABEL,
  .used_ofs      = offsetof(GUI_Label_T, used),
  .name_ofs      = offsetof(GUI_Label_T, name),
  .total_size    = sizeof  (GUI_Label_T),
  /* Element Order has to be the same as in corresponding "used"-bit set !!! */
  .gui_element   = { 
    { "Style",        GUI_STYLE,  offsetof(GUI_Label_T, style) }, 
    { "Align",        GUI_UINT8,  offsetof(GUI_Label_T, align) }, 
    { "X0",           GUI_UINT16, offsetof(GUI_Label_T, x0) }, 
    { "Y0",           GUI_UINT16, offsetof(GUI_Label_T, y0) }, 
    { "Caption",      GUI_STRING, offsetof(GUI_Label_T, caption) }, 
    { "LabelName",    GUI_STRING, offsetof(GUI_Label_T, name) }, 
  },
};


/* helper struct to define a string by char vector and length*/
struct str { 
    char *text;
    size_t len;
};
/* module global variable to hold data of edited item */
static union {
      uint32_t   u32;
      uint32_t   u16[2];
      uint8_t    u8[4];
      lv_color_t rgb;
      struct str str;
    } GUI_Edit_tempval;

/* the following two variables are set/maintained by "GUI_Edit" */
uint8_t          *act_obj;    /* actual edited object   */
const GUI_Edit_T *act_edit;   /* object's edit receipe  */
OnExitFn         OnExitEdit;  /* Callback on Exit of Editing */

const uint8_t GUI_ByteLen[] = BYTELENGTHS;

// ---- Forward declaration of user input handler in GI edit mode -------------
void handle_gui_edit_input ( char *cmdline, size_t len );

/******************************************************************************
 * Edit one GUI-Element
 * data     = ptr to complete gui structure
 * editelem = description of element to dump
 * textlen  = length to right pad the edittext with blanks, 0=no padding
 ******************************************************************************/
static void GUI_edit_dump_one ( uint8_t *bytes, const GUI_editelem_T *editelem, bool used, uint32_t textlen ) 
{
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
    GUI_Edit_tempval.u32=0;
    if (editelem->elem_type != GUI_STRING )
       memmove(GUI_Edit_tempval.u8, bytes+editelem->elem_offset, GUI_ByteLen[editelem->elem_type] );
    else
      /* In case of string store ptr to string */
       GUI_Edit_tempval.str.text = (char *)bytes+editelem->elem_offset;

    switch(editelem->elem_type) {
      case GUI_UINT8:
        printf("%u", GUI_Edit_tempval.u8[0] );
        break;
      case GUI_UINT16:
        printf("%u", GUI_Edit_tempval.u16[0] );
        break;
      case GUI_RGB888:
        printf("0x%02x%02x%02x",  GUI_Edit_tempval.rgb.blue, GUI_Edit_tempval.rgb.green, GUI_Edit_tempval.rgb.red );
        break;
      case GUI_UINT32:
        printf("%u", GUI_Edit_tempval.u32 );
        break;
      case GUI_STRING:
        printf("%s", GUI_Edit_tempval.str.text );
        break;
      default:
        printf("<No output format defined for %d",editelem->elem_type);
        return;
    }
}

/******************************************************************************
 * return the max strlen of all edit elements
 ******************************************************************************/
static uint32_t GUI_edit_get_maxtextlen ( const GUI_Edit_T *editdata )
{
  uint32_t ret=0;
  uint32_t l;
  for ( uint32_t i=0; i < editdata->count; i++ ) {
    l = strlen(editdata->gui_element[i].elem_name);
    if ( l > ret ) ret=l;
  }
  return ret;
}

/******************************************************************************
 * @brief  Dump a complete GUI-Element,
 * @param  data     - raw element data
 * @param  editdata - edit receipe for raw data
 * @param  padded   - if true, data display is left aligned to description
 ******************************************************************************/
void GUI_edit_dump_all(uint8_t *data, const GUI_Edit_T *editdata, bool padded )
{
  uint32_t padlen;
  bool used;
  /* set padding length, if desired */
  if ( padded ) padlen = GUI_edit_get_maxtextlen(editdata);

  /* Dump all data */
  for ( uint32_t i=0; i < editdata->count; i++ ) {
    /* get the used bit */
    used = *(uint32_t *)(data + editdata->used_ofs ) & ( 1 << i );
    printf("%2d: ",i);
    GUI_edit_dump_one(data, &editdata->gui_element[i], used, padlen); 
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
void GUI_Edit( const GUI_Edit_T *edit, void *anyobj, OnExitFn OnExit  )
{
  /* Store edit pattern and data vector */
  act_edit   = edit;
  act_obj    = (uint8_t *)anyobj;
  OnExitEdit = OnExit;

  /* First, dump the whole element */
  GUI_edit_dump_all(act_obj, act_edit, true );

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

#define min(a,b)    ((a)<(b)?a:b)
/*********************************************************************************
 * @brief  update the "idx"th element of actual data element
 *         new value is stored in GUI_Edit_tempval union
 * @param  bIsNull - true, if new value is <Unset> 
 ********************************************************************************/
static void GUI_Edit_update( uint32_t idx, bool bIsUnset )
{

    /* get reference to "used" bits */
    uint32_t *used = (uint32_t *)(act_obj + act_edit->used_ofs);

    /* Handle <Unset> first */
    if ( bIsUnset ) {
      *used &= ~( 1 << idx );
      return;
    } else {
      *used |= ( 1 << idx );
    }

    /* get the referenced edit element */
    const GUI_editelem_T *editelem = &act_edit->gui_element[idx];

    /* Get the correct storage position of data */
    uint8_t *datapos = act_obj + editelem->elem_offset;

    /* Store data, strings require special handling */
     if ( editelem->elem_type != GUI_STRING ) {
        memmove(datapos, GUI_Edit_tempval.u8, GUI_ByteLen[editelem->elem_type] );

        /* Reverse byte order in RGB value */
        if ( editelem->elem_type == GUI_RGB888 ) {
          uint8_t c = *datapos; *datapos = *(datapos+2); *(datapos+2)=c;
        }
    } else {
      /* copy string into data structure */
      size_t len = min(GUI_MAX_NAMELEN-1, GUI_Edit_tempval.str.len);
      strncpy(datapos, GUI_Edit_tempval.str.text, len);
      /* incoming string is (vector, length) type so append \0 in any case */
      *(datapos+len)='\0';
    }
}

/******************************************************************************
 * @brief  Store a complete GUI-Element into global GUI element list
 * @param  data     - raw element data
 * @param  editdata - edit receipe for raw data
 ******************************************************************************/
void GUI_save_entry(uint8_t *data, const GUI_Edit_T *editdata )
{
  /* create a full copy of actual data structure */
  uint8_t *copy = malloc(editdata->total_size);
  if ( !copy ) {
    printf("malloc failed!\n");
    return;
  }
  memcpy_fast(copy, data, editdata->total_size);

  /* find position of "name" field and pass that to list entry */
  char *name = (char *)(copy + act_edit->name_ofs);
  LL_append(&GUI_item_list, editdata->gui_elem_type, name, copy );
}

/******************************************************************************
 * @brief  List all globel GUI elements of actual edit type,
 * @param  editdata - edit receipe for raw data
 ******************************************************************************/
void GUI_list_entries(const GUI_Edit_T *editdata )
{
  List_Elem_T* ptr = GUI_item_list;
  uint32_t i=0;

  printf("All Elements of type %s in global list\n", EditNames[act_edit->gui_elem_type] );

  /* Iterate thr all elements of current edit type */
  while ( ptr = LL_iterate_by_type ( ptr, editdata->gui_elem_type ) ) {
    printf("%2d: %s\n", ++i, ptr->ll_name);
    ptr = LL_next(ptr);
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


/*********************************************************************************
 * @brief  Execute command in GUI edit mode
 * @param  word    - ptr to next unparsed word in input stream
 * @param  wordlen - length of next word
 * @param  idx     - numerical value of first ( already removed ) word in input stream
 * @retval true, if input could be parsed completely, false if not
 ********************************************************************************/
static bool GUI_Edit_execute_entry ( char *word, size_t wordlen, uint32_t idx )
{

    /* idx is the first numerical value of the command line, check for being in range*/
    if ( idx >= act_edit->count ) {
      printf("Edit entry: %d out of range 0 .. %d\n", idx, act_edit->count );
      return false;
    }

    /* read the new value for that entry */
    if ( CMD_argc() < 1 ) {
      printf("Missing value for entry %d\n", idx);
      return false;
    }

    /* get the referenced edit element */
    const GUI_editelem_T *editelem = &act_edit->gui_element[idx];

    CMD_get_one_word( &word, &wordlen );

    /* First check for NULL */
    if ( GUI_is_nullword(word, wordlen) ) {
        /* and update raw element data */
       GUI_Edit_update(idx, true);
       return true;
    }
    
    /* Thereafter check for number or string*/
    if ( editelem->elem_type != GUI_STRING ) {
        GUI_Edit_tempval.u32 = CMD_to_number( word, wordlen );
    } else {
        /* Strings require a separate handling: pass reference to the string 
         * to Updater, updater will copy string out of inbuf into data structure
         */
         GUI_Edit_tempval.str.text = word;
         GUI_Edit_tempval.str.len = wordlen;
    }

    /* and update raw element data */
    GUI_Edit_update(idx, false);
   
    return true;
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
      GUI_edit_dump_all(act_obj, act_edit, true );
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
      GUI_save_entry(act_obj, act_edit );
      return;
   }

   /* handle list command */
   if ( GUI_is_listword( word, wordlen ) ) {
      GUI_list_entries(act_edit);
      return;
   }


   /* handle numeric command */
   if ( CMD_is_numeric( word, wordlen ) ) {
      uint32_t idx = CMD_to_number( word, wordlen );
      GUI_Edit_execute_entry(word, wordlen, idx );
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

