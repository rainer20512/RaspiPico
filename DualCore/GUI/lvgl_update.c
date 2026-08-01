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

#include <stdlib.h>

#include "system/util.h"
#include "parser_specs.h"
#include "../GUI/gui_def.h"
#include "../GUI/variant.h"
#include "../GUI/gui_lists.h"

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

/******************************************************************************
 * @brief  We have al label with an indirect computed caption. This is done in
 *         the following way:
 *         1) computed caption = ind_value * 10^ind_scalefactor
 *         2) computed caption is printf-formatted with "ind_formatstr" and
 *            used as caption
 *         as we use no float values, negative scale factors are computed
 *         in two ints with integer part and fractional part
 *         Rules for "ind_formatstr":
 *         1) positive scale factors: we only have an integer part, this may
 *            be formatted generally as any integer format str in "printf", 
 *            in conjunction with all allowed modifiers
 *            Examples: %d, %3d, %+3d, %03d
 *         2) negative scale factors: we have a signed integer and and an unsigned
 *            frational part. Both of them have to be formatted with integer
 *            format strings. The decimal point also has to be specified in
 *            format str, the fractional part always should be formatted with
 *            leading zeroes
              Examples: %+d.%03u
 * @param  lbl    - ptr to associated existing label object in LVGL or NULL
 * @note   caller should assure, that ind_value" and "ind_formatstr" properties 
 *         are set
 * @note   no range checking is done with large scale factors!
 *****************************************************************************/     
static void label_compute_ind_caption ( lv_obj_t *lbl, int32_t ind_value, int8_t ind_scalefactor, char *ind_formatstr  )
{
    char caption[GUI_MAX_NAMELEN];    /* resulting output string */
    uint32_t frac; 
    int32_t  integer;
    int32_t  scaler;                  /* holds the scaling value 10^ind_scalefactor */
    bool     bDivide;                 /* true, if scale factor < 0 */

    bDivide = ind_scalefactor < 0;
    if ( bDivide ) {
      scaler  = ipow(10, ((int)ind_scalefactor * -1));
      frac    = ( abs(ind_value) ) % scaler;
      integer = ind_value / scaler;  
      snprintf(caption, GUI_MAX_NAMELEN, ind_formatstr, integer, frac);
    } else {
       scaler = ipow(10, (int)ind_scalefactor);
       frac   = 0;
       integer = ind_value * scaler;
       snprintf(caption, GUI_MAX_NAMELEN, ind_formatstr, integer);
    }
    
    lv_label_set_text(lbl, caption);

}

void LVGL_update_label_ind  ( lv_obj_t *lbl, uint8_t propidx, Variant_T *v, void *priv_data )
{
    /* on creation "priv_data" is NULL, in this case do nothing here */
    if ( !priv_data ) return;

    bool bSet = v != NULL && v->type != VAR_NULL;

    /* A label has private data elements, get access to them */
    GUI_LabelPrivate_T *priv = (GUI_LabelPrivate_T *)priv_data;

    switch(propidx) {
      /* the following are private data items */
      /* prop #4 - #7 */
      case LABEL_CURVAL:      if (bSet) { priv->ind_value       = V_Get_I32(v); LBLPRIV_SET_PROP(priv, LBLPRIV_CURVAL); }      else { LBLPRIV_CLR_PROP(priv, LBLPRIV_CURVAL); }      break;
      case LABEL_SCALEFACTOR: if (bSet) { priv->ind_scalefactor = V_Get_I8(v);  LBLPRIV_SET_PROP(priv, LBLPRIV_SCALEFACTOR); } else { LBLPRIV_CLR_PROP(priv, LBLPRIV_SCALEFACTOR); } break;
      case LABEL_FORMATSTR:   
          if (bSet) { 
              V_to_cstr(priv->ind_formatstr, v, ID_MAXNAMELEN);
              LBLPRIV_SET_PROP(priv, LBLPRIV_FORMATSTR); 
          } else { 
              LBLPRIV_CLR_PROP(priv, LBLPRIV_FORMATSTR); 
          } 
          break;
      case LABEL_CAPTION:   
          if (bSet) { 
              V_to_cstr(priv->ind_caption, v, ID_MAXNAMELEN);
              LBLPRIV_SET_PROP(priv, LBLPRIV_CAPTION); 
          } else { 
              LBLPRIV_CLR_PROP(priv, LBLPRIV_CAPTION); 
          } 
          break;
      default:
        #if DEBUG_GUIEDIT > 0 || DEBUG_DATAPOINTS > 0
            DEBUG_PRINTF("Err: No Update Fn for label prop #%d\n",propidx);
        #endif
    } /* switch */

    /*
     * The label caption is computed as follows:                      
     * - whenever ind_value and ind_formatstr are set, these two and ind_scale are used
     *   to compute and format the caption
     * - otherwise, if caption is set, this prop is used to set labels
     *   caption directly
     */
    if ( LBLPRIV_HAS_PROP(priv, LBLPRIV_CURVAL) && LBLPRIV_HAS_PROP(priv, LBLPRIV_FORMATSTR)) {
        label_compute_ind_caption( lbl, priv->ind_value, priv->ind_scalefactor, priv->ind_formatstr );
    } else {
        if ( LBLPRIV_HAS_PROP(priv, LBLPRIV_CAPTION))lv_label_set_text(lbl,priv->ind_caption);
    }
 }


