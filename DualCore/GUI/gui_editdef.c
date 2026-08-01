#include "config/config.h"
#if USE_GUI_INTERFACE > 0
#include <string.h>
#include "debug/debug_helper.h"

#include "parser_specs.h"
#include "../GUI/gui_def.h"
#include "../GUI/gui_edit.h"


/*** forward declatations *****************************************************/
void DumpToken        (const char* token, uint32_t tokenlength); 

/* Edit receipe for screen structure */
const  GUI_Edit_T edit_screen = {
  .count         = SCREEN_EDIT_MAX,
  .gui_elem_type = GUI_ELEM_SCREEN,
  /* the foloowing two items are used to find used bits and name when */
  /* structure will be converted to raw data in "gui_edit"            */
  .used_ofs      = offsetof(GUI_Screen_T, used),
  .name_ofs      = offsetof(GUI_Screen_T, name),
  .total_size    = sizeof  (GUI_Screen_T),
  .priv_datasize = 0,   /* No private data */
  .workspace     = (uint8_t*)&cur_screen,
  /* Element Order has to be the same as in corresponding "used"-bit set !!! */
  .receipe   = {  
/*01*/
    { SCREEN_ROTATE,      "rotate",       GUI_UINT16, offsetof(GUI_Screen_T, rotation) }, 
    { SCREEN_BGOPA,       "bgopaqe",      GUI_UINT8,  offsetof(GUI_Screen_T, bgopa) }, 
    { SCREEN_BGCOLOR,     "bgcolor",      GUI_RGB888, offsetof(GUI_Screen_T, bgcolor) }, 
    { SCREEN_BGMAINOPA,    "bgmainopa",   GUI_UINT8,  offsetof(GUI_Screen_T, bgmainopa) }, 
    { SCREEN_BGGRDCOLOR,   "bggradcolor", GUI_RGB888, offsetof(GUI_Screen_T, bggradcolor) }, 
/*06*/
    { SCREEN_BGGRADOPA,    "bggradopa",   GUI_UINT8,  offsetof(GUI_Screen_T, bggradopa) }, 
    { SCREEN_BGGRADDIR,    "bggraddir",   GUI_UINT8,  offsetof(GUI_Screen_T, bggraddir) }, 
    { SCREEN_BGMAINSTOP,   "bgmainstop",  GUI_UINT8,  offsetof(GUI_Screen_T, bgmainstop) }, 
    { SCREEN_BGGRADSTOP,   "bggradstop",  GUI_UINT8,  offsetof(GUI_Screen_T, bggradstop) }, 
    { SCREEN_RESET,        "reset",       GUI_UINT8,  offsetof(GUI_Screen_T, resetoninit) }, 
/*11*/
    { SCREEN_NAME,          "name",       GUI_STRING, offsetof(GUI_Screen_T, name) }, 
  },
};



