/**
  ******************************************************************************
  * @file    lvgl_update.c
  * @author  Rainer
  * @brief   for GUI elements do the update of associated lvgl objects
  *          Because this routines are also used by datapoint updates, which
  *          onyl know GUI element type, property index and datatype, all updates 
  *          are done via packing the value in Variant_T variables and indexed by 
  *          property ID
  ******************************************************************************
  *
  ******************************************************************************
  */

#include "config/config.h"
#if USE_LVGL > 0

#include "../GUI/gui_def.h"
#include "../GUI/variant.h"

#include "debug/debug_helper.h"

/******************************************************************************
 * @brief  translate the allowed display rotation angles 0,90,180,270
 *         to corresponding LVGL constants
 * @retval true,if o, false if any other value than 0,90,180,270 is passed
 *****************************************************************************/     
static bool GUI_display_set_rotation ( uint16_t rotation )
{
    lv_display_rotation_t rot;
    switch(rotation) {
      case   0: rot = LV_DISPLAY_ROTATION_0;   break;
      case  90: rot = LV_DISPLAY_ROTATION_90;  break;
      case 180: rot = LV_DISPLAY_ROTATION_180; break;
      case 270: rot = LV_DISPLAY_ROTATION_270; break;
      default:
        #if DEBUG_GUIEDIT > 0 || DEBUG_DATAPOINTS > 0
            DEBUG_PRINTF("Err: _set_rotation: Illegal value %d\n", rotation);
        #endif
        return false;
    }

    lv_display_set_rotation       (NULL, rot);
    return true;
}


void LVGL_update_screen ( lv_obj_t *obj, uint8_t propidx, Variant_T *newval )
{
    switch(propidx) {
/*  1 */
      case SCREEN_ROTATE:     GUI_display_set_rotation      (V_Get_U16(newval));                    break;  
      case SCREEN_BGOPA:      lv_obj_set_style_bg_opa       (obj, V_Get_U8(newval),  LV_PART_MAIN); break;  
      case SCREEN_BGCOLOR:    lv_obj_set_style_bg_color     (obj, V_Get_Rgb(newval), LV_PART_MAIN); break;  
      case SCREEN_BGMAINOPA:  lv_obj_set_style_bg_main_opa  (obj, V_Get_U8(newval),  LV_PART_MAIN); break;  
      case SCREEN_BGGRDCOLOR: lv_obj_set_style_bg_grad_color(obj, V_Get_Rgb(newval), LV_PART_MAIN); break;  
/*  6 */        
      case SCREEN_BGGRADOPA:  lv_obj_set_style_bg_main_opa  (obj, V_Get_U8(newval),  LV_PART_MAIN); break;  
      case SCREEN_BGGRADDIR:  lv_obj_set_style_bg_grad_dir  (obj, V_Get_U8(newval),  LV_PART_MAIN); break;  
      case SCREEN_BGMAINSTOP: lv_obj_set_style_bg_main_stop (obj, V_Get_U8(newval),  LV_PART_MAIN); break;  
      case SCREEN_BGGRADSTOP: lv_obj_set_style_bg_grad_stop (obj, V_Get_U8(newval),  LV_PART_MAIN); break;  
      default:
        #if DEBUG_GUIEDIT > 0 || DEBUG_DATAPOINTS > 0
            DEBUG_PRINTF("Err: No Update Fn for screen prop #%d\n",propidx);
        #endif
    } /* switch */
}

