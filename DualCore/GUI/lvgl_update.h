
/**
  ******************************************************************************
  * @file    lvgl_update.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _LVGL_UPDATE_H_
#define _LVGL_UPDATE_H_
#include "config/config.h"

void LVGL_update_screen     ( lv_obj_t *obj,   uint8_t propidx, Variant_T *newval );
void LVGL_update_style      ( lv_style_t *obj, uint8_t propidx, Variant_T *newval );
void LVGL_update_label      ( lv_obj_t *obj,   uint8_t propidx, Variant_T *newval, void *priv_data );
void LVGL_update_arc        ( lv_obj_t *obj,   uint8_t propidx, Variant_T *newval );
void LVGL_update_scale      ( lv_obj_t *obj,   uint8_t propidx, Variant_T *newval, void *priv_data );
void LVGL_update_scale_ind  ( lv_obj_t *scale, uint8_t propidx, Variant_T *v, void *priv_data );
void LVGL_update_image      ( lv_obj_t *obj,   uint8_t propidx, Variant_T *newval );

void LVGL_update            ( lv_obj_t *obj, uint8_t propidx, Variant_T *newval, GUI_Edit_Enum elemtype );             

#endif /* _LVGL_UPDATE_H_ */