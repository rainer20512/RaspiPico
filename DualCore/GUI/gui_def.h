
/**
  ******************************************************************************
  * @file    gui_def.h
  * @author  Rainer
  * @brief   LVGL object definitions for use outside og LVGL and in 
  *          inter process communication
  *
  *          IMPORTANT: Used by both Core1 and Core0. When modifying, 
  *          be sure to recompileboth cores
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _GUIDEF_H_
#define _GUIDEF_H_
#include "config/config.h"

#if USE_GUI_INTERFACE > 0

#include "../../lvgl/lvgl.h"

/* max length of user friendly name             */
/* keep it the same in all GUI elements         */
#define GUI_MAX_NAMELEN     16 

/* Different types of GUI elements */
typedef enum {
  GUI_ELEM_NOTYPE       = 0,              /* No type specified */
  GUI_ELEM_FONT         = 1,
  GUI_ELEM_STYLE        = 2,              
  GUI_ELEM_LABEL        = 3,
  GUI_ELEM_ARC          = 4,
  GUI_ELEM_MAX          = 5,              /* Last element whose value is the number of prev entries */
} GUI_Elem_T;

/* Corresponding user friendly names of these of GUI elements */
#define GUI_EDITNAMES   {"NoType", "Font", "Style", "Label", "Arc", "<Undef>" }
extern const char *EditNames[];

typedef struct {
  char             fontname[GUI_MAX_NAMELEN];
  uint8_t          fontsize;
  const lv_font_t* font;
} GUI_Font_T;

/* Filled by Init on Core1, by IPC on Core0 */
extern GUI_Font_T *AllFonts;


/* Bitfield for set properties of a GUI_Style_T */
/* Order has to be the same as in corresponding Edit receipe !!! */
typedef enum {
  STYLE_DEFWIDTH     = 0,
  STYLE_DEFHEIGHT    = 1,
  STYLE_OBJALIGN     = 2,
  STYLE_BGOPA        = 3,
  STYLE_BGCOLOR      = 4,
  STYLE_BORDERWIDTH  = 5,
  STYLE_BORDERRADIUS = 6,
  STYLE_BORDERCOLOR  = 7,
  STYLE_SHADOWWIDTH  = 8,
  STYLE_SHADOWOPA    = 9,
  STYLE_SHADOWXREF   = 10,
  STYLE_SHADOWYREF   = 11,
  STYLE_SHADOWCOLOR  = 12,
  STYLE_TEXTALIGN    = 13,
  STYLE_TEXTCOLOR    = 14,
  STYLE_TEXTFONT     = 15,
  STYLE_ARCWIDTH     = 16,
  STYLE_ARCOPA       = 17,
  STYLE_ARCCOLOR     = 18,
  STYLE_NAME         = 19,
} Style_Used_T;

#define STYLE_HAS_PROP(style, id) ( (style)->used &  (  1 << (id) ) )
#define STYLE_SET_PROP(style, id) ( (style)->used |=  ( 1 << (id) ) )
#define STYLE_CLR_PROP(style, id) ( (style)->used &= ~( 1 << (id) ) )



/* Properties of a style, not all LVGL style properties supported */
typedef struct {
  uint32_t        used;                     /* bitfield of used properties */
  uint16_t        def_width, def_height;	/* default width and height in px */
  uint8_t         objalign;                 /* Alignment of obj wihin parent obj*/
  uint8_t         bgopa;					/* BG opacity */	
  lv_color_t      bgcolor;                  /* Background color */
  uint8_t         borderwidth;
  uint8_t         borderradius;
  lv_color_t      bordercolor;			
  uint8_t         shadow_opa;				/* Shadows opacity */
  uint8_t         shadow_width;     		/* Width of shadow */
  uint8_t         sh_x, sh_y;				/* Shadows x and y offset */
  lv_color_t      shadowcolor;              /* Shadow color */
  uint8_t         textalign;				/* Text Alignment within obj*/
  lv_color_t      textcolor;		        /* Text color */
  const lv_font_t *textfont;                /* Textfont */
  uint8_t         arcwidth;                 /* witdh of arc */
  uint8_t         arcopa;                   /* opaqueness of arc */
  lv_color_t      arccolor;		            /* Arc color */
  char            name[GUI_MAX_NAMELEN];    /* User friendly name */      
} GUI_Style_T;


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


/* Bitfield for set properties of a GUI_Arc_T */
/* Order has to be the same as in corresponding Edit receipe !!! */
typedef enum {
  ARC_BGSTYLE      = 0,
  ARC_INDSTYLE     = 1,
  ARC_X0           = 2,
  ARC_Y0           = 3,
  ARC_ROTATE       = 4,
  ARC_BGSTART      = 5,
  ARC_BGEND        = 6,
  ARC_MINVAL       = 7,
  ARC_MAXVAL       = 8,
  ARC_CURVAL       = 9,
  ARC_NAME         = 10,
} Arc_Used_T;

#define ARC_HAS_PROP(arc, id) ( (arc)->used &  (  1 << (id) ) )
#define ARC_SET_PROP(arc, id) ( (arc)->used |=  ( 1 << (id) ) )
#define ARC_CLR_PROP(arc, id) ( (arc)->used &= ~( 1 << (id) ) )


/* Properties that define an arc, not all LVGL properties supported */
typedef struct {
  uint32_t      used;                       /* bitfield of used properties */
  lv_style_t 	*bgstyle;                   /* style to be used for background arc */	
  lv_style_t 	*indstyle;                  /* style to be used for indicator arc */	
  uint16_t 		x0, y0;                     /* reference position */
  int16_t       rotation;                   /* rotation           */
  int16_t       bg_start, bg_end;           /* background start and end angle */
  int16_t       minval, maxval;             /* minimum and maximum values for arc */ 
  int16_t       curval;                     /* current value */
  char          name[GUI_MAX_NAMELEN];      /* User friendly name */      
} GUI_Arc_T;


void GUI_dump_coords ( lv_obj_t * obj );
void GUI_Init_Fonts_Core1(void);

struct List_Elem;
struct List_Elem *GUI_new_or_update_entry(uint8_t *data, GUI_Elem_T gui_elem );

#endif /*  USE_GUI_INTERFACE */
#endif /* _GUIDEF_H_ */