/* Edit receipe for Style structure */
const  GUI_Edit_T edit_style = {
  .count         = STYLE_EDIT_MAX,
  .gui_elem_type = GUI_ELEM_STYLE,
  /* the foloowing two items are used to find used bits and name when */
  /* structure will be converted to raw data in "gui_edit"            */
  .used_ofs      = offsetof(GUI_Style_T, used),
  .name_ofs      = offsetof(GUI_Style_T, name),
  .total_size    = sizeof  (GUI_Style_T),
  .priv_datasize = 0,   /* No private data */
  .workspace     = (uint8_t*)&cur_style,
  /* Element Order has to be the same as in corresponding "used"-bit set !!! */
  .receipe   = {  
/*01*/
    { STYLE_WIDTH,        "width",        GUI_UINT16, offsetof(GUI_Style_T, def_width) },
    { STYLE_HEIGHT,       "height",       GUI_UINT16, offsetof(GUI_Style_T, def_height) }, 
    { STYLE_LENGTH,       "length",       GUI_UINT16, offsetof(GUI_Style_T, def_length) }, 
    { STYLE_OBJALIGN,     "objectalign",  GUI_UINT8,  offsetof(GUI_Style_T, objalign) }, 
    { STYLE_BGOPA,        "bgopaqe",      GUI_UINT8,  offsetof(GUI_Style_T, bgopa) }, 
/*06*/
    { STYLE_BGCOLOR,      "bgcolor",      GUI_RGB888, offsetof(GUI_Style_T, bgcolor) }, 
    { STYLE_BGMAINOPA,    "bgmainopa",    GUI_UINT8,  offsetof(GUI_Style_T, bgmainopa) }, 
    { STYLE_BGGRDCOLOR,   "bggradcolor",  GUI_RGB888, offsetof(GUI_Style_T, bggradcolor) }, 
    { STYLE_BGGRADOPA,    "bggradopa",    GUI_UINT8,  offsetof(GUI_Style_T, bggradopa) }, 
    { STYLE_BGGRADDIR,    "bggraddir",    GUI_UINT8,  offsetof(GUI_Style_T, bggraddir) }, 
/*11*/
    { STYLE_BGMAINSTOP,   "bgmainstop",   GUI_UINT8,  offsetof(GUI_Style_T, bgmainstop) }, 
    { STYLE_BGGRADSTOP,   "bggradstop",   GUI_UINT8,  offsetof(GUI_Style_T, bggradstop) }, 
    { STYLE_BORDERWIDTH,  "borderwidth",  GUI_UINT8,  offsetof(GUI_Style_T, borderwidth) }, 
    { STYLE_BORDERRADIUS, "borderradius", GUI_UINT8,  offsetof(GUI_Style_T, borderradius) }, 
    { STYLE_BORDERCOLOR,  "bordercolor",  GUI_RGB888, offsetof(GUI_Style_T, bordercolor) }, 
/*16*/
    { STYLE_SHADOWWIDTH,  "shadowwidth",  GUI_UINT8,  offsetof(GUI_Style_T, shadow_width) }, 
    { STYLE_SHADOWOPA,    "shadowopaqe",  GUI_UINT8,  offsetof(GUI_Style_T, shadow_opa) }, 
    { STYLE_SHADOWXREF,   "shadowxref",   GUI_UINT8,  offsetof(GUI_Style_T, sh_x) }, 
    { STYLE_SHADOWYREF,   "shadowyref",   GUI_UINT8,  offsetof(GUI_Style_T, sh_y) }, 
    { STYLE_SHADOWCOLOR,  "shadowColor",  GUI_RGB888, offsetof(GUI_Style_T, shadowcolor) }, 
/*21*/
    { STYLE_TEXTALIGN,    "textalign",    GUI_UINT8,  offsetof(GUI_Style_T, textalign) }, 
    { STYLE_TEXTCOLOR,    "textcolor",    GUI_RGB888, offsetof(GUI_Style_T, textcolor) }, 
    { STYLE_TEXTFONT,     "textfont",     GUI_FONT,   offsetof(GUI_Style_T, textfont) }, 
    { STYLE_ARCWIDTH,     "arcwidth",     GUI_UINT8,  offsetof(GUI_Style_T, arcwidth) }, 
    { STYLE_ARCOPA,       "arcopaque",    GUI_UINT8,  offsetof(GUI_Style_T, arcopa) }, 
/*26*/
    { STYLE_ARCCOLOR,     "arccolor",     GUI_RGB888, offsetof(GUI_Style_T, arccolor) }, 
    { STYLE_LINEWIDTH,    "linewidth",    GUI_UINT8,  offsetof(GUI_Style_T, linewidth) },     
    { STYLE_LINECOLOR,    "linecolor",    GUI_RGB888, offsetof(GUI_Style_T, linecolor) },     
    { STYLE_LINEWIDTH,    "lineopaque",   GUI_UINT8,  offsetof(GUI_Style_T, lineopa) },     
    { STYLE_NAME,         "name",         GUI_STRING, offsetof(GUI_Style_T, name) }, 
  },
};