void LVGL_update_label  ( lv_obj_t *lbl, uint8_t propidx, Variant_T *v, void *priv_data )
{
    bool bSet = v != NULL && v->type != VAR_NULL;
 
    switch(propidx) {
      /* prop #0 - #3*/
      case LABEL_STYLE:   if (bSet) lv_obj_add_style(lbl, V_Get_Ref(v), 0);                   break;
      case LABEL_ALIGN:   if (bSet) lv_obj_set_style_align(lbl, V_Get_U8(v), 0);              break;
      case LABEL_X0:      if (bSet) lv_obj_set_x(lbl, V_Get_U16(v));                          break;
      case LABEL_Y0:      if (bSet) lv_obj_set_y(lbl, V_Get_U16(v));                          break;
      case LABEL_PIVOTX:  if (bSet) lv_obj_set_style_transform_pivot_x(lbl, V_Get_U16(v),0);  break;
      case LABEL_PIVOTY:  if (bSet) lv_obj_set_style_transform_pivot_y(lbl, V_Get_U16(v),0);  break;
      case LABEL_ROTATE:  if (bSet) lv_obj_set_style_transform_rotation(lbl, V_Get_I16(v),0); break;

      /* the remaining properties belong to private data items or depend from them ( caption )*/
      default:
          LVGL_update_label_ind(lbl, propidx, v, priv_data );
                      
    } /* switch */
 }


void LVGL_update_arc    ( lv_obj_t *arc, uint8_t propidx, Variant_T *v )
{
    switch(propidx) {
/*  1 */
      case ARC_BGSTYLE:  lv_obj_add_style         (arc, V_Get_Ref(v), LV_PART_MAIN);      break;
      case ARC_INDSTYLE: lv_obj_add_style         (arc, V_Get_Ref(v), LV_PART_INDICATOR); break;
      case ARC_X0:       lv_obj_set_x             (arc, V_Get_U16(v));                    break;
      case ARC_Y0:       lv_obj_set_y             (arc, V_Get_U16(v));                    break; 
      case ARC_ROTATE:   lv_arc_set_rotation      (arc, V_Get_U16(v));                    break;
/*  6 */
      case ARC_BGSTART:  lv_arc_set_bg_start_angle(arc, V_Get_I16(v));                    break;
      case ARC_BGEND:    lv_arc_set_bg_end_angle  (arc, V_Get_I16(v));                    break;
      case ARC_MINVAL:   lv_arc_set_min_value     (arc, V_Get_I16(v));                    break;
      case ARC_MAXVAL:   lv_arc_set_max_value     (arc, V_Get_I16(v));                    break;
      case ARC_CURVAL:   lv_arc_set_value         (arc, V_Get_I16(v));                    break;
      default:
        #if DEBUG_GUIEDIT > 0 || DEBUG_DATAPOINTS > 0
            DEBUG_PRINTF("Err: No Update Fn for arc prop #%d\n",propidx);
        #endif
    } /* switch */
}


void LVGL_update_scale_ind  ( lv_obj_t *scale, uint8_t propidx, Variant_T *v, void *priv_data )
{
    /* on creation "priv_data" is NULL, in this case do nothing here */
    if ( !priv_data ) return;

    bool bSet = v != NULL && v->type != VAR_NULL;

    /* A scale has private data elements, get access to them */
    GUI_ScalePrivate_T *priv = (GUI_ScalePrivate_T *)priv_data;

    switch(propidx) {
      /* the following are private data items */
      case SCALE_CURVAL:  if (bSet) { priv->prv_curval  = V_Get_I16(v); SCAPRIV_SET_PROP(priv, SCAPRIV_CURVAL);  } else { SCAPRIV_CLR_PROP(priv, SCAPRIV_CURVAL);  } break;
      case SCALE_MYIMAGE: if (bSet) { priv->prv_myimage = V_Get_Ref(v); SCAPRIV_SET_PROP(priv, SCAPRIV_MYIMAGE); } else { SCAPRIV_CLR_PROP(priv, SCAPRIV_MYIMAGE); } break;
      case SCALE_MYLABEL: if (bSet) { priv->prv_mylabel = V_Get_Ref(v); SCAPRIV_SET_PROP(priv, SCAPRIV_MYLABEL); } else { SCAPRIV_CLR_PROP(priv, SCAPRIV_MYLABEL); } break;
      default:
        #if DEBUG_GUIEDIT > 0 || DEBUG_DATAPOINTS > 0
            DEBUG_PRINTF("Err: No Update Fn for scale prop #%d\n",propidx);
        #endif
    } /* switch */
 
 
    if ( SCAPRIV_HAS_PROP(priv, SCAPRIV_CURVAL) ) {
        /* update associated image */
        if ( SCAPRIV_HAS_PROP(priv, SCAPRIV_MYIMAGE) ) {
            lv_scale_set_image_needle_value(scale, priv->prv_myimage, priv->prv_curval);
        }
        /* update associated label */
        if ( SCAPRIV_HAS_PROP(priv, SCAPRIV_MYLABEL) ) {
            /* Get the labels associated GUI-Element */ 
            List_Elem_T *ll = LL_find_by_type_n_obj  ( GUI_item_list, GUI_ELEM_LABEL, priv->prv_mylabel );
            if ( ll ) {
                V_Set_I32(v,priv->prv_curval);
                LVGL_update_label_ind  ( priv->prv_mylabel,  LABEL_CURVAL, v, ll->private_data );
            } else {
                #if DEBUG_GUIEDIT > 0 || DEBUG_DATAPOINTS > 0
                    DEBUG_PRINTF("Err: Found no associated label for scale!\n");
                #endif
            }
        }
    }
}

