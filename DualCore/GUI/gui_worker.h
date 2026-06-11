
#ifndef _GUIDEF_H_
#define _GUIDEF_H_
#include "config/config.h"

#if USE_LVGL > 0
#include "../../lvgl/lvgl.h"

/* All properties that define a label */

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

/* Recognized data types */
typedef enum {
  GUI_UINT8 = 0,
  GUI_UINT16= 1,
  GUI_RGB888= 2,
  GUI_UINT32= 3,
} GUI_edittype_T;

/* and corresponding length in bytes          */
/* enum and const must match in element count */
const uint8_t GUI_ByteLen[] = {1,2,3,4};

typedef struct {
  const char      *elem_name;
  GUI_edittype_T  elem_type;
  uint16_t        elem_offset;
} GUI_editelem_T;

typedef struct {
  uint16_t count;
  const char *editname;
  GUI_editelem_T gui_element[];
} GUI_edit_T;


typedef struct {
  lv_obj_t 		*obj;					/* Reference to underlying lvgl object, only used by lvgl task */	
  lv_style_t 	*style;					/* style to be used, must be initialized */	
  lv_align_t 	align;					/* Alignment */
  uint16_t 		x0, y0;					/* reference position */
  char     		*txt;					/* Label text */
} GUI_Label_T;


lv_style_t * gui_set_style ( GUI_Style_T *act );

#if GUI_TEST > 0
  void gui_create_label(void);
#endif /* GUI_TEST > 0 */
#endif /* USE_LVGL */
#endif /* _GUIDEF_H_ */