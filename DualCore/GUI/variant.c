#include "debug/debug_helper.h"
#include "./variant.h"
#include <string.h>
#include "cmdline.h"   /* due to "CMD_to_number" */

/*************************************************************************************************************
 * @brief Copy string from variant type String or Font to destination, 0 terminated
 * @param v      - variant to copy from
 * @param dest   - destination to copy to, 
 * @param maxlen - maximal length that dest can take, including terminating \0
 * @retval       - ptr to destination
 *************************************************************************************************************/
char *V_to_cstr(char *dest, Variant_T *v, size_t maxlen)
{
      String_T *src;
      /* check type */
      if ( v->type = VAR_STRING )
        src=&v->str;
      else if ( v->type = VAR_FONT )
        src=&v->font.fontname;
      else 
        /* only strings and fontnames can be copied out */
        return NULL;

      /* copy string into data structure */
      size_t len = MIN(maxlen-1, src->len);
      strncpy(dest, src->text, len);
      /* incoming string is (vector, length) type so append \0 in any case */
      *(dest+len)='\0';
      return dest;
}


/*************************************************************************************************************
 * @brief A Variant of type String in the format "<fontname><sep><fontsize>" is converted to a Font 
 * representation valid separators are _ and . 
 *        
 * @param v      - variant of type string
 * @param dest   - destination to copy to, 
 * @param maxlen - maximal length that dest can take, including terminating \0
 * @retval       - true, if conversion was successful, false if not 

 *************************************************************************************************************/
bool  V_Str_to_Font(Variant_T *v )
{
    const char delim[]="_.,#:";
    
    if ( v->type != VAR_STRING  ) return false;

    char *idx;
    char *pos = (char *)delim;
    /* Find any of the delimiters in String */
    while( *pos ) {
        idx = strnchr(v->str.text, v->str.len, *pos);    
        if ( idx ) break;
        pos++;
    }
    
    /* Any separator found ? */
    if (! *pos ) return false;
    
    size_t ofs = (size_t)(idx - v->str.text);
    /* numlength = length of string _behind_ separator */
    size_t numlength = v->str.len - ( ofs+1);

    /*reduce string length */
    v->str.len = ofs;

    /* convert numerical part and store as fontsize */
    v->font.fontsize = (uint8_t) CMD_to_number( idx+1, numlength);
    v->font.fontname = v->str;
    v->type = VAR_FONT;
    return true;
}


