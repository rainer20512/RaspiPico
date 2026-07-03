#include "config/config.h"
#if USE_GUI_INTERFACE > 0
#include <string.h>
#include "debug/debug_helper.h"
	
#include "../GUI/gui_def.h"
#include "../GUI/gui_edit.h"



/* Edit receipe for Style structure */
const  GUI_Edit_T edit_style = {
  .count         = STYLE_EDIT_MAX,
  .gui_elem_type = GUI_ELEM_STYLE,
  /* the foloowing two items are used to find used bits and name when */
  /* structure will be converted to raw data in "gui_edit"            */
  .used_ofs      = offsetof(GUI_Style_T, used),
  .name_ofs      = offsetof(GUI_Style_T, name),
  .total_size    = sizeof  (GUI_Style_T),
  /* Element Order has to be the same as in corresponding "used"-bit set !!! */
  .receipe   = {  
/*01*/
    { STYLE_DEFWIDTH,     "width",        GUI_UINT16, offsetof(GUI_Style_T, def_width) },
    { STYLE_DEFHEIGHT,    "height",       GUI_UINT16, offsetof(GUI_Style_T, def_height) }, 
    { STYLE_OBJALIGN,     "objectalign",  GUI_UINT8,  offsetof(GUI_Style_T, objalign) }, 
    { STYLE_BGOPA,        "bgopaqe",      GUI_UINT8,  offsetof(GUI_Style_T, bgopa) }, 
    { STYLE_BGCOLOR,      "bgcolor",      GUI_RGB888, offsetof(GUI_Style_T, bgcolor) }, 
/*06*/
    { STYLE_BORDERWIDTH,  "borderwidth",  GUI_UINT8,  offsetof(GUI_Style_T, borderwidth) }, 
    { STYLE_BORDERRADIUS, "borderradius", GUI_UINT8,  offsetof(GUI_Style_T, borderradius) }, 
    { STYLE_BORDERCOLOR,  "bordercolor",  GUI_RGB888, offsetof(GUI_Style_T, bordercolor) }, 
    { STYLE_SHADOWWIDTH,  "shadowwidth",  GUI_UINT8,  offsetof(GUI_Style_T, shadow_width) }, 
    { STYLE_SHADOWOPA,    "shadowopaqe",  GUI_UINT8,  offsetof(GUI_Style_T, shadow_opa) }, 
/*11*/
    { STYLE_SHADOWXREF,   "shadowxref",   GUI_UINT8,  offsetof(GUI_Style_T, sh_x) }, 
    {  STYLE_SHADOWYREF,  "shadowyref",   GUI_UINT8,  offsetof(GUI_Style_T, sh_y) }, 
    { STYLE_SHADOWCOLOR,  "shadowColor",  GUI_RGB888, offsetof(GUI_Style_T, shadowcolor) }, 
    { STYLE_TEXTALIGN,    "textalign",    GUI_UINT8,  offsetof(GUI_Style_T, textalign) }, 
    { STYLE_TEXTCOLOR,    "textcolor",    GUI_RGB888, offsetof(GUI_Style_T, textcolor) }, 
/*16*/
    { STYLE_TEXTFONT,     "textfont",     GUI_FONT,   offsetof(GUI_Style_T, textfont) }, 
    { STYLE_ARCWIDTH,     "arcwidth",     GUI_UINT8,  offsetof(GUI_Style_T, arcwidth) }, 
    { STYLE_ARCOPA,       "arcopaque",    GUI_UINT8,  offsetof(GUI_Style_T, arcopa) }, 
    { STYLE_ARCCOLOR,     "arccolor",     GUI_RGB888, offsetof(GUI_Style_T, arccolor) }, 
    { STYLE_NAME,         "name",         GUI_STRING, offsetof(GUI_Style_T, name) }, 
/*21*/
  },
};

const  GUI_Edit_T edit_label = {
  .count         = LABEL_EDIT_MAX,
  .gui_elem_type = GUI_ELEM_LABEL,
  .used_ofs      = offsetof(GUI_Label_T, used),
  .name_ofs      = offsetof(GUI_Label_T, name),
  .total_size    = sizeof  (GUI_Label_T),
  /* Element Order has to be the same as in corresponding "used"-bit set !!! */
  .receipe   = { 
    { LABEL_STYLE,        "style",        GUI_STYLE,  offsetof(GUI_Label_T, style) }, 
    { LABEL_ALIGN,        "align",        GUI_UINT8,  offsetof(GUI_Label_T, align) }, 
    { LABEL_X0,           "x",            GUI_UINT16, offsetof(GUI_Label_T, x0) }, 
    { LABEL_Y0,           "y",            GUI_UINT16, offsetof(GUI_Label_T, y0) }, 
    { LABEL_CAPTION,      "caption",      GUI_STRING, offsetof(GUI_Label_T, caption) }, 
    { LABEL_NAME,         "name",         GUI_STRING, offsetof(GUI_Label_T, name) }, 
  },
};

