#include "config/config.h"

#if USE_GUI_INTERFACE > 0

#include "system/util.h"
#include "../GUI/gui_def.h"
#include "../GUI/gui_edit.h"
#include "../GUI/gui_lists.h"
#include "../GUI/dp_lists.h"
#include "../GUI/gui_ops.h"
#include "../GUI/variant.h"
#include "../GUI/lvgl_update.h"
#include "xml_feeder.h"

#include "system/ipc_msg.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define DEBUG_GUIDEF 1

#if DEBUG_GUIDEF > 0
  #include "debug/debug_helper.h"
#endif


/******************************************************************************
/* Define, how the default screen looks like
 ******************************************************************************/
const GUI_Screen_T def_screen = 
    { .used         = 0b10000000111,
      .rotation     = 180, 
      .bgopa        = 255, 
      .bgcolor      = {0xFF, 0x7f, 0x7f},
      .name         = "Default",
    };

GUI_Screen_T cur_screen;    /* Init'ed by GUI_Init_Curr_Elems */

/******************************************************************************
/* When setting default properties at compile time, keep in mind not to set 
 * fonts bcs they are loaded at runtime. 
 * Fonts may be set at runtime in "GUI_Init_Curr_Elems" by Accessing 
 * "AllFonts"-Array
 ******************************************************************************/

const GUI_Style_T def_style = 
                    /*0b10'0000'0011'1111'1010'0000'0011'1011 */
    { .used         = 0x203fa03b,
      .def_width    = 120, 
      .def_height   = 20, 
      /* .def_length not set */
      .objalign     = LV_ALIGN_CENTER,
      .bgopa        = 255, 
      .bgcolor      = {0x00, 0x00, 0x00},
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
      /* associated textfont is set at runtime by "GUI_Init_Curr_Elems" */
      /* .arcwidth, .arcopa, .arccolor not set! */
      /* .linewidth, .lineopa, .linecolor not set */
      .name         = "Style01",
    };

GUI_Style_T cur_style;    /* Init'ed by GUI_Init_Curr_Elems */

const GUI_Label_T def_label =
  { .used           = 0b10100001110,
    .style          = NULL,
    .align          = LV_ALIGN_CENTER,
    .x0             = 0,
    .y0             = 0,
    /* all rotation properties unset */
    .caption        = "-3,5",
    .ind_formatstr  = NULL,
    .name           = "Label01",
};

GUI_Label_T cur_label;    /* Init'ed by GUI_Init_Curr_Elems */

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

GUI_Arc_T cur_arc;         /* Init'ed by GUI_Init_Curr_Elems */

const GUI_Scale_T def_scale = {
  //.used         = 0b1'0011'1101'1111'1000,
    .used         = 0x13df8,
    .mainstyle    = NULL,
    .majorstyle   = NULL,
    .minorstyle   = NULL,
    .scalemode    = 8,    /* see gui_def.h */
    .totalticks   = 101,  /* 0-100, major ticks every 10 */
    .tickdistance = 10,
    .bLabelShow   = 1,
    .minval       = 0,
    .maxval       = 100,
    .curval       = 10,
    .angle_range  = 270,
    .rotation     = 135,
    .x0           = 0, 
    .y0           = 0,
    .myimage      = NULL,
    .mylabel      = NULL, 
    .name         = "Scale01",
};

GUI_Scale_T cur_scale;         /* Init'ed by GUI_Init_Curr_Elems */

const GUI_Image_T def_image = {
    .used     = 0b1111111100,
    /* associated image ais set at runtime by "GUI_Init_Curr_Elems" */
    .style        = NULL,
    .xofs         = 0, 
    .yofs         = 0, 
    .align        = LV_ALIGN_CENTER,
    .rot_angle    = 0,
    .scale        = 256,
    .pivotx       = 0,
    .pivoty       = 0,
    .name         = "Image01",
};

GUI_Image_T cur_image;         /* Init'ed by GUI_Init_Curr_Elems */


const GUI_Datapoint_T def_dp = {
    .used         = 0b000,
    .dpname       = "",
    .guielemname  = "",
    .propertyname = "",
};


GUI_Datapoint_T cur_dp;         /* Init'ed by GUI_Init_Curr_Elems */


/* user friendly names of these of GUI elements */
const ElemInfo_T Editinfo[]  = GUI_EDITINFO;

/******************************************************************************
 * Internal buffer for sending/receiving one GUI element
 * It consists of GUI elem type, the GUI descrioption data and the
 * binary object translation table.
 * binary objects ( like styles, fonts, ... ) are transferred by
 * name, and in case of fonts: by fontsize
 * Even if not completely filled, the whole buffer is copied/transferred
 *****************************************************************************/
typedef struct {
  uint16_t      size;      /* we also store the size bcs IPC routines dont know that */ 
  uint8_t       bDelete;   /* 1 = delete object, 0 = create or update object         */
  GUI_Edit_Enum elem_type; /* GUI element type in gui_elem                           */
  union {
      GUI_Screen_T    gui_screen;
      GUI_Style_T     gui_style;
      GUI_Label_T     gui_label;
      GUI_Arc_T       gui_arc;
      GUI_Scale_T     gui_scale;
      GUI_Image_T     gui_image;
      GUI_Datapoint_T gui_datapoint;
  } gui_elem;
} IPC_GUI_Xfer_Buff_T;

