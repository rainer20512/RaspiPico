
#ifndef _GUIDEF_H_
#define _GUIDEF_H_
#include "config/config.h"

#if USE_LVGL > 0
#include "../../lvgl/lvgl.h"

/* All properties that define a label */

/* Properties of a style, not all LVGL style properties supported */
typedef struct {
  lv_style_t 	*style;					/* Reference to created lvgl style, only used by lvgl task */	
  lv_color_t 	bgcolor;		        /* Background color */
  lv_opa_t		bgopa;					/* BG opacity */	
  lv_color_t 	txtcolor;		        /* Text color */
  uint16_t 		def_width, def_height;	/* default width and height in px */
  lv_text_align_t align;				/* Text Alignment */
  lv_color_t	bordercolor;			
  uint16_t 		borderwidth;
  uint8_t		borderradius;
  lv_color_t 	shadowcolor;		    /* Shadow color */
  lv_opa_t		shadow_opa;				/* Shadows opacity */
  uint8_t 		shadow_width;			/* Width of shadow */
  uint8_t		sh_x, sh_y;				/* Shadows x and y offset */
} GUI_Style_T;

typedef struct {
  lv_obj_t 		*obj;					/* Reference to underlying lvgl object, only used by lvgl task */	
  lv_style_t 	*style;					/* style to be used, must be initialized */	
  lv_align_t 	align;					/* Alignment */
  uint16_t 		x0, y0;					/* reference position */
  char     		*txt;					/* Label text */
} GUI_Label_T;


lv_style_t * gui_set_style ( GUI_Style_T *act );


#endif /* USE_LVGL */
#endif /* _GUIDEF_H_ */