#include "../GUI/gui_def.h"
#if USE_LVGL > 0

#include <stdio.h>
#include <malloc.h>

GUI_Font_T AllFonts[] =
{
  {"Montserrat14", &lv_font_montserrat_14 },

  // {"Adlam24",      &rb_font_adlam_24 },
  // {"Adlam40",      &rb_font_adlam_40 },
  {"Adlam60",      &rb_font_adlam_60 },
  {"Adlam80",      &rb_font_adlam_80 },
  {"Adlam100",     &rb_font_adlam_100 },
  {"Adlam120",     &rb_font_adlam_120 },

  // {"Stencil24",      &w_font_stencil_24 },
  // {"Stencil40",      &w_font_stencil_40 },
  {"Stencil60",      &w_font_stencil_60 },
  {"Stencil80",      &w_font_stencil_80 },
  {"Stencil100",     &w_font_stencil_100 },
  {"Stencil120",     &w_font_stencil_120 },

  {"BahnSbC24",      &w_font_bahn_sbc_24 },
  {"BahnSbC40",      &w_font_bahn_sbc_40},
  {"BahnSbC60",      &w_font_bahn_sbc_60 },
  {"BahnSbC80",      &w_font_bahn_sbc_80 },
  {"BahnSbC100",     &w_font_bahn_sbc_100 },
  {"BahnSbC120",     &w_font_bahn_sbc_120},

  {NULL, NULL },  /* Mandatory list Terminator */
};

const GUI_Style_T def_style = 
    { .used         = 0b11111111101011111,
      .def_width    = 120, 
      .def_height   = 20, 
      .textalign    = LV_TEXT_ALIGN_CENTER,  
      .objalign     = LV_ALIGN_CENTER,
      .bgopa        = 128, 
      .borderwidth  = 0, 
      .borderradius = 5, 
      .shadow_width = 0, 
      .shadow_opa   = 128, 
      .sh_x         = 4, 
      .sh_y         = 4,   
      .bgcolor      = {0xC0, 0xC0, 0xC0},
      .bordercolor  = {0x00, 0x00, 0x00},
      .textcolor    = {0xff, 0x00, 0x00}, 
      .textfont     = &w_font_bahn_sbc_40,
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
    .caption = "-3,5",
    .name    = "Label01",
};

GUI_Label_T cur_label = def_label;

const GUI_Arc_T def_arc = {
    .used     = 0b11111111100,
    .bgstyle  = NULL,
    .indstyle = NULL,
    .x0       = 0, 
    .y0       = 0, 
    .rotation = 135,
    .bg_start = 0,
    .bg_end   = 270,
    .minval   = 0,
    .maxval   = 100,
    .curval   = 50,
    .name     = "Arc01",
};

GUI_Arc_T cur_arc = def_arc;

/* user friendly names of these of GUI elements */
const char *EditNames[]  = GUI_EDITNAMES;



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

/******************************************************************************
 * @brief Create a new LVGL obj from heap space
 * @param obj_in - if NULL, a new object will be created. Not NULL: No action
 * @param size   - size of element to be created
 * @retval returns the ptr to the new obj, when created or the original ptr,
 *         in case of "obj_in" was not NULL
 * @note   user is repsonsible for freeing if no longer needed
 *****************************************************************************/     
static void *GUI_Check_for_new ( void *obj_in, size_t size ) {
	/* Check, whether obj has been allocated already */
	if ( !obj_in ) { 
      obj_in = malloc (size);
      if ( !obj_in ) {
        puts("malloc of GUI obj failed");
      }
    }
    return obj_in;
}

/******************************************************************************
 * @brief Create a new LVGL style from GUI_Style_T variable
 *        the lvgl style variabel is dynamically allocated from heap
 *        user is repsonsible for freeing if no longer needed
 *****************************************************************************/     