/* make sure, the IPC transfer buf is capable of store any GUI element */
static_assert(sizeof(IPC_GUI_Xfer_Buff_T) <= IPC_BUFSIZE, "ICP transfer buffer size too small");


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
      obj_in = my_malloc (size);
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
     * @brief  Update the current screen settings from GUI_Screen_T variable. 
     * @param  act    - GUI description of LVGL screen
     * @param  scr    - ptr to associated screen variable
     *****************************************************************************/     
     static lv_obj_t *GUI_update_screen ( GUI_Screen_T *act, lv_obj_t *scr )
    { 
        Variant_T v;
     	/* Check, whether screen is already known to LVGL */
    	/* if not, allocate space and init */
        if ( !scr ) {
          scr = lv_scr_act();
        } 

       /* handle reset first */
        if ( SCREEN_HAS_PROP(act, SCREEN_RESET) && act->resetoninit ) {
          GUI_Reset_GUI_Core1();
        } 

    	/* set screen properties */
/*  1 */
        if ( SCREEN_HAS_PROP(act, SCREEN_ROTATE))       { V_Set_U16(&v, act->rotation);     LVGL_update_screen (scr, SCREEN_ROTATE, &v);      }
        if ( SCREEN_HAS_PROP(act, SCREEN_BGOPA))        { V_Set_U8(&v, act->bgopa);         LVGL_update_screen (scr, SCREEN_BGOPA, &v);       }
        if ( SCREEN_HAS_PROP(act, SCREEN_BGCOLOR))      { V_Set_Rgb(&v, act->bgcolor);      LVGL_update_screen (scr, SCREEN_BGCOLOR, &v);     }
        if ( SCREEN_HAS_PROP(act, SCREEN_BGMAINOPA))    { V_Set_U8(&v, act->bgmainopa);     LVGL_update_screen (scr, SCREEN_BGMAINOPA, &v);   }
        if ( SCREEN_HAS_PROP(act, SCREEN_BGGRDCOLOR))   { V_Set_Rgb(&v, act->bggradcolor);  LVGL_update_screen (scr, SCREEN_BGGRDCOLOR, &v);  }
/*  6 */        
        if ( SCREEN_HAS_PROP(act, SCREEN_BGGRADOPA))    { V_Set_U8(&v, act->bggradopa);     LVGL_update_screen (scr, SCREEN_BGGRADOPA, &v);   }
        if ( SCREEN_HAS_PROP(act, SCREEN_BGGRADDIR))    { V_Set_U8(&v, act->bggraddir);     LVGL_update_screen (scr, SCREEN_BGGRADDIR, &v);   }
        if ( SCREEN_HAS_PROP(act, SCREEN_BGMAINSTOP))   { V_Set_U8(&v, act->bgmainstop);    LVGL_update_screen (scr, SCREEN_BGMAINSTOP, &v);  }
        if ( SCREEN_HAS_PROP(act, SCREEN_BGGRADSTOP))   { V_Set_U8(&v, act->bggradstop);    LVGL_update_screen (scr, SCREEN_BGGRADSTOP, &v);  }

        return scr;
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
        } 

    	/* assign _All_ style properties */
        Variant_T v;
/*  1 */
    	LVGL_update_style(style, STYLE_WIDTH,       STYLE_HAS_PROP(act, STYLE_WIDTH)        ? V_Set_U16(&v,act->def_width),&v         : NULL );   
    	LVGL_update_style(style, STYLE_HEIGHT,      STYLE_HAS_PROP(act, STYLE_HEIGHT)       ? V_Set_U16(&v,act->def_height),&v        : NULL );  
    	LVGL_update_style(style, STYLE_LENGTH,      STYLE_HAS_PROP(act, STYLE_LENGTH)       ? V_Set_U16(&v,act->def_length),&v        : NULL );  
        LVGL_update_style(style, STYLE_OBJALIGN,    STYLE_HAS_PROP(act, STYLE_OBJALIGN)     ? V_Set_U8(&v,act->objalign),&v           : NULL );     
        LVGL_update_style(style, STYLE_BGOPA,       STYLE_HAS_PROP(act, STYLE_BGOPA)        ? V_Set_U8(&v,act->bgopa),&v              : NULL );        
/*  6 */
        LVGL_update_style(style, STYLE_BGCOLOR,     STYLE_HAS_PROP(act, STYLE_BGCOLOR)      ? V_Set_Rgb(&v,act->bgcolor),&v           : NULL );     
        LVGL_update_style(style, STYLE_BGMAINOPA,   STYLE_HAS_PROP(act, STYLE_BGMAINOPA)    ? V_Set_U8(&v,act->bgmainopa),&v          : NULL );    
        LVGL_update_style(style, STYLE_BGGRDCOLOR,  STYLE_HAS_PROP(act, STYLE_BGGRDCOLOR)   ? V_Set_Rgb(&v,act->bggradcolor),&v       : NULL ); 
        LVGL_update_style(style, STYLE_BGGRADOPA,   STYLE_HAS_PROP(act, STYLE_BGGRADOPA)    ? V_Set_U8(&v,act->bggradopa),&v          : NULL );    
        LVGL_update_style(style, STYLE_BGGRADDIR,  STYLE_HAS_PROP(act, STYLE_BGGRADDIR)    ? V_Set_U8(&v,act->bggraddir),&v           : NULL );    