const  GUI_Edit_T edit_arc = {
  .count         = ARC_EDIT_MAX,
  .gui_elem_type = GUI_ELEM_ARC,
  .used_ofs      = offsetof(GUI_Arc_T, used),
  .name_ofs      = offsetof(GUI_Arc_T, name),
  .total_size    = sizeof  (GUI_Arc_T),
  /* Element Order has to be the same as in corresponding "used"-bit set !!! */
  .receipe   = { 
/*01*/
    { ARC_BGSTYLE,      "bgstyle",        GUI_STYLE,  offsetof(GUI_Arc_T, bgstyle) }, 
    { ARC_INDSTYLE,     "indicatorstyle", GUI_STYLE,  offsetof(GUI_Arc_T, indstyle) }, 
    { ARC_X0,           "x",              GUI_UINT16, offsetof(GUI_Arc_T, x0) }, 
    { ARC_Y0,           "y",              GUI_UINT16, offsetof(GUI_Arc_T, y0) }, 
    { ARC_ROTATE,       "rotation",       GUI_INT16,  offsetof(GUI_Arc_T, rotation) }, 
/*06*/
    { ARC_BGSTART,      "bgstart",        GUI_INT16,  offsetof(GUI_Arc_T, bg_start) }, 
    { ARC_BGEND,        "bgend",          GUI_INT16,  offsetof(GUI_Arc_T, bg_end) }, 
    { ARC_MINVAL,       "minval",         GUI_INT16,  offsetof(GUI_Arc_T, minval) }, 
    { ARC_MAXVAL,       "maxval",         GUI_INT16,  offsetof(GUI_Arc_T, maxval) }, 
    { ARC_CURVAL,       "curval",         GUI_INT16,  offsetof(GUI_Arc_T, curval) }, 
/*11*/
    { ARC_NAME,         "name",           GUI_STRING, offsetof(GUI_Arc_T, name) }, 
  },
};

/* one pair of GUI_Edit_Enum -> GUI_Edit_T  */
struct receipes_S {
  GUI_Edit_Enum        gui_type;
  const GUI_Edit_T* gui_edit;
};

/* complete list of Gui_elem_t -> Gui_Edit_T */
static const struct receipes_S gui_to_receipe[GUI_ELEM_MAX] = {
    { GUI_ELEM_NOTYPE, NULL },
    { GUI_ELEM_FONT, NULL },
    { GUI_ELEM_STYLE, &edit_style },
    { GUI_ELEM_LABEL, &edit_label },
    { GUI_ELEM_ARC,   &edit_arc   },
};

/******************************************************************************
 * @brief  Get the edit receipe for a GUI element
 * @param  gui_type  - GUI element type for what the receipe is requested
 * @param  editdata - edit receipe for raw data
 * @retval ptr to the receipe or NULL, if not found
 ******************************************************************************/
const GUI_Edit_T *GUI_edit_get_receipe_for_elemtype( GUI_Edit_Enum gui_type )
{
    uint32_t limit = sizeof(gui_to_receipe)/sizeof(struct receipes_S);
    for ( uint32_t i=0; i <limit; i++ ) {
        if ( gui_to_receipe[i].gui_type == gui_type ) return gui_to_receipe[i].gui_edit;
    }
    #if DEBUG_GUIEDIT
        DEBUG_PRINTF("ErrNo Receipe for GUI Element ""%s""\n",EditNames[gui_type]);
    #endif
    return NULL;
}

/******************************************************************************
 * @brief  Find the edit receipe for one property of an gui edit description 
*          by the name of that property
 * @param  edit  - complete GUI element edit description
 * @param  name  - property name to find
 * @retval ptr to the single receipe or NULL, if not found
 * @note   no case conversion, match only if identical
 ******************************************************************************/
const Edit_Receipe_T *FindReceipeByName   ( const GUI_Edit_T *edit, const char *name)
{
    for ( uint32_t i = 0; i < edit->count; i++ ) {
        if ( strcmp(edit->receipe[i].elem_name, name) == 0 ) {
            return &edit->receipe[i];
        }
    }

    return NULL;
}

/******************************************************************************
 * @brief  Find the edit receipe for one property of an gui edit description 
*          by the name of that property
 * @param  edit  - complete GUI element edit description
 * @param  name  - property name to find
 * @retval ptr to the single receipe or NULL, if not found
 * @note   no case conversion, match only if identical
 ******************************************************************************/
const Edit_Receipe_T *FindReceipeByPropIdx( const GUI_Edit_T *edit, uint8_t prop_idx)
{
    for ( uint32_t i = 0; i < edit->count; i++ ) {
        if ( edit->receipe[i].propnum == prop_idx ) {
            return &edit->receipe[i];
        }
    }

    return NULL;
}


#endif /*USE_GUI_INTERFACE > 0 */ 