lv_style_t * GUI_new_or_update_style ( GUI_Style_T *act, lv_style_t *style )
{

	/* Check, whether style has been allocated already */
    if ( !style ) {
      if ( !(style = GUI_Check_for_new( style, sizeof(lv_style_t))) ) return NULL;
      lv_style_init(style);
    }

	/* assign _All_ style properties */
    if ( STYLE_HAS_PROP(act, STYLE_BGCOLOR))      lv_style_set_bg_color         (style, act->bgcolor);      else lv_style_remove_prop(style, LV_STYLE_BG_COLOR);
    if ( STYLE_HAS_PROP(act, STYLE_BGOPA))        lv_style_set_bg_opa           (style, act->bgopa);        else lv_style_remove_prop(style, LV_STYLE_BG_OPA);
    if ( STYLE_HAS_PROP(act, STYLE_TEXTCOLOR))    lv_style_set_text_color       (style, act->textcolor);    else lv_style_remove_prop(style, LV_STYLE_TEXT_COLOR);
    if ( STYLE_HAS_PROP(act, STYLE_TEXTFONT))     lv_style_set_text_font        (style, act->textfont);     else lv_style_remove_prop(style, LV_STYLE_TEXT_FONT);
	if ( STYLE_HAS_PROP(act, STYLE_DEFWIDTH))     lv_style_set_width            (style, act->def_width);    else lv_style_remove_prop(style, LV_STYLE_WIDTH);
	if ( STYLE_HAS_PROP(act, STYLE_DEFHEIGHT))    lv_style_set_height           (style, act->def_height);   else lv_style_remove_prop(style, LV_STYLE_HEIGHT);
    if ( STYLE_HAS_PROP(act, STYLE_BORDERCOLOR))  lv_style_set_border_color     (style, act->bordercolor);  else lv_style_remove_prop(style, LV_STYLE_BORDER_COLOR);
    if ( STYLE_HAS_PROP(act, STYLE_BORDERWIDTH))  lv_style_set_border_width     (style, act->borderwidth);  else lv_style_remove_prop(style, LV_STYLE_BG_COLOR);
    if ( STYLE_HAS_PROP(act, STYLE_BORDERRADIUS)) lv_style_set_radius           (style, act->borderradius); else lv_style_remove_prop(style, LV_STYLE_RADIUS);
    if ( STYLE_HAS_PROP(act, STYLE_SHADOWCOLOR))  lv_style_set_shadow_color     (style, act->shadowcolor);  else lv_style_remove_prop(style, LV_STYLE_SHADOW_COLOR);
    if ( STYLE_HAS_PROP(act, STYLE_SHADOWXREF))   lv_style_set_shadow_offset_x  (style, act->sh_x);         else lv_style_remove_prop(style, LV_STYLE_SHADOW_OFS_X);
    if ( STYLE_HAS_PROP(act, STYLE_SHADOWYREF))   lv_style_set_shadow_offset_y  (style, act->sh_y);         else lv_style_remove_prop(style, LV_STYLE_SHADOW_OFS_Y);
    if ( STYLE_HAS_PROP(act, STYLE_SHADOWWIDTH))  lv_style_set_shadow_width     (style, act->shadow_width); else lv_style_remove_prop(style, LV_STYLE_SHADOW_WIDTH);
    if ( STYLE_HAS_PROP(act, STYLE_SHADOWOPA))    lv_style_set_shadow_opa       (style, act->shadow_opa);   else lv_style_remove_prop(style, LV_STYLE_SHADOW_OPA);
    if ( STYLE_HAS_PROP(act, STYLE_TEXTALIGN))    lv_style_set_text_align	    (style, act->textalign);    else lv_style_remove_prop(style, LV_STYLE_TEXT_ALIGN);
    if ( STYLE_HAS_PROP(act, STYLE_OBJALIGN))     lv_style_set_align	        (style, act->objalign);     else lv_style_remove_prop(style, LV_STYLE_ALIGN);
    /* update using widgets about style change */
    lv_obj_report_style_change( style);
	return style;	
}

/******************************************************************************
 * @brief Create a new LVGL style from GUI_Style_T variable
 *        the lvgl style variabel is dynamically allocated from heap
 *        user is repsonsible for freeing if no longer needed
 *****************************************************************************/     
lv_obj_t * GUI_new_or_update_label ( GUI_Label_T *act, lv_obj_t *lbl)
{
	/* Check, whether label has been allocated already */
	if ( !lbl ) { 
      if ( !(lbl = GUI_Check_for_new( lbl, sizeof(lv_obj_t*))) ) return NULL;
      lbl = lv_label_create(lv_screen_active());
    }
  
    if ( LABEL_HAS_PROP(act, LABEL_STYLE))  lv_obj_add_style(lbl, act->style, 0);
    if ( LABEL_HAS_PROP(act, LABEL_CAPTION))lv_label_set_text(lbl,act->caption);
    if ( LABEL_HAS_PROP(act, LABEL_ALIGN))  lv_obj_set_style_align(lbl, act->align, 0);
    if ( LABEL_HAS_PROP(act, LABEL_X0))     lv_obj_set_x(lbl, act->x0);
    if ( LABEL_HAS_PROP(act, LABEL_Y0))     lv_obj_set_y(lbl, act->y0); 
    GUI_dump_coords(lbl);

    return lbl;
}


/******************************************************************************
 * @brief Create a new Arc from GUI_Arc_T variable
 *        the lvgl object is dynamically allocated from heap
 *        user is repsonsible for freeing if no longer needed
 *****************************************************************************/     
lv_obj_t* GUI_new_or_update_arc ( GUI_Arc_T *act, lv_obj_t *arc)
{
	/* Check, whether arc has been allocated already */
	if ( !arc ) { 
      if ( !(arc = GUI_Check_for_new( arc, sizeof(lv_obj_t*))) ) return NULL;
      /* Create arc, not clickable, no knob */
      arc = lv_arc_create(lv_screen_active());
      lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_remove_style(arc, NULL, LV_PART_KNOB);   
    }

    if ( ARC_HAS_PROP(act, ARC_BGSTYLE))  lv_obj_add_style(arc, act->bgstyle, LV_PART_MAIN);
    if ( ARC_HAS_PROP(act, ARC_INDSTYLE)) lv_obj_add_style(arc, act->indstyle, LV_PART_INDICATOR);
    if ( ARC_HAS_PROP(act, ARC_X0))       lv_obj_set_x(arc, act->x0);
    if ( ARC_HAS_PROP(act, ARC_Y0))       lv_obj_set_y(arc, act->y0); 
    if ( ARC_HAS_PROP(act, ARC_ROTATE))   lv_arc_set_rotation(arc, act->rotation);
    if ( ARC_HAS_PROP(act, ARC_BGSTART))  lv_arc_set_bg_start_angle(arc, act->bg_start);
    if ( ARC_HAS_PROP(act, ARC_BGEND))    lv_arc_set_bg_end_angle(arc, act->bg_end);
    if ( ARC_HAS_PROP(act, ARC_MINVAL))   lv_arc_set_min_value(arc, act->minval);
    if ( ARC_HAS_PROP(act, ARC_MAXVAL))   lv_arc_set_max_value(arc, act->maxval);
    if ( ARC_HAS_PROP(act, ARC_CURVAL))   lv_arc_set_value(arc, act->curval);
     
    GUI_dump_coords(arc);

    return arc;

}

/******************************************************************************
 * @brief Shortcut to only update arc value
 *****************************************************************************/     
void GUI_Arc_set_value  ( lv_obj_t * arc, int16_t newval )
{
  lv_arc_set_value(arc, newval);
}
  

#endif /* USE_LVGL > 0 */ 