/* 11 */
        LVGL_update_style(style, STYLE_BGMAINSTOP,  STYLE_HAS_PROP(act, STYLE_BGMAINSTOP)   ? V_Set_U8(&v,act->bgmainstop),&v         : NULL );   
        LVGL_update_style(style, STYLE_BGGRADSTOP,  STYLE_HAS_PROP(act, STYLE_BGGRADSTOP)   ? V_Set_U8(&v,act->bggradstop),&v         : NULL );   
        LVGL_update_style(style, STYLE_BORDERWIDTH, STYLE_HAS_PROP(act, STYLE_BORDERWIDTH)  ? V_Set_U8(&v,act->borderwidth),&v        : NULL );  
        LVGL_update_style(style, STYLE_BORDERRADIUS,STYLE_HAS_PROP(act, STYLE_BORDERRADIUS) ? V_Set_U8(&v,act->borderradius),&v       : NULL ); 
        LVGL_update_style(style, STYLE_BORDERCOLOR, STYLE_HAS_PROP(act, STYLE_BORDERCOLOR)  ? V_Set_Rgb(&v,act->bordercolor),&v       : NULL ); 
/* 16 */
        LVGL_update_style(style, STYLE_SHADOWXREF,  STYLE_HAS_PROP(act, STYLE_SHADOWXREF)   ? V_Set_U8(&v,act->sh_x),&v               : NULL );         
        LVGL_update_style(style, STYLE_SHADOWYREF,  STYLE_HAS_PROP(act, STYLE_SHADOWYREF)   ? V_Set_U8(&v,act->sh_y),&v               : NULL );         
        LVGL_update_style(style, STYLE_SHADOWWIDTH, STYLE_HAS_PROP(act, STYLE_SHADOWWIDTH)  ? V_Set_U8(&v,act->shadow_width),&v       : NULL ); 
        LVGL_update_style(style, STYLE_SHADOWOPA,   STYLE_HAS_PROP(act, STYLE_SHADOWOPA)    ? V_Set_U8(&v,act->shadow_opa),&v         : NULL );   
        LVGL_update_style(style, STYLE_SHADOWCOLOR, STYLE_HAS_PROP(act, STYLE_SHADOWCOLOR)  ? V_Set_Rgb(&v,act->shadowcolor),&v       : NULL ); 
/* 21 */
        LVGL_update_style(style, STYLE_TEXTALIGN,   STYLE_HAS_PROP(act, STYLE_TEXTALIGN)    ? V_Set_U8(&v,act->textalign),&v          : NULL );    
        LVGL_update_style(style, STYLE_TEXTCOLOR,   STYLE_HAS_PROP(act, STYLE_TEXTCOLOR)    ? V_Set_Rgb(&v,act->textcolor),&v         : NULL );   
        LVGL_update_style(style, STYLE_TEXTFONT,    STYLE_HAS_PROP(act, STYLE_TEXTFONT)     ? V_Set_Ref(&v,(void *)act->textfont),&v  : NULL );    
        LVGL_update_style(style, STYLE_ARCWIDTH,    STYLE_HAS_PROP(act, STYLE_ARCWIDTH)     ? V_Set_U8(&v,act->arcwidth),&v           : NULL );     
        LVGL_update_style(style, STYLE_ARCOPA,      STYLE_HAS_PROP(act, STYLE_ARCOPA)       ? V_Set_U8(&v,act->arcopa),&v             : NULL );       
