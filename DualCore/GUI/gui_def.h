
/**
  ******************************************************************************
  * @file    gui_def.h
  * @author  Rainer
  * @brief   LVGL object definitions for use outside og LVGL and in 
  *          inter process communication
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _GUIDEF_H_
#define _GUIDEF_H_
#include "config/config.h"


#if USE_LVGL > 0
#include "../../lvgl/lvgl.h"
#include "../GUI/gui_edit.h"

/* Properties of a style, not all LVGL style properties supported */
typedef struct {
  lv_color_t 	bgcolor;		        /* Background color */
  uint8_t		bgopa;					/* BG opacity */	
  lv_color_t 	txtcolor;		        /* Text color */
  uint16_t 		def_width, def_height;	/* default width and height in px */
  uint8_t       align;				/* Text Alignment */
  lv_color_t	bordercolor;			
  uint8_t 		borderwidth;
  uint8_t		borderradius;
  lv_color_t 	shadowcolor;		    /* Shadow color */
  uint8_t		shadow_opa;				/* Shadows opacity */
  uint8_t 		shadow_width;			/* Width of shadow */
  uint8_t		sh_x, sh_y;				/* Shadows x and y offset */
} GUI_Style_T;


extern const GUI_Style_T def_style;             /* default style settings */
extern       GUI_Style_T cur_style;             /* actual style settings  */
extern const GUI_Edit_T edit_style;             /* Edit receipe for style variables */
 

lv_style_t * gui_new_style ( GUI_Style_T *act, lv_style_t *style );


/* Properties that define a label, not all LVGL properties supported */
typedef struct {
  lv_style_t 	*style;					/* style to be used, must be initialized */	
  lv_align_t 	align;					/* Alignment */
  uint16_t 		x0, y0;					/* reference position */
  const char    *txt;					/* Label text */
} GUI_Label_T;

extern const GUI_Label_T def_label;             /* default label settings */
extern       GUI_Label_T cur_label;             /* actual label settings  */
extern const GUI_Edit_T edit_label;             /* Edit receipe for label variables */

lv_obj_t * gui_new_label ( GUI_Label_T *act, lv_style_t *style, lv_obj_t *lbl);

#endif /* USE_LVGL */
#endif /* _GUIDEF_H_ */