void LVGL_update_style  ( lv_style_t *style, uint8_t propidx, Variant_T *v ) 
{
    bool bSet = v != NULL && v->type != VAR_NULL;
    switch(propidx) {

/*  1 */
      case STYLE_WIDTH:        if (bSet) lv_style_set_width            (style, V_Get_U16(v)); else lv_style_remove_prop(style, LV_STYLE_WIDTH);         break;
      case STYLE_HEIGHT:       if (bSet) lv_style_set_height           (style, V_Get_U16(v)); else lv_style_remove_prop(style, LV_STYLE_HEIGHT);        break;
      case STYLE_LENGTH:       if (bSet) lv_style_set_length           (style, V_Get_U16(v)); else lv_style_remove_prop(style, LV_STYLE_LENGTH);        break;
      case STYLE_OBJALIGN:     if (bSet) lv_style_set_align	           (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_ALIGN);         break;
      case STYLE_BGOPA:        if (bSet) lv_style_set_bg_opa           (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_BG_OPA);        break;
/*  6 */
      case STYLE_BGCOLOR:      if (bSet) lv_style_set_bg_color         (style, V_Get_Rgb(v)); else lv_style_remove_prop(style, LV_STYLE_BG_COLOR);      break;
      case STYLE_BGMAINOPA:    if (bSet) lv_style_set_bg_main_opa      (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_BG_MAIN_OPA);   break;
      case STYLE_BGGRDCOLOR:   if (bSet) lv_style_set_bg_grad_color    (style, V_Get_Rgb(v)); else lv_style_remove_prop(style, LV_STYLE_BG_GRAD_COLOR); break;
      case STYLE_BGGRADOPA:    if (bSet) lv_style_set_bg_main_opa      (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_BG_GRAD_OPA);   break;
      case STYLE_BGGRADDIR:    if (bSet) lv_style_set_bg_grad_dir      (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_BG_GRAD_DIR);   break;
/* 11 */
      case STYLE_BGMAINSTOP:   if (bSet) lv_style_set_bg_main_stop     (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_BG_MAIN_STOP);  break;
      case STYLE_BGGRADSTOP:   if (bSet) lv_style_set_bg_grad_stop     (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_BG_GRAD_STOP);  break;
      case STYLE_BORDERWIDTH:  if (bSet) lv_style_set_border_width     (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_BG_COLOR);      break;
      case STYLE_BORDERRADIUS: if (bSet) lv_style_set_radius           (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_RADIUS);        break;
      case STYLE_BORDERCOLOR:  if (bSet) lv_style_set_border_color     (style, V_Get_Rgb(v)); else lv_style_remove_prop(style, LV_STYLE_BORDER_COLOR);  break;
/* 16 */
      case STYLE_SHADOWXREF:   if (bSet) lv_style_set_shadow_offset_x  (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_SHADOW_OFS_X);  break;
      case STYLE_SHADOWYREF:   if (bSet) lv_style_set_shadow_offset_y  (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_SHADOW_OFS_Y);  break;
      case STYLE_SHADOWWIDTH:  if (bSet) lv_style_set_shadow_width     (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_SHADOW_WIDTH);  break;
      case STYLE_SHADOWOPA:    if (bSet) lv_style_set_shadow_opa       (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_SHADOW_OPA);    break;
      case STYLE_SHADOWCOLOR:  if (bSet) lv_style_set_shadow_color     (style, V_Get_Rgb(v)); else lv_style_remove_prop(style, LV_STYLE_SHADOW_COLOR);  break;
/* 21 */
      case STYLE_TEXTALIGN:    if (bSet) lv_style_set_text_align	   (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_TEXT_ALIGN);    break;
      case STYLE_TEXTCOLOR:    if (bSet) lv_style_set_text_color       (style, V_Get_Rgb(v)); else lv_style_remove_prop(style, LV_STYLE_TEXT_COLOR);    break;
      case STYLE_TEXTFONT:     if (bSet) lv_style_set_text_font        (style, V_Get_Ref(v)); else lv_style_remove_prop(style, LV_STYLE_TEXT_FONT);     break;
      case STYLE_ARCWIDTH:     if (bSet) lv_style_set_arc_width        (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_ARC_WIDTH);     break;
      case STYLE_ARCOPA:       if (bSet) lv_style_set_arc_opa          (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_ARC_OPA);       break;
/* 26 */
      case STYLE_ARCCOLOR:     if (bSet) lv_style_set_arc_color        (style, V_Get_Rgb(v)); else lv_style_remove_prop(style, LV_STYLE_ARC_COLOR);     break;
      case STYLE_LINEWIDTH:    if (bSet) lv_style_set_line_width       (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_LINE_WIDTH);    break;
      case STYLE_LINECOLOR:    if (bSet) lv_style_set_line_color       (style, V_Get_Rgb(v)); else lv_style_remove_prop(style, LV_STYLE_ARC_WIDTH);     break;
      case STYLE_LINEOPA:      if (bSet) lv_style_set_line_opa         (style, V_Get_U8(v));  else lv_style_remove_prop(style, LV_STYLE_ARC_OPA);       break;
      default:
        #if DEBUG_GUIEDIT > 0 || DEBUG_DATAPOINTS > 0
            DEBUG_PRINTF("Err: No Update Fn for style prop #%d\n",propidx);
        #endif
    } /* switch */

    /* update using widgets about style change */
    lv_obj_report_style_change(style);
}

void LVGL_update_label  ( lv_obj_t *obj, uint8_t propidx, Variant_T *newval )
{
}

void LVGL_update_arc    ( lv_obj_t *obj, uint8_t propidx, Variant_T *newval )
{
}

void LVGL_update_scale  ( lv_obj_t *obj, uint8_t propidx, Variant_T *newval )
{
}

void LVGL_update_image  ( lv_obj_t *obj, uint8_t propidx, Variant_T *newval )
{
}



void LVGL_update        ( lv_obj_t *obj, uint8_t propidx, Variant_T *newval, GUI_Edit_Enum elemtype )
{
  switch(elemtype) {
    case GUI_ELEM_SCREEN: LVGL_update_screen  (obj, propidx, newval); break;
    case GUI_ELEM_STYLE:  LVGL_update_style   ((lv_style_t*)obj, propidx, newval); break;
    case GUI_ELEM_LABEL:  LVGL_update_label   (obj, propidx, newval); break;
    case GUI_ELEM_ARC:    LVGL_update_arc     (obj, propidx, newval); break;
    case GUI_ELEM_SCALE:  LVGL_update_scale   (obj, propidx, newval); break;
    case GUI_ELEM_IMAGE:  LVGL_update_image   (obj, propidx, newval); break;
    default:
        #if DEBUG_GUIEDIT > 0 || DEBUG_DATAPOINTS > 0
            DEBUG_PRINTF("Err: No Update Fn for GUI element %s\n",Editinfo[elemtype].name);
        #endif
  } /* switch */             
}
#endif /* USE_LVGL > 0 */