/* 26 */
        LVGL_update_style(style, STYLE_ARCCOLOR,    STYLE_HAS_PROP(act, STYLE_ARCCOLOR)     ? V_Set_Rgb(&v,act->arccolor),&v          : NULL );    
        LVGL_update_style(style, STYLE_LINEWIDTH,   STYLE_HAS_PROP(act, STYLE_LINEWIDTH)    ? V_Set_U8(&v,act->linewidth),&v          : NULL );    
        LVGL_update_style(style, STYLE_LINECOLOR,   STYLE_HAS_PROP(act, STYLE_LINECOLOR)    ? V_Set_Rgb(&v,act->linecolor),&v         : NULL );   
        LVGL_update_style(style, STYLE_LINEOPA,     STYLE_HAS_PROP(act, STYLE_LINEOPA)      ? V_Set_U8(&v,act->lineopa),&v            : NULL );      


    	return style;	
    }

    /******************************************************************************
     * @brief  Resets a LVGL style to empty and informs all style users
     *         styles cannot be deleted but reset and must be handeled separately
     *****************************************************************************/     
    static lv_style_t * GUI_reset_style ( lv_style_t *style ) 
    {
        lv_style_reset( style);
        /* update using widgets about style change */
        lv_obj_report_style_change(style);
    }

    /******************************************************************************
     * @brief  delete an LVGL GUI object ( label, arc, scale, ... )
     *         styles cannot be deleted but reset and must be handeled separately
     *****************************************************************************/     
    void GUI_delete_obj ( lv_obj_t *anyobj  )
    {
        /* delete LVGL object first */
        lv_obj_delete(anyobj);
        /* free object ptr */
        my_free( anyobj );
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
 
        /* A label has private data elements, get access to it */
        List_Elem_T *ll = LL_find_by_type_n_obj(GUI_ITEM_LIST, GUI_ELEM_LABEL, lbl);
        void *privdata = ll ? ll->private_data : NULL;
        #if DEBUG_GUIEDIT > 0 || DEBUG_DATAPOINTS > 0
            if ( !ll) DEBUG_PUTS("Err: No private data for label");
        #endif

    	/* assign label  properties */
        Variant_T v;
        /*  1 */        
        LVGL_update_label(lbl, LABEL_STYLE,       LABEL_HAS_PROP(act, LABEL_STYLE)       ? V_Set_Ref (&v, act->style),&v           : NULL, privdata);
        LVGL_update_label(lbl, LABEL_ALIGN,       LABEL_HAS_PROP(act, LABEL_ALIGN)       ? V_Set_U8  (&v, act->align),&v           : NULL, privdata);
        LVGL_update_label(lbl, LABEL_X0,          LABEL_HAS_PROP(act, LABEL_X0)          ? V_Set_U16 (&v, act->x0),&v              : NULL, privdata);
        LVGL_update_label(lbl, LABEL_Y0,          LABEL_HAS_PROP(act, LABEL_Y0)          ? V_Set_U16 (&v, act->y0),&v              : NULL, privdata);
        LVGL_update_label(lbl, LABEL_CAPTION,     LABEL_HAS_PROP(act, LABEL_CAPTION)     ? V_Set_CStr(&v, act->caption),&v         : NULL, privdata);
        /*  6 */ 
        LVGL_update_label(lbl, LABEL_PIVOTX,      LABEL_HAS_PROP(act, LABEL_PIVOTX)      ? V_Set_U16 (&v, act->pivotx),&v          : NULL, privdata);
        LVGL_update_label(lbl, LABEL_PIVOTY,      LABEL_HAS_PROP(act, LABEL_PIVOTY)      ? V_Set_U16 (&v, act->pivoty),&v          : NULL, privdata);
        LVGL_update_label(lbl, LABEL_ROTATE,      LABEL_HAS_PROP(act, LABEL_ROTATE)      ? V_Set_I16 (&v, act->rot_angle),&v       : NULL, privdata);
        LVGL_update_label(lbl, LABEL_Y0,          LABEL_HAS_PROP(act, LABEL_Y0)          ? V_Set_U16 (&v, act->y0),&v              : NULL, privdata);
        LVGL_update_label(lbl, LABEL_CURVAL,      LABEL_HAS_PROP(act, LABEL_CURVAL)      ? V_Set_I32 (&v, act->ind_value),&v       : NULL, privdata);
        LVGL_update_label(lbl, LABEL_SCALEFACTOR, LABEL_HAS_PROP(act, LABEL_SCALEFACTOR) ? V_Set_I8  (&v, act->ind_scalefactor),&v : NULL, privdata);
        LVGL_update_label(lbl, LABEL_FORMATSTR,   LABEL_HAS_PROP(act, LABEL_FORMATSTR)   ? V_Set_CStr(&v, act->ind_formatstr),&v   : NULL, privdata);

        //GUI_dump_coords(lbl);
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

    	/* assign arc properties */
        Variant_T v;
        /*  1 */  
        if ( ARC_HAS_PROP(act, ARC_BGSTYLE))  { V_Set_Ref(&v, act->bgstyle);  LVGL_update_arc(arc, ARC_BGSTYLE, &v); }
        if ( ARC_HAS_PROP(act, ARC_INDSTYLE)) { V_Set_Ref(&v, act->indstyle); LVGL_update_arc(arc, ARC_INDSTYLE, &v); }
        if ( ARC_HAS_PROP(act, ARC_X0))       { V_Set_U16(&v, act->x0);       LVGL_update_arc(arc, ARC_X0, &v); }
        if ( ARC_HAS_PROP(act, ARC_Y0))       { V_Set_U16(&v, act->y0);       LVGL_update_arc(arc, ARC_Y0, &v); }
        if ( ARC_HAS_PROP(act, ARC_ROTATE))   { V_Set_U16(&v, act->rotation); LVGL_update_arc(arc, ARC_ROTATE, &v); }
        /*  6 */  
        if ( ARC_HAS_PROP(act, ARC_BGSTART))  { V_Set_I16(&v, act->bg_start); LVGL_update_arc(arc, ARC_BGSTART, &v); }
        if ( ARC_HAS_PROP(act, ARC_BGEND))    { V_Set_I16(&v, act->bg_end);   LVGL_update_arc(arc, ARC_BGEND, &v); }
        if ( ARC_HAS_PROP(act, ARC_MINVAL))   { V_Set_I16(&v, act->minval);   LVGL_update_arc(arc, ARC_MINVAL, &v); }
        if ( ARC_HAS_PROP(act, ARC_MAXVAL))   { V_Set_I16(&v, act->maxval);   LVGL_update_arc(arc, ARC_MINVAL, &v); }
        if ( ARC_HAS_PROP(act, ARC_CURVAL))   { V_Set_I16(&v, act->curval);   LVGL_update_arc(arc, ARC_MINVAL, &v); }

        //GUI_dump_coords(arc);
        return arc;
    }


 
    /******************************************************************************
     * @brief  Create a new LVGL scale or update an existing scale 
     *         from GUI_Scale_T variable. 
     *         if scale IS NULL, new LVGL object will be created, if != NULL it is
     *         regarded as a valid lvgl scale object and updated accordingly  
     * @param  act    - GUI description of LVGL scale
     * @param  scale  - ptr to associated existing scale object in LVGL or NULL
     * @retval pointer to new or updated LVGL scale
     * @note   the lvgl object variable is dynamically allocated from heap
     *         user is repsonsible for freeing if no longer needed
     *****************************************************************************/     
    static lv_obj_t* GUI_new_or_update_scale ( GUI_Scale_T *act, lv_obj_t *scale )
    {
    	/* Check, whether scale is already created in LVGL */
    	if ( !scale ) { 
          if ( !(scale = GUI_Allocate( scale, sizeof(lv_obj_t*))) ) return NULL;
          scale = lv_scale_create(lv_screen_active());
        }

        /* A scale has private data elements, get access to it */
        List_Elem_T *ll = LL_find_by_type_n_obj(GUI_ITEM_LIST, GUI_ELEM_SCALE, scale);
        void *privdata = ll ? ll->private_data : NULL;
        #if DEBUG_GUIEDIT > 0 || DEBUG_DATAPOINTS > 0
            if ( !ll) DEBUG_PUTS("Err: No private data for scale");
        #endif

    	/* assign scale properties */
        Variant_T v;
        /* 1 */
        if ( SCALE_HAS_PROP(act, SCALE_MAINSTYLE))      { V_Set_Ref(&v, act->mainstyle);      LVGL_update_scale(scale, SCALE_MAINSTYLE, &v, privdata); }
        if ( SCALE_HAS_PROP(act, SCALE_MAJORSTYLE))     { V_Set_Ref(&v, act->majorstyle);     LVGL_update_scale(scale, SCALE_MAJORSTYLE, &v, privdata); }
        if ( SCALE_HAS_PROP(act, SCALE_MINORSTYLE))     { V_Set_Ref(&v, act->minorstyle);     LVGL_update_scale(scale, SCALE_MINORSTYLE, &v, privdata); }
        if ( SCALE_HAS_PROP(act, SCALE_MODE))           { V_Set_U8(&v, act->scalemode);       LVGL_update_scale(scale, SCALE_MODE, &v, privdata); }
        if ( SCALE_HAS_PROP(act, SCALE_X0))             { V_Set_U16(&v, act->x0);             LVGL_update_scale(scale, SCALE_X0, &v, privdata); }
        /* 6 */
        if ( SCALE_HAS_PROP(act, SCALE_Y0))             { V_Set_U16(&v, act->y0);             LVGL_update_scale(scale, SCALE_Y0, &v, privdata); }
        if ( SCALE_HAS_PROP(act, SCALE_MINVAL))         { V_Set_I16(&v, act->minval);         LVGL_update_scale(scale, SCALE_MINVAL, &v, privdata); }
        if ( SCALE_HAS_PROP(act, SCALE_MAXVAL))         { V_Set_I16(&v, act->maxval);         LVGL_update_scale(scale, SCALE_MAXVAL, &v, privdata); }
        if ( SCALE_HAS_PROP(act, SCALE_TOTAL_TICKS))    { V_Set_U16(&v, act->totalticks);     LVGL_update_scale(scale, SCALE_TOTAL_TICKS, &v, privdata); }
        if ( SCALE_HAS_PROP(act, SCALE_MAJ_TICK_DIST))  { V_Set_U16(&v, act->tickdistance);   LVGL_update_scale(scale, SCALE_MAJ_TICK_DIST, &v, privdata); }
        /* 11 */
        if ( SCALE_HAS_PROP(act, SCALE_SHOWLABEL))      { V_Set_U8(&v, act->bLabelShow != 0); LVGL_update_scale(scale, SCALE_SHOWLABEL, &v, privdata); }
        if ( SCALE_HAS_PROP(act, SCALE_ANGLE_RANGE))    { V_Set_U16(&v, act->angle_range);    LVGL_update_scale(scale, SCALE_ANGLE_RANGE, &v, privdata); }
        if ( SCALE_HAS_PROP(act, SCALE_ROTATE))         { V_Set_U16(&v, act->rotation);       LVGL_update_scale(scale, SCALE_ROTATE, &v, privdata); }

        /* finally the three private values */
        LVGL_update_scale(scale, SCALE_CURVAL,  SCALE_HAS_PROP(act, SCALE_CURVAL)  ? V_Set_I16(&v, act->curval),&v  : NULL, privdata);
        LVGL_update_scale(scale, SCALE_MYIMAGE, SCALE_HAS_PROP(act, SCALE_MYIMAGE) ? V_Set_Ref(&v, act->myimage),&v : NULL, privdata);
        LVGL_update_scale(scale, SCALE_MYLABEL, SCALE_HAS_PROP(act, SCALE_MYLABEL) ? V_Set_Ref(&v, act->mylabel),&v : NULL, privdata);

        //GUI_dump_coords(arc);
        return scale;

    }

    /******************************************************************************
     * @brief  Create a new LVGL image or update an existing image 
     *         from GUI_Image_T variable. 
     *         if img IS NULL, new LVGL object will be created, if != NULL it is
     *         regarded as a valid lvgl image object and updated accordingly  
     * @param  act    - GUI description of LVGL img
     * @param  img    - ptr to associated existing img object in LVGL or NULL
     * @retval pointer to new or updated LVGL image
     * @note   the lvgl object variable is dynamically allocated from heap
     *         user is repsonsible for freeing if no longer needed
     *****************************************************************************/     
    static lv_obj_t* GUI_new_or_update_image ( GUI_Image_T *act, lv_obj_t *img )
    {
    	/* Check, whether scale is already created in LVGL */
    	if ( !img ) { 
          if ( !(img = GUI_Allocate( img, sizeof(lv_obj_t*))) ) return NULL;
          img = lv_img_create(lv_screen_active());
        }
        /* 1 */
        if ( IMAGE_HAS_PROP(act, IMAGE_IMAGE))          lv_image_set_src(img, act->image);
        if ( IMAGE_HAS_PROP(act, IMAGE_STYLE))          lv_obj_add_style(img, act->style, LV_PART_MAIN );

        if ( IMAGE_HAS_PROP(act, IMAGE_XOFS))           lv_image_set_offset_x(img, act->xofs);
        if ( IMAGE_HAS_PROP(act, IMAGE_YOFS))           lv_image_set_offset_y(img, act->yofs); 

/*
        if ( IMAGE_HAS_PROP(act, IMAGE_XOFS))           lv_obj_set_x(img, act->xofs);
        if ( IMAGE_HAS_PROP(act, IMAGE_YOFS))           lv_obj_set_y(img, act->yofs); 
*/
        if ( IMAGE_HAS_PROP(act, IMAGE_ALIGN))          lv_image_set_inner_align(img, act->align);
        if ( IMAGE_HAS_PROP(act, IMAGE_ROTATE))         lv_image_set_rotation(img, act->rot_angle);
        /* 6 */
        if ( IMAGE_HAS_PROP(act, IMAGE_SCALE))          lv_image_set_scale(img, act->scale);
        if ( IMAGE_HAS_PROP(act, IMAGE_PIVOTX))         lv_image_set_pivot_x(img, act->pivotx);
        if ( IMAGE_HAS_PROP(act, IMAGE_PIVOTY))         lv_image_set_pivot_y(img, act->pivoty); 

        GUI_dump_coords(img);

        return img;

    }


