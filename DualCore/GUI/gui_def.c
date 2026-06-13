#include "../GUI/gui_def.h"
#if USE_LVGL > 0

#include <stdio.h>
#include <malloc.h>

const GUI_Style_T def_style = 
    { .used         = 0x7FFF,
      .def_width    = 120, 
      .def_height   = 20, 
      .align        = LV_ALIGN_TOP_MID,  
      .bgopa        = 128, 
      .borderwidth  = 2, 
      .borderradius = 5, 
      .shadow_width = 4, 
      .shadow_opa   = 128, 
      .sh_x         = 4, 
      .sh_y         = 4,   
      .bgcolor      = {0x40, 0x40, 0x40},
      .bordercolor  = {0x00, 0x00, 0x00},
      .txtcolor     = {0xff, 0x00, 0x00},  
      .shadowcolor  = {0x80, 0x80, 0x80},
      .name         = "Predef01",
    };

GUI_Style_T cur_style = def_style;

const GUI_Label_T def_label =
  { .used    = 0b11110,
    .style   = NULL,
    .align   = LV_ALIGN_CENTER,
    .x0      = 0,
    .y0      = 0,
    .caption = "Label01",
    .name    = "Label01",
};

GUI_Label_T cur_label = def_label;

/* user friendly names of these of GUI elements */
const char *EditNames[]  = GUI_EDITNAMES;

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
    if ( STYLE_HAS_PROP(act, STYLE_BGCOLOR))      lv_style_set_bg_color         (style, act->bgcolor);
    if ( STYLE_HAS_PROP(act, STYLE_BGOPA))        lv_style_set_bg_opa           (style, act->bgopa);
    if ( STYLE_HAS_PROP(act, STYLE_TXTCOLOR))     lv_style_set_text_color       (style, act->txtcolor);
	if ( STYLE_HAS_PROP(act, STYLE_DEFWIDTH))     lv_style_set_width            (style, act->def_width);
	if ( STYLE_HAS_PROP(act, STYLE_DEFHEIGHT))    lv_style_set_height           (style, act->def_height);
    if ( STYLE_HAS_PROP(act, STYLE_BORDERCOLOR))  lv_style_set_border_color     (style, act->bordercolor);
    if ( STYLE_HAS_PROP(act, STYLE_BORDERWIDTH))  lv_style_set_border_width     (style, act->borderwidth);
    if ( STYLE_HAS_PROP(act, STYLE_BORDERRADIUS)) lv_style_set_radius           (style, act->borderradius);
    if ( STYLE_HAS_PROP(act, STYLE_SHADOWCOLOR))  lv_style_set_shadow_color     (style, act->shadowcolor);
    if ( STYLE_HAS_PROP(act, STYLE_SHADOWXREF))   lv_style_set_shadow_offset_x  (style, act->sh_x);
    if ( STYLE_HAS_PROP(act, STYLE_SHADOWYREF))   lv_style_set_shadow_offset_y  (style, act->sh_y);
    if ( STYLE_HAS_PROP(act, STYLE_SHADOWWIDTH))  lv_style_set_shadow_width     (style, act->shadow_width);
    if ( STYLE_HAS_PROP(act, STYLE_SHADOWOPA))    lv_style_set_shadow_opa       (style, act->shadow_opa);
    if ( STYLE_HAS_PROP(act, STYLE_ALIGN))        lv_style_set_text_align	    (style, act->align);
    	
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
  
    if ( LABEL_HAS_PROP(act, LABEL_STYLE))  lv_obj_add_style(lbl, style, 0);
    if ( LABEL_HAS_PROP(act, LABEL_CAPTION))lv_label_set_text(lbl,act->caption);
    if ( LABEL_HAS_PROP(act, LABEL_ALIGN))  lv_obj_set_style_align(lbl, act->align, 0);
    if ( LABEL_HAS_PROP(act, LABEL_X0))     lv_obj_set_x(lbl, act->x0);
    if ( LABEL_HAS_PROP(act, LABEL_Y0))     lv_obj_set_y(lbl, act->y0); 

    return lbl;
}

void GUI_dump_coords ( lv_obj_t * obj )
{
  lv_area_t coords;
  if ( !obj ) {
    printf("Dump Coords: No Obj!\n");
  } else {
    lv_obj_update_layout(obj);
    lv_obj_get_coords(obj, &coords);
    int32_t x = lv_obj_get_x(obj);
    int32_t y = lv_obj_get_y(obj);
    printf ("Obj coords=(%d,%d), (%d,%d)\n", x, y, x+lv_obj_get_width(obj),y+lv_obj_get_height(obj) );
  }
}
#endif /* USE_LVGL > 0 */ 