const  GUI_Edit_T edit_label = {
  .count         = LABEL_EDIT_MAX,
  .gui_elem_type = GUI_ELEM_LABEL,
  .used_ofs      = offsetof(GUI_Label_T, used),
  .name_ofs      = offsetof(GUI_Label_T, name),
  .total_size    = sizeof  (GUI_Label_T),
  .priv_datasize = sizeof  (GUI_LabelPrivate_T),  /* A Label has private data */
  .workspace     = (uint8_t*)&cur_label,
  /* Element Order has to be the same as in corresponding "used"-bit set !!! */
  .receipe   = { 
/*01*/
    { LABEL_STYLE,        "style",        GUI_STYLE,  offsetof(GUI_Label_T, style) }, 
    { LABEL_ALIGN,        "align",        GUI_UINT8,  offsetof(GUI_Label_T, align) }, 
    { LABEL_X0,           "x",            GUI_UINT16, offsetof(GUI_Label_T, x0) }, 
    { LABEL_Y0,           "y",            GUI_UINT16, offsetof(GUI_Label_T, y0) }, 
    { LABEL_CAPTION,      "caption",      GUI_STRING, offsetof(GUI_Label_T, caption) }, 
/*06*/
    { LABEL_PIVOTX,       "pivotx",       GUI_UINT16, offsetof(GUI_Label_T, pivotx) }, 
    { LABEL_PIVOTY,       "pivoty",       GUI_UINT16, offsetof(GUI_Label_T, pivoty) }, 
    { LABEL_ROTATE,       "rotate",       GUI_INT16,  offsetof(GUI_Label_T, rot_angle) }, 
    { LABEL_CURVAL,       "curval",       GUI_INT32,  offsetof(GUI_Label_T, ind_value) }, 
    { LABEL_SCALEFACTOR,  "scaler",       GUI_INT8,   offsetof(GUI_Label_T, ind_scalefactor) }, 
    { LABEL_FORMATSTR,    "formatstr",    GUI_STRING, offsetof(GUI_Label_T, ind_formatstr) }, 
    { LABEL_NAME,         "name",         GUI_STRING, offsetof(GUI_Label_T, name) }, 
  },
};

