#include "../GUI/gui_edit.h"

#if USE_LVGL > 0
	
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include "cmdline.h"

/* module global variable to hold data of edited item */
static union {
      uint32_t   u32;
      uint32_t   u16[2];
      uint8_t    u8[4];
      lv_color_t rgb;
      char       *txt;
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
static void GUI_edit_dump_one ( uint8_t *bytes, const GUI_editelem_T *editelem, uint32_t textlen ) 
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

    /* copy data and output with specified format */
    GUI_Edit_tempval.u32=0;
    memmove(GUI_Edit_tempval.u8, bytes+editelem->elem_offset, GUI_ByteLen[editelem->elem_type] );
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
        printf("%s", GUI_Edit_tempval.txt );
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
void GUI_edit_dump_all(void *data, const GUI_Edit_T *editdata, bool padded )
{
  uint32_t padlen;
  /* set padding length, if desired */
  if ( padded ) padlen = GUI_edit_get_maxtextlen(editdata);

  /* Dump all data */
  for ( uint32_t i=0; i < editdata->count; i++ ) {
    printf("%2d: ",i);
    GUI_edit_dump_one(data, &editdata->gui_element[i], padlen); 
    putchar('\n');
  }
}

/******************************************************************************
 * @brief set the prompt when in GUI edit mode 
 ******************************************************************************/
void GUI_Edit_Prompt(void)
{
  printf("GUI Edit %s>",act_edit->editname);
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

/*********************************************************************************
 * @brief  update the "idx"th element of actual data element
 *         new value is stored in GUI_Edit_tempval union
 ********************************************************************************/
static void GUI_Edit_update( uint32_t idx )
{
    /* get the referenced edit element */
    const GUI_editelem_T *editelem = &act_edit->gui_element[idx];
    /* Get the correct storage position of data */
    uint8_t *datapos = act_obj + editelem->elem_offset;

    /* Store data */
    memmove(datapos, GUI_Edit_tempval.u8, GUI_ByteLen[editelem->elem_type] );

    /* Reverse byte order in RGB value */
    if ( editelem->elem_type == GUI_RGB888 ) {
      uint8_t c = *datapos; *datapos = *(datapos+2); *(datapos+2)=c;
    }
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
    
    if ( editelem->elem_type != GUI_STRING ) {
        GUI_Edit_tempval.u32 = CMD_to_number( word, wordlen );
    } else {
        /* Strings require a separate handling: copy new string out of the 
         * input buffer to heap string and assign this string to label text
         * no chance to free this label text anyhow !
         */
         GUI_Edit_tempval.txt = malloc(wordlen+1);
         strcpy( GUI_Edit_tempval.txt, word);
    }

    /* and update raw element data */
    GUI_Edit_update(idx);
   
    return true;
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

