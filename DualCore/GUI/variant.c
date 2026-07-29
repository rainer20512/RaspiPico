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

/******************************************************************************
 * @brief converts the number representation in Variant string to 
 *        variant I32 representation. 
 * @retval - true, if initial variant type is STR, false, if not
 * @note  The conversion is done in place within variant !
 *****************************************************************************/
bool V_Str_to_I32(Variant_T *v)
{
    bool ret = v->type == VAR_STRING;
    if (ret) V_Set_I32(v, CMD_to_number( v->str.text, v->str.len ));
    return ret;
}

/******************************************************************************
 * @brief returns true, if Variant is of type string and is numeric, ie optional 
 *        sign, optional hex prefix "0x" followed by digits only
 * @retval       - true, if numeric, false if not 
 *****************************************************************************/
bool V_Str_is_numeric (Variant_T *v)
{
    return v->type == VAR_STRING && CMD_is_numeric(v->str.text, v->str.len);
}

/******************************************************************************
 * @brief  if variant string is numeric, the numerical value will be returned
 *         numbers have an optional sign, followed by and optional hex prefix,
 *         followed by digits only
 *         sign, optional hex prefix "0x" followed by digits only
 * @retval numerical value of variant string or 0 if not a number
 * @note   use "V_Str_is_numeric" to check, whether variant string is nzmeric
 * @note   maximum range is int32_t range !
 * @note   variant type and string is not changed!
 *****************************************************************************/
uint32_t V_Str_get_int32 (Variant_T *v)
{
    if ( V_Str_is_numeric(v) ) 
      return CMD_to_number(v->str.text, v->str.len);
    else
      return 0;
}

/******************************************************************************
 * @brief  if variant is of type string, search for char "sep" in string and
 *         return its offset withing strind (range 0 .. len(str)-1)
 * @retval offset of "sep" within "str" 
 *         or -1, if V is not of type string or if sep is not found
 * @note   variant type and string is not changed!
 *****************************************************************************/
int V_Str_chr_pos(Variant_T *v, const char sep)
{
    if ( v->type != VAR_STRING )  return -1;

    /* Search for sep within str vector */
    char *idx = strnchr(v->str.text, v->str.len, sep);    
    if ( idx ) {
        return idx - v->str.text;
    } else {
        /* Not found */
        return -1;
    }
}

/******************************************************************************
 * @brief  remove one character from within variant string
 * @param  v   - variant variable
 * @param  ofs - index of char to be removed  [0 .. len-1]
 * @retval offset of "sep" within "str" 
 *         or -1, if V is not of type string or if sep is not found
 * @note   variant type and string is not changed!
 *****************************************************************************/
int  V_Str_rm_char(Variant_T *v, size_t ofs)
{
    /* Check type and validity oc char to remove */
    if ( v->type != VAR_STRING )  return -1;
    if ( ofs >= v->str.len )      return -1;  

    /* special case: last char to be removed ? */
    if ( ofs == v->str.len-1 )    return --v->str.len;

    size_t copylen = v->str.len-1-ofs;
    /* memmove will handle overlap correctly */
    memmove(v->str.text+ofs, v->str.text+ofs+1, copylen);

    return --v->str.len;  
}