#if defined(RP2040_M0_1) || defined(CORE1_SIM)
    /******************************************************************************
     * @brief Load all known images _once_ into GUI elem list, 
     * @note  Core1 implementation
     *****************************************************************************/     
    void GUI_Init_Images_Core1(void)
    {
      List_Elem_T *img;

      /* Iterate thru all defined imgs and insert them into global item list */
      /* List of defined imgs _MUST BE_ terminated by NULL,NULL */
      for ( uint32_t i = 0; i < AllImagesNum1; i++ ) {
         img = LL_New_Element(GUI_ELEM_RAWIMG,  (void *)AllImages1[i].image, AllImages1[i].imagename, &AllImages1[i], 0, 0);
         LL_append(&GUI_item_list, img);
      }
      printf("%d images loaded from 0x%p\n", AllImagesNum1,AllImages1);
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
         font = LL_New_Element(GUI_ELEM_FONT,  (void *)AllFonts1[i].font, AllFonts1[i].fontname, &AllFonts1[i], AllFonts1[i].fontsize, 0);
         LL_append(&GUI_item_list, font);
      }
      printf("%d fonts loaded from 0x%p\n", AllFontNum1,AllFonts1);
    }


    /******************************************************************************
     * @brief  Create a new or update a LVGL element from GUI-Element
     * @param  data     - raw GUI element data
     * @param  editdata - edit receipe for raw data
     * @retval untyped ptr to updated or new created LVGL obj or NULL, if creation failed
     ******************************************************************************/
    void *GUI_Create_or_update_LVGL_Core1(uint8_t *data, const GUI_Edit_T *editdata, void *lvgl_obj )
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
          case GUI_ELEM_SCALE:
            return  GUI_new_or_update_scale ( (GUI_Scale_T *)data, (lv_obj_t *)lvgl_obj );
            break;
          case GUI_ELEM_IMAGE:
            return  GUI_new_or_update_image ( (GUI_Image_T *)data, (lv_obj_t *)lvgl_obj );
            break;
          case GUI_ELEM_SCREEN:
            return  GUI_update_screen ( (GUI_Screen_T *)data, (lv_obj_t *)lvgl_obj );
            break;
          default:
            printf("Err: No LVGL Update handler for LVGL %s\n", Editinfo[editdata->gui_elem_type].name);
        }
    }

        /******************************************************************************
     * @brief  Create a new or update a LVGL element from GUI-Element
     * @param  data     - raw GUI element data
     * @param  editdata - edit receipe for raw data
     * @retval untyped ptr to updated or new created LVGL obj or NULL, if creation failed
     ******************************************************************************/
    void GUI_Delete_LVGL_Core1(uint8_t *data, const GUI_Edit_T *editdata, void *lvgl_obj )
    {
        switch(editdata->gui_elem_type) {
          case GUI_ELEM_NOTYPE:
            printf("Err: Cannot delete untyped LVGL object\n");
            break;
          case GUI_ELEM_STYLE:
            GUI_reset_style( (lv_style_t *)lvgl_obj );
            break;
          case GUI_ELEM_LABEL:
          case GUI_ELEM_ARC:
          case GUI_ELEM_SCALE:
          case GUI_ELEM_IMAGE:
             GUI_delete_obj( (lv_obj_t *)lvgl_obj );
            break;
          default:
            printf("Err: No LVGL Delete handler for LVGL %s\n", Editinfo[editdata->gui_elem_type].name);
        } /* switch */
    }

