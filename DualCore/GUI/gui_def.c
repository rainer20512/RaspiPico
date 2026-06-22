#include "config/config.h"

#if USE_GUI_INTERFACE > 0

#include "../GUI/gui_def.h"
#include "../GUI/gui_edit.h"
#include "../GUI/gui_lists.h"
#include "../GUI/gui_ops.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

const GUI_Style_T def_style = 
    { .used         = 0b10001111111101011111,
      .def_width    = 120, 
      .def_height   = 20, 
      .objalign     = LV_ALIGN_CENTER,
      .bgopa        = 128, 
      .bgcolor      = {0xC0, 0xC0, 0xC0},
      .borderwidth  = 0, 
      .borderradius = 5, 
      .bordercolor  = {0x00, 0x00, 0x00},
      .shadow_width = 0, 
      .shadow_opa   = 128, 
      .sh_x         = 4, 
      .sh_y         = 4,   
      .shadowcolor  = {0x80, 0x80, 0x80},
      .textalign    = LV_TEXT_ALIGN_CENTER,  
      .textcolor    = {0xff, 0x00, 0x00}, 
      .textfont     = &w_font_bahn_sbc_40,
      /* .arcwidth, .arcopa, .arccolor not set! */
      .name         = "Style01",
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


#if defined(RP2040_M0_1) || defined(CORE1_SIM)

/******************************************************************************
 * @brief Create a new LVGL obj from heap space
 * @param obj_in - if NULL, a new object will be created. Not NULL: No action
 * @param size   - size of element to be created
 * @retval returns the ptr to the new obj, when created or the original ptr,
 *         in case of "obj_in" was not NULL
 * @note   user is repsonsible for freeing if no longer needed
 *****************************************************************************/     
static void *GUI_Allocate ( void *obj_in, size_t size ) {
	/* Check, whether obj has been allocated already */
	if ( !obj_in ) { 
      obj_in = malloc (size);
      if ( !obj_in ) {
        puts("malloc of GUI obj failed");
      }
    }
    return obj_in;
}

#if USE_LVGL > 0


/******************************************************************************
 * @brief Debug function: print lower left and upper right coord of an 
 *        lvgl object
 * @param obj    - LVGL obj to dump
 *****************************************************************************/     
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
 * @brief  Create a new LVGL style or update an existing style 
 *         from GUI_Style_T variable. 
 *         if style is NULL, new LVGL style will be created, if != NULL it is
 *         regarded as a valid lvgl style object and updated accordingly  
 * @param  act    - GUI description of LVGL style
 * @param  style  - ptr to associated existing style object in LVGL or NULL
 * @retval pointer to new or updated LVGL style
 * @note   the lvgl style variable is dynamically allocated from heap
 *         user is repsonsible for freeing if no longer needed
 *****************************************************************************/     
static lv_style_t * GUI_new_or_update_style ( GUI_Style_T *act, lv_style_t *style )
{ 
	/* Check, whether style is already known to LVGL */
	/* if not, allocate space and init */
    if ( !style ) {
      if ( !(style = GUI_Allocate( style, sizeof(lv_style_t))) ) return NULL;
      lv_style_init(style);
    } else {
    }

	/* assign _All_ style properties */
	if ( STYLE_HAS_PROP(act, STYLE_DEFWIDTH))     lv_style_set_width            (style, act->def_width);    else lv_style_remove_prop(style, LV_STYLE_WIDTH);
	if ( STYLE_HAS_PROP(act, STYLE_DEFHEIGHT))    lv_style_set_height           (style, act->def_height);   else lv_style_remove_prop(style, LV_STYLE_HEIGHT);
    if ( STYLE_HAS_PROP(act, STYLE_OBJALIGN))     lv_style_set_align	        (style, act->objalign);     else lv_style_remove_prop(style, LV_STYLE_ALIGN);
    if ( STYLE_HAS_PROP(act, STYLE_BGOPA))        lv_style_set_bg_opa           (style, act->bgopa);        else lv_style_remove_prop(style, LV_STYLE_BG_OPA);
    if ( STYLE_HAS_PROP(act, STYLE_BGCOLOR))      lv_style_set_bg_color         (style, act->bgcolor);      else lv_style_remove_prop(style, LV_STYLE_BG_COLOR);
    if ( STYLE_HAS_PROP(act, STYLE_BORDERWIDTH))  lv_style_set_border_width     (style, act->borderwidth);  else lv_style_remove_prop(style, LV_STYLE_BG_COLOR);
    if ( STYLE_HAS_PROP(act, STYLE_BORDERRADIUS)) lv_style_set_radius           (style, act->borderradius); else lv_style_remove_prop(style, LV_STYLE_RADIUS);
    if ( STYLE_HAS_PROP(act, STYLE_BORDERCOLOR))  lv_style_set_border_color     (style, act->bordercolor);  else lv_style_remove_prop(style, LV_STYLE_BORDER_COLOR);
    if ( STYLE_HAS_PROP(act, STYLE_SHADOWXREF))   lv_style_set_shadow_offset_x  (style, act->sh_x);         else lv_style_remove_prop(style, LV_STYLE_SHADOW_OFS_X);
    if ( STYLE_HAS_PROP(act, STYLE_SHADOWYREF))   lv_style_set_shadow_offset_y  (style, act->sh_y);         else lv_style_remove_prop(style, LV_STYLE_SHADOW_OFS_Y);
    if ( STYLE_HAS_PROP(act, STYLE_SHADOWWIDTH))  lv_style_set_shadow_width     (style, act->shadow_width); else lv_style_remove_prop(style, LV_STYLE_SHADOW_WIDTH);
    if ( STYLE_HAS_PROP(act, STYLE_SHADOWOPA))    lv_style_set_shadow_opa       (style, act->shadow_opa);   else lv_style_remove_prop(style, LV_STYLE_SHADOW_OPA);
    if ( STYLE_HAS_PROP(act, STYLE_SHADOWCOLOR))  lv_style_set_shadow_color     (style, act->shadowcolor);  else lv_style_remove_prop(style, LV_STYLE_SHADOW_COLOR);
    if ( STYLE_HAS_PROP(act, STYLE_TEXTALIGN))    lv_style_set_text_align	    (style, act->textalign);    else lv_style_remove_prop(style, LV_STYLE_TEXT_ALIGN);
    if ( STYLE_HAS_PROP(act, STYLE_TEXTCOLOR))    lv_style_set_text_color       (style, act->textcolor);    else lv_style_remove_prop(style, LV_STYLE_TEXT_COLOR);
    if ( STYLE_HAS_PROP(act, STYLE_TEXTFONT))     lv_style_set_text_font        (style, act->textfont);     else lv_style_remove_prop(style, LV_STYLE_TEXT_FONT);
    if ( STYLE_HAS_PROP(act, STYLE_ARCWIDTH))     lv_style_set_arc_width        (style, act->arcwidth);     else lv_style_remove_prop(style, LV_STYLE_ARC_WIDTH);
    if ( STYLE_HAS_PROP(act, STYLE_ARCOPA))       lv_style_set_arc_opa          (style, act->arcopa);       else lv_style_remove_prop(style, LV_STYLE_ARC_OPA);
    if ( STYLE_HAS_PROP(act, STYLE_ARCCOLOR))     lv_style_set_arc_color        (style, act->arccolor);     else lv_style_remove_prop(style, LV_STYLE_ARC_COLOR);

    /* update using widgets about style change */
    lv_obj_report_style_change(style);
	return style;	
}

/******************************************************************************
 * @brief  Create a new LVGL label or update an existing label 
 *         from GUI_Labele_T variable. 
 *         if lbl IS NULL, new LVGL object will be created, if != NULL it is
 *         regarded as a valid lvgl label object and updated accordingly  
 * @param  act    - GUI description of LVGL label
 * @param  lbl    - ptr to associated existing label object in LVGL or NULL
 * @retval pointer to new or updated LVGL label
 * @note   the lvgl object variable is dynamically allocated from heap
 *         user is repsonsible for freeing if no longer needed
 *****************************************************************************/     
static lv_obj_t * GUI_new_or_update_label ( GUI_Label_T *act, lv_obj_t *lbl  )
{
	/* Check, whether label is already known to LVGL */
	/* if not found, allocate space and init */
	if ( !lbl ) { 
      if ( !(lbl = GUI_Allocate( lbl, sizeof(lv_obj_t*))) ) return NULL;
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
 * @brief  Create a new LVGL arc or update an existing arc 
 *         from GUI_Arc_T variable. 
 *         if arc IS NULL, new LVGL object will be created, if != NULL it is
 *         regarded as a valid lvgl arc object and updated accordingly  
 * @param  act    - GUI description of LVGL Arc
 * @param  arc    - ptr to associated existing arc object in LVGL or NULL
 * @retval pointer to new or updated LVGL arc
 * @note   the lvgl object variable is dynamically allocated from heap
 *         user is repsonsible for freeing if no longer needed
 *****************************************************************************/     
static lv_obj_t* GUI_new_or_update_arc ( GUI_Arc_T *act, lv_obj_t *arc )
{
	/* Check, whether ARC is already created in LVGL */
	if ( !arc ) { 
      if ( !(arc = GUI_Allocate( arc, sizeof(lv_obj_t*))) ) return NULL;
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

/******************************************************************************
 * @brief Load all known fonts _once_ into GUI elem list, 
 * @note  Core1 implementation
 *****************************************************************************/     
void GUI_Init_Fonts_Core1(void)
{
  List_Elem_T *font;

  /* Iterate thru all defined fonts and insert them into global item list */
  /* List of defined fonts _MUST BE_ terminated by NULL,NULL */
  for ( uint32_t i = 0; i < AllFontNum1; i++ ) {
     font = LL_New_Element(GUI_ELEM_FONT,  (void *)AllFonts1[i].font, AllFonts1[i].fontname, &AllFonts1[i], AllFonts1[i].fontsize);
     LL_append(&GUI_item_list, font);
     i++;
  }
  printf("%d fonts loaded\n", AllFontNum1);
}

/******************************************************************************
 * @brief  Create a new or update a LVGL element from GUI-Element
 * @param  data     - raw GUI element data
 * @param  editdata - edit receipe for raw data
 * @retval untyped ptr to updated or new created LVGL obj or NULL, if creation failed
 ******************************************************************************/
void *GUI_Create_or_update_LVGL(uint8_t *data, const GUI_Edit_T *editdata, void *lvgl_obj )
{
    /* We have different handlers, depending from GUI element data type */
    switch(editdata->gui_elem_type) {
      case GUI_ELEM_NOTYPE:
        printf("Err: Cannot create untyped LVGL object\n");
        break;
      case GUI_ELEM_STYLE:
        return  GUI_new_or_update_style ( (GUI_Style_T *)data, (lv_style_t *)lvgl_obj );
        break;
      case GUI_ELEM_LABEL:
        return  GUI_new_or_update_label ( (GUI_Label_T *)data, (lv_obj_t *)lvgl_obj );
        break;
      case GUI_ELEM_ARC:
        return  GUI_new_or_update_arc ( (GUI_Arc_T *)data, (lv_obj_t *)lvgl_obj );
        break;
      default:
        printf("Err: No LVGL Update handler for LVGL %s\n", EditNames[editdata->gui_elem_type]);
    }
}



/******************************************************************************
 * @brief  update an existing GUI-Element in global GUI element list
 *         and update the corresponging LVGL object
 * @param  ll_elem  - ptr to exisiting GUI element list entry
 * @param  data     - raw changed element data to be updated
 * @param  editdata - edit receipe for raw data
 ******************************************************************************/
static void GUI_update_entry(List_Elem_T *ll_elem, uint8_t *data, const GUI_Edit_T *editdata )
{
    /* overwrite the complete GUI element data structure */
    uint8_t *dest = ll_elem->ll_entry;
    memcpy_fast(dest, data, editdata->total_size);

    /* Update associated LVGL obj */
    ll_elem->ll_lvgl_obj = GUI_Create_or_update_LVGL( data, editdata, ll_elem->ll_lvgl_obj );

    printf("%s %s updated\n",EditNames[editdata->gui_elem_type],ll_elem->ll_name);
}

/******************************************************************************
 * @brief  Store a completely new GUI-Element into global GUI element list
 *         and create the corresponding LVGL object
 * @param  data     - raw element data
 * @param  editdata - edit receipe for raw data
 ******************************************************************************/
static void GUI_create_entry(uint8_t *data, const GUI_Edit_T *editdata )
{
    /* create a full copy of actual data structure */
    uint8_t *copy = malloc(editdata->total_size);
    if ( !copy ) {
      printf("malloc failed!\n");
      return;
    }
    memcpy_fast(copy, data, editdata->total_size);

  /* Create associated LVGL obj */
  void *lvgl_obj = GUI_Create_or_update_LVGL( data, editdata, NULL );

  List_Elem_T *new;
  /* find position of "name" field in raw data */
  char *name = (char *)(copy + editdata->name_ofs);
  /* In case of fonts: also get the fontsize and store as additional item */
  uint32_t additional = ( editdata->gui_elem_type == GUI_ELEM_FONT ? ((GUI_Font_T*)data)->fontsize: 0); 
    
  new = LL_New_Element( editdata->gui_elem_type, lvgl_obj, name, copy, additional );
  LL_append(&GUI_item_list, new );
  printf("%s %s created\n",EditNames[editdata->gui_elem_type],name);
}

/******************************************************************************
 * @brief  Store a completely new GUI-Element into global GUI element list
 *         _AND_ LVGL    -or-
 *         Update an existing GUI-Element in Global GUI element list
 *         _AND_ LVGL
 *         When only using this function to store/update, it is guaranteed, that
 *         both lists are consistent.
 *         A GUI element is defined by type and name, or, in case of Fonts, by
 *         type, name and fontsize
 * @param  data     - raw element data
 * @param  gui_elem - GUI element Type
 * @retval List_Elem_T ptr to the new or updated element or NULL in case of Error
 ******************************************************************************/
List_Elem_T *GUI_new_or_update_entry(uint8_t *data, GUI_Elem_T gui_elem )
{
  /* Find the edit receipe */
  const GUI_Edit_T *edit = GUI_edit_get_receipe_for_elemtype( gui_elem );
  if ( !edit ) return NULL;

  /* find position of "name" field in raw data */
  char *name = (char *)(data + edit->name_ofs);

  /* first try to find the element in list */
  List_Elem_T *ll_elem = LL_find_by_type_n_name (GUI_item_list, edit->gui_elem_type, name );  
  
  if ( ll_elem ) {
    /* Element already in list: Update GUI list entry and LVGL object*/
    GUI_update_entry(ll_elem, data, edit);
  } else {
    /* Element not in list: Create GUI list entry and LVGL obj */
    GUI_create_entry(data, edit);
  }
}



#endif /* USE_LVGL > 0 */ 
#endif /* if defined(RP2040_M0_1) || defined(CORE1_SIM) */
#endif /* USE_GUI_INTERFACE > 0 */ 

