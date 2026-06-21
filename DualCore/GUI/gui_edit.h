
#ifndef _GUI_EDIT_H_
#define _GUI_EDIT_H_
#include "config/config.h"

#if USE_LVGL > 0
#include "../../lvgl/lvgl.h"
#include "gui_def.h"

/* Recognized data types */
typedef enum {
  GUI_UINT8  = 0,
  GUI_UINT16 = 1,
  GUI_RGB888 = 2,
  GUI_UINT32 = 3,
  GUI_INT8   = 4,
  GUI_INT16  = 5,
  GUI_INT32  = 6,
  GUI_STRING = 7,
  GUI_STYLE  = 8,
  GUI_FONT   = 9,
} GUI_edittype_T;

/* and corresponding length in bytes          */
/* enum and const must match in element count */
#define BYTELENGTHS  {1, 2, 3, 4, 1, 2, 3, 4, 4, 4}

/* Receipe for one Edit-Element                                               */
typedef struct {
  const char      *elem_name;   /* User freindly name of GUI-Element          */
  GUI_edittype_T  elem_type;    /* Data type of GUI-Element                   */
  uint16_t        elem_offset;  /* offset of data element within data vector  */ 
} GUI_editelem_T;

/* Receipe for a complete GUI-Element                                          */
typedef struct {
  uint16_t count;               /* number of elements in "gui_element" array   */
  GUI_Elem_T gui_elem_type;     /* type of gui element                         */
  uint16_t   used_ofs;          /* offset of "used" bitfield in data structure */
  uint16_t   name_ofs;          /* offset of "name" string in data structure   */
  uint16_t   total_size;        /* total size of GUI-Element data structure    */
  GUI_editelem_T gui_element[]; /* array of GUI-Element description            */
} GUI_Edit_T;

typedef void (*OnExitFn) ( void );

/* For test/edit purposes: One constant element type as default setting        */
/* and one current element type which is modified when editing                 */

extern const GUI_Style_T def_style;             /* default style settings */
extern       GUI_Style_T cur_style;             /* actual style settings  */

extern const GUI_Label_T def_label;             /* default label settings */
extern       GUI_Label_T cur_label;             /* actual label settings  */

extern const GUI_Arc_T def_arc;             /* default arc settings */
extern       GUI_Arc_T cur_arc;             /* actual arc settings  */


/* The edit receipes for all known GUI elements                                */
extern const GUI_Edit_T edit_style;             /* Edit receipe for style variables */
extern const GUI_Edit_T edit_label;             /* Edit receipe for label variables */
extern const GUI_Edit_T edit_arc;               /* Edit receipe for arc variables */


void GUI_Edit( const GUI_Edit_T *edit, void *anyobj, OnExitFn OnExit );

#endif /* USE_LVGL */
#endif /* _GUIDEF_H_ */