#endif /* defined(RP2040_M0_1) || defined(CORE1_SIM) */
  
#endif /* USE_LVGL > 0 */

struct TxInfoT {
  uint16_t  additional;
  uint8_t   tr_idx;
};

#if  RP2040_M0_0
    bool IPC_Pack_Transferbuf( IPC_GUI_Xfer_Buff_T *txbuf, uint8_t *data, const GUI_Edit_T *editdata, uint8_t bDelete )
    {
        /* Determine size of packet */
        uint8_t sendsize = sizeof(IPC_GUI_Xfer_Buff_T);
    
        /* We do not need to handle each type individually, the following switch/case */
        /* is jut to ensure, you check all entities when adding new GUI element types */
        if (     editdata->gui_elem_type != GUI_ELEM_STYLE && editdata->gui_elem_type != GUI_ELEM_LABEL && editdata->gui_elem_type != GUI_ELEM_ARC 
              && editdata->gui_elem_type != GUI_ELEM_SCALE && editdata->gui_elem_type != GUI_ELEM_IMAGE && editdata->gui_elem_type != GUI_ELEM_SCREEN
              && editdata->gui_elem_type != GUI_ELEM_DATAPOINT
           ) 
        { 
            printf("Err: No Transfer handler for LVGL %s\n", Editinfo[editdata->gui_elem_type].name);
            return false;
        }
        txbuf->size      = sendsize;
        txbuf->bDelete   = bDelete;
        txbuf->elem_type = editdata->gui_elem_type;

        /* Copy GUI Element, length depending from type */
        memcpy_fast(&txbuf->gui_elem, data, editdata->total_size);

        return true;
    }

 
    /* Buffer to send one GUI element */
    static IPC_GUI_Xfer_Buff_T sendbuf;

    /******************************************************************************
     * @brief  Core0 Implementation of "GUI_Create_or_update_LVGL":
     *         just send the GUI_Item data via IPC to Core 1, regadrless whether 
     *         new or opdated
     * @param  data     - raw GUI element data
     * @param  editdata - edit receipe for raw data
     * @retval untyped ptr to updated or new created LVGL obj or NULL, if creation failed
     ******************************************************************************/
    void GUI_Create_or_update_LVGL_Core0(uint8_t *data, const GUI_Edit_T *editdata)
    {
        /* pack GUI Element and initiate transfer, which is done asynchronally */
        if ( IPC_Pack_Transferbuf(&sendbuf, data, editdata, 0 ) ) Core0_Send_Gui_Elem ( &sendbuf, NULL );
   }

 
    /******************************************************************************
     * @brief  Create a new or update an existing GUI element in Core0 
     *         This is done b just sending the corresponding element data to core 1 
     * @param  data     - raw element data
     * @param  gui_elem - GUI element Type
     * @retval List_Elem_T ptr to the new or updated element or NULL in case of Error
     ******************************************************************************/
    void GUI_new_or_update_entry_Core0(uint8_t *data, GUI_Edit_Enum gui_elem )
    {
        /* Find the edit receipe */
        const GUI_Edit_T *edit = Find_EditInfoByType( gui_elem );
        GUI_Create_or_update_LVGL_Core0(data, edit);
    }
