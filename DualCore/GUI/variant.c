#include "debug/debug_helper.h"
#include "./variant.h"
#include <string.h>

/*************************************************************************************************************
 * @brief Copy string from variant type String or Font to destination, 0 terminated
 * @param v      - variant to copy from
 * @param dest   - destination to copy to, 
 * @param maxlen - maximal length that dest can take, including terminating \0
 * @retval       - ptr to destination
 *************************************************************************************************************/
char *V_to_cstr(Variant_T *v, char *dest, size_t maxlen)
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




