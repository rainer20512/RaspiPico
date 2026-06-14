
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
// #include "../GUI/gui_edit.h"

/* max length of user friendly name             */
/* keep it the same in all GUI elements         */
#define GUI_MAX_NAMELEN     16 

/* Different types of GUI elements */
typedef enum {
  GUI_ELEM_NOTYPE       = 0,              /* No type specified */
  GUI_ELEM_STYLE        = 1,              
  GUI_ELEM_LABEL        = 2,
} GUI_Elem_T;

/* Corresponding user friendly names of these of GUI elements */
#define GUI_EDITNAMES   {"NoType", "Style", "Label", }
extern const char *EditNames[];

/* Bitfield for set properties of a GUI_Style_T */
/* Order has to be the same as in corresponding Edit receipe !!! */
typedef enum {
  STYLE_DEFWIDTH     = 0,
  STYLE_DEFHEIGHT    = 1,
  STYLE_TEXTALIGN    = 2,
  STYLE_BGOPA        = 3,
  STYLE_BORDERWIDTH  = 4,
  STYLE_BORDERRADIUS = 5,
  STYLE_SHADOWWIDTH  = 6,
  STYLE_SHADOWOPA    = 7,
  STYLE_SHADOWXREF   = 8,
  STYLE_SHADOWYREF   = 9,
  STYLE_BGCOLOR      = 10,
  STYLE_BORDERCOLOR  = 11,
  STYLE_TXTCOLOR     = 12,
  STYLE_SHADOWCOLOR  = 13,
} Style_Used_T;

#define STYLE_HAS_PROP(style, id) ( (style)->used &  (  1 << (id) ) )
#define STYLE_SET_PROP(style, id) ( (style)->used |=  ( 1 << (id) ) )
#define STYLE_CLR_PROP(style, id) ( (style)->used &= ~( 1 << (id) ) )



/* Properties of a style, not all LVGL style properties supported */
typedef struct {
  uint32_t      used;                   /* bitfield of used properties */
  lv_color_t 	bgcolor;		        /* Background color */
  uint8_t		bgopa;					/* BG opacity */	
  lv_color_t 	txtcolor;		        /* Text color */
  uint16_t 		def_width, def_height;	/* default width and height in px */
  uint8_t       textalign;				/* Text Alignment */
  lv_color_t	bordercolor;			
  uint8_t 		borderwidth;
  uint8_t		borderradius;
  lv_color_t 	shadowcolor;		    /* Shadow color */
  uint8_t		shadow_opa;				/* Shadows opacity */
  uint8_t 		shadow_width;			/* Width of shadow */
  uint8_t		sh_x, sh_y;				/* Shadows x and y offset */
  char          name[GUI_MAX_NAMELEN];  /* User friendly name */      
} GUI_Style_T;



extern const GUI_Style_T def_style;             /* default style settings */
extern       GUI_Style_T cur_style;             /* actual style settings  */

lv_style_t * GUI_new_or_update_style ( GUI_Style_T *act, lv_style_t *style );

/* Bitfield for set properties of a GUI_Label_T */
/* Order has to be the same as in corresponding Edit receipe !!! */
typedef enum {
  LABEL_STYLE        = 0,
  LABEL_ALIGN        = 1,
  LABEL_X0           = 2,
  LABEL_Y0           = 3,
  LABEL_CAPTION      = 4,
  LABEL_NAME         = 5,
} Label_Used_T;

#define LABEL_HAS_PROP(lbl, id) ( (lbl)->used &  (  1 << (id) ) )
#define LABEL_SET_PROP(lbl, id) ( (lbl)->used |=  ( 1 << (id) ) )
#define LABEL_CLR_PROP(lbl, id) ( (lbl)->used &= ~( 1 << (id) ) )


/* Properties that define a label, not all LVGL properties supported */
typedef struct {
  uint32_t      used;
  lv_style_t 	*style;                     /* style to be used, must be initialized */	
  lv_align_t 	align;                      /* Alignment */
  uint16_t 		x0, y0;                     /* reference position */
  char          caption[GUI_MAX_NAMELEN];   /* User friendly name */      
  char          name[GUI_MAX_NAMELEN];      /* User friendly name */      
} GUI_Label_T;

extern const GUI_Label_T def_label;             /* default label settings */
extern       GUI_Label_T cur_label;             /* actual label settings  */

lv_obj_t* GUI_new_or_update_label ( GUI_Label_T *act, lv_obj_t *lbl);
void      GUI_dump_coords ( lv_obj_t * obj );

#endif /* USE_LVGL */
#endif /* _GUIDEF_H_ */