#endif /* RP2040_M0_0 */

#if RP2040_M0_1 || defined(CORE1_SIM) 

    bool IPC_Unpack_Transferbuf( IPC_GUI_Xfer_Buff_T *rxbuf, uint8_t *data, uint16_t datalen )
    {
        /* Get the total size. It has to be the size of IPC_GUI_Xfer_Buff_T */
        if ( datalen != sizeof(IPC_GUI_Xfer_Buff_T)) {
            printf("Err: expected and real RX buffer size differ: %d vs %d\n", datalen, sizeof(IPC_GUI_Xfer_Buff_T));
            return false;
        }

        /* copy to rxbuf */
        memcpy_fast(rxbuf, data, datalen); 

        return true;
    }
    /* forward declarations -----------------------------------------------------*/
    void GUI_new_or_update_entry_Core1(uint8_t *data, GUI_Edit_Enum gui_elem );

    /* Buffer to receive one GUI element */
    static IPC_GUI_Xfer_Buff_T recvbuf;

    /******************************************************************************
     * @brief  Core1 received an Gui element description via IPC. The data is
     *         still stored in IPC_Buff
     *         to free the ipc buf asap, copy content to a IPC_GUI_Xfer_Buff_T
     *         variable and continue processing on that variable
     * @param  data     - ptr to IPC receive buffer 
     * @param  buflen   - number of bytes in that buffer
     ******************************************************************************/
    void Core1_Receive_LVGL_obj(uint8_t *data, uint16_t buflen)
    {
      uint8_t bDelete;
      /* unpack data into recvbuf */
      IPC_Unpack_Transferbuf( &recvbuf, data, buflen );


      /* and create GUI-Element */
      #if DEBUG_GUIDEF > 0
          DEBUG_PRINTF("Received GUI element of type %s\n", Editinfo[recvbuf.elem_type].name);
      #endif

      if ( recvbuf.bDelete ) 
          GUI_delete_entry_Core1( (uint8_t*)&recvbuf.gui_elem, recvbuf.elem_type );
      else
          GUI_new_or_update_entry_Core1( (uint8_t*)&recvbuf.gui_elem, recvbuf.elem_type );
    }


    static void GUI_update_entry_Core1(List_Elem_T *ll_elem, uint8_t *data, const GUI_Edit_T *editdata )
    {
        /* overwrite the complete GUI element data structure */
        uint8_t *dest = ll_elem->ll_entry;
        memcpy_fast(dest, data, editdata->total_size);

        /* Update associated LVGL obj */
        ll_elem->ll_lvgl_obj = GUI_Create_or_update_LVGL_Core1( data, editdata, ll_elem->ll_lvgl_obj );
        printf("Core1: %s %s updated\n",Editinfo[editdata->gui_elem_type].name,ll_elem->ll_name);
    }

    static void GUI_create_entry_Core1(uint8_t *data, const GUI_Edit_T *editdata )
    {
        GUI_Edit_Enum etype = editdata->gui_elem_type;
        
        /* create a full copy of actual data structure */
        uint8_t *copy = my_malloc(editdata->total_size);
        if ( !copy ) return;
    
        memcpy_fast(copy, data, editdata->total_size);

        /* First create associated LVGL obj */
        void *lvgl_obj=NULL;
        lvgl_obj = GUI_Create_or_update_LVGL_Core1( data, editdata, lvgl_obj );

        /* Therafter create GUI item list entry */
        List_Elem_T *new;
        /* find position of "name" field in raw data */
        char *name = (char *)(copy + editdata->name_ofs);
        /* In case of fonts: also get the fontsize and store as additional item */
        uint32_t additional = ( etype == GUI_ELEM_FONT ? ((GUI_Font_T*)data)->fontsize: 0); 
    
        new = LL_New_Element( etype, lvgl_obj, name, copy, additional, editdata->priv_datasize );
        LL_append(&GUI_item_list, new );

        printf("Core1: %s %s created\n",Editinfo[etype].name,name);

        /* In case of labels or scales: Update immediately after creation to have access to private data in list elem */
        if ( etype == GUI_ELEM_LABEL || etype == GUI_ELEM_SCALE ) GUI_Create_or_update_LVGL_Core1( data, editdata, lvgl_obj );
    }

    void GUI_delete_entry_Core1(uint8_t *data, GUI_Edit_Enum gui_elem )
    {
       /* Find the edit receipe */
       const GUI_Edit_T *editdata = Find_EditInfoByType( gui_elem );
       if ( !editdata )NULL;
   
       if (editdata->gui_elem_type == GUI_ELEM_FONT || editdata->gui_elem_type == GUI_ELEM_RAWIMG) {
            #if DEBUG_GUIDEF > 0
                DEBUG_PRINTF("Err: Attempt to delete Image or Font!\n");
            #endif
            return;
        }
    
        /* find position of "name" field in raw data */
        char *name = (char *)(data + editdata->name_ofs);
        /* Fonts cannot be deleted, so we don't have to care about additional info here, just find by name and type */
        List_Elem_T *del = LL_find_by_type_n_name ( GUI_item_list, editdata->gui_elem_type, name );
        if  ( !del ) {
            #if DEBUG_GUIDEF > 0
                DEBUG_PRINTF("Err: %s %s not found when trying to delete !\n", Editinfo[editdata->gui_elem_type].name, name);
            #endif
            return;
        }

        /* Delete associated LVGL obj */
        void *lvgl_obj=NULL;
        GUI_Delete_LVGL_Core1( data, editdata, del->ll_lvgl_obj);

        /* Styles cannot be deleted, they have been reset by GUI_delete, so reset them in internal data, too */
        if ( editdata->gui_elem_type == GUI_ELEM_STYLE ) {
            GUI_Edit_SetUsedBits(editdata, 0,0 );
            printf("%s %s reset\n",Editinfo[editdata->gui_elem_type].name,name);
        } else {
            del->ll_lvgl_obj = NULL;
            /* first delete internal edit data */
            printf("%s %s deleted\n",Editinfo[editdata->gui_elem_type].name,name);
            my_free( del->ll_entry);
            /* if we have private data, delete it, too */
            if ( del->private_data ) my_free( del->private_data );
            /* thereafter delete element in GUI item list */
            LL_delete(&GUI_item_list, del);
        }
    }

    static void GUI_handle_datapoint_Core1(uint8_t *data, GUI_Edit_Enum gui_elem )
    {
        /* Ensure, that element type is allowed to have datapoints and has an edit receipe*/
        const GUI_Edit_T *edit = Find_EditInfoByType( gui_elem );
        if ( !edit ) return;

        /* Get Datapoint string identifiers */
        GUI_Datapoint_T *temp = (GUI_Datapoint_T *)data;

        DEBUG_PRINTF("N=%s, G=%s, P=%s\n", temp->dpname, temp->guielemname, temp->propertyname);

        DPList_Elem_T *ret = DP_Setup_ListElem(temp->dpname, temp->guielemname, temp->propertyname);
        if ( !ret ) {
            output_printf("Datapoint %s creation error\n", temp->dpname);
            return;
        }

        if ( !Editinfo[ret->elemtype].bHasDatapoints ) {
            output_printf("Datapoints not allowed for %s\n", Editinfo[ret->elemtype].name);
            return;
        }

        if ( DP_Find(GUI_Dp_list) ) {
            output_printf("Datapoint %s defined twice\n", temp->dpname);
            return;
        }
        
        DP_Append(&GUI_Dp_list);  

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
    void GUI_new_or_update_entry_Core1(uint8_t *data, GUI_Edit_Enum gui_elem )
    {
      /* First check for pseudo elements, like Datapoints */
      if ( gui_elem == GUI_ELEM_DATAPOINT ) {
        GUI_handle_datapoint_Core1(data, gui_elem );
        return;
      }
      /* Find the edit receipe */
      const GUI_Edit_T *edit = Find_EditInfoByType( gui_elem );
      if ( !edit ) return;

      /* find position of "name" field in raw data */
      char *name = (char *)(data + edit->name_ofs);

      /* first try to find the element in list */
      List_Elem_T *ll_elem = LL_find_by_type_n_name (GUI_item_list, edit->gui_elem_type, name );  

      if ( ll_elem ) {
        /* Element already in list: Update GUI list entry and LVGL object*/
        GUI_update_entry_Core1(ll_elem, data, edit);
      } else {
        /* Element not in list: Create GUI list entry and LVGL obj */
        GUI_create_entry_Core1(data, edit);
      }
    }
#endif /* RP2040_M0_1 || defined(CORE1_SIM) */

#endif /* USE_GUI_INTERFACE > 0 */ 

