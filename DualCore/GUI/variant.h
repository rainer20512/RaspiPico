
/**
 ******************************************************************************
 * @file    variant.h
 * @author  Rainer
 * @brief   Interface for a "variant" datatype implementation
 *
 ******************************************************************************
 *
 ******************************************************************************
 */
#ifndef _VARIANT_H_
#define _VARIANT_H_
#include "config/config.h"

#if USE_LVGL > 0
  #include "../../lvgl/lvgl.h"
#endif

#include <string.h>   /* Due to use of "strlen" in macro */

/*************************************************************************************************************
 * Codes for possible variant Datatypes 
 *************************************************************************************************************/
typedef enum {
  VAR_UNSET   = 0, 
  VAR_UINT8   = 1,
  VAR_UINT16  = 2,
  VAR_RGB888  = 3,
  VAR_ARGB32  = 4,
  VAR_UINT32  = 5,
  VAR_INT8    = 6,
  VAR_INT16   = 7,
  VAR_INT32   = 8,
  VAR_STRING  = 9,
  VAR_STYLE   = 10,
  VAR_FONT    = 11,
  VAR_REF     = 12, 
  VAR_NULL    = 13,
  VAR_MAXELEM = 14,
} Variant_E;

/*************************************************************************************************************
 * Definition of String datatype with <ptr to char vector>/<length>
 *************************************************************************************************************/
typedef struct { 
    char *text;
    size_t len;
} String_T;

/* helper struct to define a string or fontname by char vector and length */ 
/* and fontsize (only for fonts  )*/
struct strfont { 
    String_T fontname;
    uint8_t fontsize;
};

/*************************************************************************************************************
 * Definition of variant 
 *************************************************************************************************************/
typedef struct {
      Variant_E       type;               /* actual datatype contained in union     */
      union {                             /* union vor all possible representations */  
          uint32_t        u32;
          uint32_t        u16[2];
          uint8_t         u8[4];
          int8_t          i8[4];
          int16_t         i16[2];
          int32_t         i32;
#if USE_LVGL > 0
          lv_color_t      rgb888;
          lv_color32_t    argb;
#endif
          String_T        str;
          struct strfont  font;
          void            *ref;
      };
} Variant_T;

static inline void          V_Init      ( Variant_T* v) { v->type = VAR_UNSET; };
/* there is _no_ typechecking when retrieving values, caller must ensure correct variant type ! */
static inline uint32_t      V_Get_U32   ( Variant_T* v) { return v->u32; };
static inline uint16_t      V_Get_U16   ( Variant_T* v) { return v->u16[0]; };
static inline uint8_t       V_Get_U8    ( Variant_T* v) { return v->u8[0]; };
static inline int32_t       V_Get_I32   ( Variant_T* v) { return v->i32; };
static inline int16_t       V_Get_I16   ( Variant_T* v) { return v->i16[0]; };
static inline int8_t        V_Get_I8    ( Variant_T* v) { return v->i8[0]; };
#if USE_LVGL > 0
static inline lv_color_t    V_Get_Rgb   ( Variant_T* v) { return v->rgb888; };
static inline lv_color32_t  V_Get_Argb   ( Variant_T* v) { return v->argb; };
#endif
static inline void *        V_Get_Ref   ( Variant_T* v) { return v->ref; };
static inline String_T      V_Get_Str   ( Variant_T* v) { return v->str; };
static inline void          V_Get_Font  ( Variant_T* v, String_T *fontstr, uint8_t *size ) { *fontstr = v->font.fontname; *size=v->font.fontsize; }

static inline uint32_t      V_Set_U32   ( Variant_T* v, uint32_t u32) { v->type=VAR_UINT32; return v->u32=u32; };
static inline uint16_t      V_Set_U16   ( Variant_T* v, uint16_t u16) { v->type=VAR_UINT16; return v->u16[0]=u16; };
static inline uint8_t       V_Set_U8    ( Variant_T* v, uint8_t  u8 ) { v->type=VAR_UINT8;  return v->u8[0]=u8; };
static inline int32_t       V_Set_I32   ( Variant_T* v, int32_t  i32) { v->type=VAR_INT32;  return v->i32=i32; };
static inline int16_t       V_Set_I16   ( Variant_T* v,  int16_t i16) { v->type=VAR_INT16;  return v->i16[0]=i16; };
static inline int8_t        V_Set_I8    ( Variant_T* v,  int8_t  i8 ) { v->type=VAR_INT8;   return v->i8[0]=i8; };
#if USE_LVGL > 0
static inline lv_color_t    V_Set_Rgb   ( Variant_T* v, lv_color_t rgb) { v->type=VAR_RGB888; v->rgb888=rgb; return v->rgb888; };
static inline lv_color32_t  V_Set_Argb  ( Variant_T* v, lv_color32_t argb) { v->type=VAR_ARGB32; v->argb=argb; return v->argb; };
#endif
static inline void *        V_Set_Ref   ( Variant_T* v, void     *ptr){ v->type=VAR_REF;    return v->ref=ptr; };
static inline void          V_Set_Null  ( Variant_T* v) { v->type=VAR_NULL; };
static inline String_T      V_Set_Str   ( Variant_T* v, char *text, size_t len) { v->type=VAR_STRING; v->str.text = text; v->str.len=len; return v->str; };
static inline void          V_Set_Font  ( Variant_T* v, String_T fontname, uint8_t fontsize ) { v->type=VAR_FONT; v->font.fontname = fontname; v->font.fontsize = fontsize; };

#define                     V_Set_CStr(v, str)   V_Set_Str(v, (str), strlen(str))

/* Get copy of String data ( VAR_STRING, VAR_FONT */
char     *V_to_cstr       (char *dest, Variant_T *v, size_t maxlen);
bool      V_Str_to_Font   (Variant_T *v); 
bool      V_Str_to_I32    (Variant_T *v); 
bool      V_Str_is_numeric(Variant_T *v);
uint32_t  V_Str_get_int32 (Variant_T *v);
int       V_Str_chr_pos   (Variant_T *v, const char sep);
int       V_Str_rm_char   (Variant_T *v, size_t ofs);
#endif /* _VARIANT_H_ */