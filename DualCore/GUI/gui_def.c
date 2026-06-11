#include "../GUI/gui_def.h"
#if USE_LVGL > 0

#include <stdio.h>
#include <malloc.h>

const GUI_Style_T def_style = 
    { .def_width= 120, 
      .def_height = 20, 
      .align = LV_ALIGN_TOP_MID,  
      .bgopa=128, 
      .borderwidth=2, 
      .borderradius = 5, 
      .shadow_width=4, 
      .shadow_opa=128, 
      .sh_x=4, 
      .sh_y = 4,   
      .bgcolor    = {0x40, 0x40, 0x40},
      .bordercolor= {0x00, 0x00, 0x00},
      .txtcolor   = {0xff, 0x00, 0x00},  
      .shadowcolor= {0x80, 0x80, 0x80},
    };

GUI_Style_T cur_style = def_style;

const  GUI_Edit_T edit_style = {
  .count        = 14,
  .editname     = "Style",
  .gui_element  = { 
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
  },
};


const char lbltext[]  = "Def. Label";
const GUI_Label_T def_label =
  { .style = NULL,
    .align = LV_ALIGN_CENTER,
    .x0    = 120,
    .y0    = 120,
    .txt   = lbltext,
};

GUI_Label_T cur_label = def_label;

const  GUI_Edit_T edit_label = {
  .count        = 4,
  .editname     = "Label",
  .gui_element  = { 
    { "Align",        GUI_UINT8,  offsetof(GUI_Label_T, align) }, 
    { "X0",           GUI_UINT16, offsetof(GUI_Label_T, x0) }, 
    { "Y0",           GUI_UINT16, offsetof(GUI_Label_T, y0) }, 
    { "Text",         GUI_STRING, offsetof(GUI_Label_T, txt) }, 
  },
};

void gui_test2(void)
{
}

/******************************************************************************
 * @brief Create a new LVGL style from GUI_Style_T variable
 *        the lvgl style variabel is dynamically allocated from heap
 *        user is repsonsible for freeing if no longer needed
 *****************************************************************************/     
lv_style_t * gui_new_style ( GUI_Style_T *act, lv_style_t *style )
{

	/* Check, whether style has been allocated already */
	if ( !style ) { 
      style = malloc ( sizeof(lv_style_t) );
      if ( !style ) {
        puts("malloc of style failed");
        return NULL;
      }
      lv_style_init(style);
    }

	/* assign _All_ style properties */
    lv_style_set_bg_color		(style, act->bgcolor);
    lv_style_set_bg_opa			(style, act->bgopa);
    lv_style_set_text_color		(style, act->txtcolor);
	lv_style_set_width			(style, act->def_width);
	lv_style_set_height			(style, act->def_height);
    lv_style_set_border_color	(style, act->bordercolor);
    lv_style_set_border_width	(style, act->borderwidth);
    lv_style_set_radius			(style, act->borderradius);
    lv_style_set_shadow_color	(style, act->shadowcolor);
    lv_style_set_shadow_offset_x(style, act->sh_x);
    lv_style_set_shadow_offset_y(style, act->sh_y);
    lv_style_set_shadow_width	(style, act->shadow_width);
    lv_style_set_shadow_opa		(style, act->shadow_opa);
    lv_style_set_text_align	    (style, act->align);
    	
	return style;	
}

/******************************************************************************
 * @brief Create a new LVGL style from GUI_Style_T variable
 *        the lvgl style variabel is dynamically allocated from heap
 *        user is repsonsible for freeing if no longer needed
 *****************************************************************************/     
lv_obj_t * gui_new_label ( GUI_Label_T *act, lv_style_t *style, lv_obj_t *lbl)
{
	/* Check, whether label has been allocated already */
	if ( !lbl ) { 
        lbl = malloc ( sizeof(lv_obj_t*) );
        if ( !lbl ) {
            puts("malloc of label failed");
            return NULL;
        }
        lbl = lv_label_create(lv_screen_active());
    }
  
    if ( style) lv_obj_add_style(lbl, style, 0);
    lv_label_set_text(lbl,act->txt);
    lv_obj_align(lbl, act->align, act->x0, act->y0);


}


#endif /* USE_LVGL > 0 */ 

