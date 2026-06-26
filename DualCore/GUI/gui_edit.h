
#ifndef _GUI_EDIT_H_
#define _GUI_EDIT_H_
#include "config/config.h"

#if USE_GUI_INTERFACE > 0
#include "../../lvgl/lvgl.h"
#include "gui_def.h"

/* Recognized data types */
typedef enum {
  GUI_UINT8   = 0,
  GUI_UINT16  = 1,
  GUI_RGB888  = 2,
  GUI_UINT32  = 3,
  GUI_INT8    = 4,
  GUI_INT16   = 5,
  GUI_INT32   = 6,
  GUI_STRING  = 7,
  GUI_STYLE   = 8,
  GUI_FONT    = 9,
  GUI_MAXELEM = 10,
} GUI_E_Type_Enum;

typedef struct {
  GUI_E_Type_Enum   e;  /* GUI Edit Type                                                                 */
  uint8_t is_obj;       /* set to TRUE, if datatype is a ptr to LVGL obj ( like style, label, arc, ... ) */
                        /* strings and fonts are excluded, they are handeled directly                    */
  uint8_t bytelen;      /* length of datatype in bytes                                                   */
  GUI_Elem_T  gui_elem; /* GUI element type that is represented by that object                           */
} GUI_Edit_TypeSpec_T;

/* and corresponding length in bytes          */
/* enum and const must match in element count */
/* the index and thus the order must match the order of Enum above */
#define GUI_TYPE_SPECS  {\
  {GUI_UINT8, 0, 1, 0,},    {GUI_UINT16, 0, 2, 0,}, {GUI_RGB888, 0, 3, 0,}, {GUI_UINT32, 0, 4, 0,},\
  {GUI_INT8, 0, 1, 0,},     {GUI_INT16, 0, 2, 0,},                       {GUI_INT32, 0, 4, 0,},\
  {GUI_STRING, 0, 4, 0, },  {GUI_STYLE, 1, 4, GUI_ELEM_STYLE},  {GUI_FONT, 1, 4, GUI_ELEM_FONT },\
}

extern const GUI_Edit_TypeSpec_T GUI_TypeSpec[GUI_MAXELEM];

/* Macro to retrieve the byte length of a GUI element, unsafe, bcs no range checking of idx */
#define GET_GUI_ELEM_LEN(e)    (GUI_TypeSpec[e].bytelen)
/* Macro to retrieve the GUI Element Type GUI element, unsafe, bcs no range checking of idx */
#define GET_GUI_ELEM_TYPE(e)   (GUI_TypeSpec[e].gui_elem)
/* check whether a GUI element is an object, unsafe, bcs no range checking of idx */
#define IS_GUI_ELEM_BIN(e)     (GUI_TypeSpec[e].is_obj)



/* Receipe for one Edit-Element                                               */
typedef struct {
  const char      *elem_name;   /* User freindly name of GUI-Element          */
  GUI_E_Type_Enum elem_type;    /* Data type of GUI-Element                   */
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
const GUI_Edit_T *GUI_edit_get_receipe_for_elemtype( GUI_Elem_T gui_type );
#endif /* USE_LVGL */
#endif /* _GUIDEF_H_ */