const  GUI_Edit_T edit_arc = {
  .count         = ARC_EDIT_MAX,
  .gui_elem_type = GUI_ELEM_ARC,
  .used_ofs      = offsetof(GUI_Arc_T, used),
  .name_ofs      = offsetof(GUI_Arc_T, name),
  .total_size    = sizeof  (GUI_Arc_T),
  .priv_datasize = 0,   /* No private data */
  .workspace     = (uint8_t*)&cur_arc,
  /* Element Order has to be the same as in corresponding "used"-bit set !!! */
  .receipe   = { 
/*01*/
    { ARC_BGSTYLE,      "bgstyle",        GUI_STYLE,  offsetof(GUI_Arc_T, bgstyle) }, 
    { ARC_INDSTYLE,     "indicatorstyle", GUI_STYLE,  offsetof(GUI_Arc_T, indstyle) }, 
    { ARC_X0,           "x",              GUI_UINT16, offsetof(GUI_Arc_T, x0) }, 
    { ARC_Y0,           "y",              GUI_UINT16, offsetof(GUI_Arc_T, y0) }, 
    { ARC_ROTATE,       "rotation",       GUI_UINT16, offsetof(GUI_Arc_T, rotation) }, 
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

const  GUI_Edit_T edit_scale = {
  .count         = SCALE_EDIT_MAX,
  .gui_elem_type = GUI_ELEM_SCALE,
  .used_ofs      = offsetof(GUI_Scale_T, used),
  .name_ofs      = offsetof(GUI_Scale_T, name),
  .total_size    = sizeof  (GUI_Scale_T),
  .priv_datasize = sizeof  (GUI_ScalePrivate_T), /* A scale has private data */
  .workspace     = (uint8_t*)&cur_scale,
  /* Element Order has to be the same as in corresponding "used"-bit set !!! */
  .receipe   = { 
/*01*/
    { SCALE_MAINSTYLE,    "mainstyle",      GUI_STYLE,  offsetof(GUI_Scale_T, mainstyle) }, 
    { SCALE_MAJORSTYLE,   "majorstyle",     GUI_STYLE,  offsetof(GUI_Scale_T, majorstyle) }, 
    { SCALE_MINORSTYLE,   "minorstyle",     GUI_STYLE,  offsetof(GUI_Scale_T, minorstyle) }, 
    { SCALE_MODE,         "mode",           GUI_UINT8,  offsetof(GUI_Scale_T, scalemode) },
    { SCALE_TOTAL_TICKS,  "totalticks",     GUI_UINT16, offsetof(GUI_Scale_T, totalticks) },
/* 06 */
    { SCALE_MAJ_TICK_DIST,"tickdistance",   GUI_UINT16, offsetof(GUI_Scale_T, tickdistance) },
    { SCALE_SHOWLABEL,    "showlabel",      GUI_UINT8,  offsetof(GUI_Scale_T, bLabelShow) },
    { SCALE_MINVAL,       "minval",         GUI_INT16,  offsetof(GUI_Scale_T, minval) }, 
    { SCALE_MAXVAL,       "maxval",         GUI_INT16,  offsetof(GUI_Scale_T, maxval) }, 
    { SCALE_CURVAL,       "curval",         GUI_INT16,  offsetof(GUI_Scale_T, curval) }, 
/* 11 */
    { SCALE_ANGLE_RANGE,  "anglerange",     GUI_UINT16, offsetof(GUI_Scale_T, angle_range) },
    { SCALE_ROTATE,       "rotation",       GUI_UINT16, offsetof(GUI_Scale_T, rotation) }, 
    { SCALE_X0,           "x",              GUI_UINT16, offsetof(GUI_Scale_T, x0) }, 
    { SCALE_Y0,           "y",              GUI_UINT16, offsetof(GUI_Scale_T, y0) }, 
    { SCALE_MYIMAGE,      "myimage",        GUI_IMAGE,  offsetof(GUI_Scale_T, myimage) }, 
/* 16 */
    { SCALE_MYLABEL,      "mylabel",        GUI_LABEL,  offsetof(GUI_Scale_T, mylabel) }, 
    { SCALE_NAME,         "name",           GUI_STRING, offsetof(GUI_Scale_T, name) }, 
  },
};

const  GUI_Edit_T edit_image = {
  .count         = IMAGE_EDIT_MAX,
  .gui_elem_type = GUI_ELEM_IMAGE,
  .used_ofs      = offsetof(GUI_Image_T, used),
  .name_ofs      = offsetof(GUI_Image_T, name),
  .total_size    = sizeof  (GUI_Image_T),
  .priv_datasize = 0,   /* No private data */
  .workspace     = (uint8_t*)&cur_image,
  /* Element Order has to be the same as in corresponding "used"-bit set !!! */
  .receipe   = { 
/* 01 */
    { IMAGE_IMAGE,        "rawimage",       GUI_RAWIMG, offsetof(GUI_Image_T, image) }, 
    { IMAGE_IMAGE,        "style",          GUI_STYLE,  offsetof(GUI_Image_T, style) }, 
    { IMAGE_XOFS,         "xofs",           GUI_INT16,  offsetof(GUI_Image_T, xofs) }, 
    { IMAGE_YOFS,         "yofs",           GUI_INT16,  offsetof(GUI_Image_T, yofs) }, 
    { IMAGE_ALIGN,        "align",          GUI_UINT8,  offsetof(GUI_Image_T, align) }, 
    { IMAGE_ROTATE,       "rotation",       GUI_INT16,  offsetof(GUI_Image_T, rot_angle) }, 
/* 06 */
    { IMAGE_SCALE,        "scale",          GUI_UINT16, offsetof(GUI_Image_T, scale) }, 
    { IMAGE_PIVOTX,       "pivotx",         GUI_INT16,  offsetof(GUI_Image_T, pivotx) }, 
    { IMAGE_PIVOTY,       "pivoty",         GUI_INT16,  offsetof(GUI_Image_T, pivoty) }, 
    { IMAGE_NAME,         "name",           GUI_STRING, offsetof(GUI_Image_T, name) }, 
  },
};

const  GUI_Edit_T edit_datapoint = {
  .count         = DP_EDIT_MAX,
  .gui_elem_type = GUI_ELEM_DATAPOINT,
  .used_ofs      = offsetof(GUI_Datapoint_T, used),
  .name_ofs      = offsetof(GUI_Datapoint_T, dpname),
  .total_size    = sizeof  (GUI_Datapoint_T),
  .priv_datasize = 0,   /* No private data */
  .workspace     = (uint8_t*)&cur_dp,
  /* Element Order has to be the same as in corresponding "used"-bit set !!! */
  .receipe   = { 
/* 01 */
    { DP_NAME,            "name",           GUI_STRING, offsetof(GUI_Datapoint_T, dpname) }, 
    { DP_GUIELEMNAME,     "ref",            GUI_STRING, offsetof(GUI_Datapoint_T, guielemname) }, 
    { DP_PROPERTYNAME,    "property",       GUI_STRING, offsetof(GUI_Datapoint_T, propertyname) }, 
  },
};



/* one triple of GUI_Edit_Enum or ElementName -> GUI_Edit_T  */
struct receipes_S {
  GUI_Edit_Enum     gui_type;
  const GUI_Edit_T* gui_edit;
  const char        name[ID_MAXNAMELEN];
};

/* complete list of Gui_elem_t -> Gui_Edit_T */
static const struct receipes_S gui_to_receipe[GUI_ELEM_MAX] = {
    { GUI_ELEM_NOTYPE,    NULL,             NULL },
    { GUI_ELEM_RAWIMG,    NULL,             NULL},
    { GUI_ELEM_FONT,      NULL,             NULL},
    { GUI_ELEM_SCREEN,    &edit_screen,     SCREEN_IDSTR },
    { GUI_ELEM_STYLE,     &edit_style,      STYLE_IDSTR },
    { GUI_ELEM_LABEL,     &edit_label,      LABEL_IDSTR },
    { GUI_ELEM_ARC,       &edit_arc,        ARC_IDSTR,   },
    { GUI_ELEM_SCALE,     &edit_scale,      SCALE_IDSTR,   },
    { GUI_ELEM_IMAGE,     &edit_image,      IMAGE_IDSTR,   },
    { GUI_ELEM_DATAPOINT, &edit_datapoint,  DATAPOINT_IDSTR, },
};

/******************************************************************************
 * @brief  Get the edit receipe for a GUI element idetifiey by GUI Type ID
 * @param  gui_type  - GUI element type ID for what the receipe is requested
 * @retval ptr to the receipe or NULL, if not found
 ******************************************************************************/
const GUI_Edit_T *Find_EditInfoByType( GUI_Edit_Enum gui_type )
{
    for ( uint32_t i=0; i <GUI_ELEM_MAX; i++ ) {
        if ( gui_to_receipe[i].gui_type == gui_type ) return gui_to_receipe[i].gui_edit;
    }
    #if DEBUG_GUIEDIT
        DEBUG_PRINTF("ErrNo Receipe for GUI Element ""%s""\n",Editinfo[gui_type].name);
    #endif
    return NULL;
}

/******************************************************************************
 * @brief  Find the edit receipe for an GUI element identified by name    
 * @param  name  - GUI element name for what the receipe is requested
 * @param  namelen - length of namestr
 * @param  editdata - edit receipe for raw data
 * @retval ptr to the receipe or NULL, if not found
 ******************************************************************************/
const GUI_Edit_T *FindEditInfoByName( const char *name, const size_t namelen)
{
    for ( uint32_t i=0; i <GUI_ELEM_MAX; i++ ) {
        if ( strncmp(gui_to_receipe[i].name,name, namelen) == 0 ) 
            return gui_to_receipe[i].gui_edit;
    }
    #if DEBUG_GUIEDIT > 0
        DEBUG_PUTS("ErrNo Receipe for GUI Element ");
        DumpToken(name, namelen);
        DEBUG_PUTC('\n');
    #endif
    return NULL;
}

/******************************************************************************
 * @brief  Find the  index for one property of an gui edit description 
*          by the name of that property
 * @param  edit  - complete GUI element edit description
 * @param  name/namelen  - property name to find
 * @retval index of the property in "edit" (0 ...n) or -1 if not found
 * @note   no case conversion, match only if identical
 ******************************************************************************/
int32_t GetReceipeIdxByName( const GUI_Edit_T *edit, const char *name, const size_t namelen)
{
    for ( uint32_t i = 0; i < edit->count; i++ ) {
        if ( strncmp(edit->receipe[i].elem_name, name, namelen) == 0 ) {
            return i;
        }
    }

    return -1;
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