void LVGL_update_scale  ( lv_obj_t *scale, uint8_t propidx, Variant_T *v, void *priv_data )
{
    switch(propidx) {
      /* 1 */
      case SCALE_MAINSTYLE:      lv_obj_add_style             (scale, V_Get_Ref(v), LV_PART_MAIN );     break;
      case SCALE_MAJORSTYLE:     lv_obj_add_style             (scale, V_Get_Ref(v), LV_PART_INDICATOR); break;
      case SCALE_MINORSTYLE:     lv_obj_add_style             (scale, V_Get_Ref(v), LV_PART_ITEMS);     break;
      case SCALE_MODE:           lv_scale_set_mode            (scale, V_Get_U8(v));   break;
      case SCALE_X0:             lv_obj_set_x                 (scale, V_Get_I16(v));  break;  
      /* 6 */
      case SCALE_Y0:             lv_obj_set_y                 (scale, V_Get_I16(v));  break;
      case SCALE_MINVAL:         lv_scale_set_min_value       (scale, V_Get_I16(v));  break;
      case SCALE_MAXVAL:         lv_scale_set_max_value       (scale, V_Get_I16(v));  break;
      case SCALE_TOTAL_TICKS:    lv_scale_set_total_tick_count(scale, V_Get_U16(v));  break;
      case SCALE_MAJ_TICK_DIST:  lv_scale_set_major_tick_every(scale, V_Get_U16(v));  break;
      /* 11 */
      case SCALE_SHOWLABEL:      lv_scale_set_label_show      (scale, V_Get_U8(v));   break;
      case SCALE_ANGLE_RANGE:    lv_scale_set_angle_range     (scale, V_Get_U16(v));  break;
      case SCALE_ROTATE:         lv_scale_set_rotation        (scale, V_Get_U16(v));  break;
      default:
         LVGL_update_scale_ind(scale, propidx, v, priv_data );
    } /* switch */
}

void LVGL_update_image  ( lv_obj_t *obj, uint8_t propidx, Variant_T *newval )
{
}



void LVGL_update        ( lv_obj_t *obj, uint8_t propidx, Variant_T *newval, GUI_Edit_Enum elemtype )
{
  List_Elem_T *ll;

  switch(elemtype) {
    case GUI_ELEM_SCREEN: LVGL_update_screen  (obj, propidx, newval); break;
    case GUI_ELEM_STYLE:  LVGL_update_style   ((lv_style_t*)obj, propidx, newval); break;
    case GUI_ELEM_LABEL:  
      /* A label has private data elements, get access to it */
      ll = LL_find_by_type_n_obj(GUI_ITEM_LIST, elemtype, obj);
      #if DEBUG_GUIEDIT > 0 || DEBUG_DATAPOINTS > 0
          if ( !ll) DEBUG_PUTS("Err: No private data for label");
      #endif
      LVGL_update_label   (obj, propidx, newval, ll ? ll->private_data : NULL ); 
      break;
    case GUI_ELEM_ARC:    LVGL_update_arc     (obj, propidx, newval); break;
    case GUI_ELEM_SCALE: 
      /* A scale has private data elements, get access to it */
      ll = LL_find_by_type_n_obj(GUI_ITEM_LIST, elemtype, obj);
      #if DEBUG_GUIEDIT > 0 || DEBUG_DATAPOINTS > 0
          if ( !ll) DEBUG_PUTS("Err: No private data for scale");
      #endif
      LVGL_update_scale(obj, propidx, newval, ll ? ll->private_data : NULL ); 
      break;
    case GUI_ELEM_IMAGE:  LVGL_update_image   (obj, propidx, newval); break;
    default:
        #if DEBUG_GUIEDIT > 0 || DEBUG_DATAPOINTS > 0
            DEBUG_PRINTF("Err: No Update Fn for GUI element %s\n",Editinfo[elemtype].name);
        #endif
  } /* switch */             
}
#endif /* USE_LVGL > 0 */