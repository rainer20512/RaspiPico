
/**
 ******************************************************************************
 * @file    gui_edit.h
 * @author  Rainer
 * @brief   Interface for simple gui element editing via command line
 *
 ******************************************************************************
 *
 ******************************************************************************
 */
#ifndef _GUI_EDIT_H_
#define _GUI_EDIT_H_
#include "config/config.h"

#if USE_GUI_INTERFACE > 0
#include "../../lvgl/lvgl.h"
#include "gui_def.h"
#include "gui_editdef.h"

/*************************************************************************************************************
 * Description of every known GUI-Datatype 
 *************************************************************************************************************/
typedef struct {
  GUI_E_Datatype_Enum e;    /* GUI Element Type                                                              */
  uint8_t is_obj;           /* set to TRUE, if datatype is a ptr to LVGL obj ( like style, label, arc, ... ) */
                            /* strings and fonts are excluded, they are handeled directly                    */
  uint8_t bytelen;          /* length of datatype in bytes                                                   */
  GUI_Edit_Enum gui_elem;   /* GUI element type that is represented by that object                           */
} GUI_Edit_TypeSpec_T;

/* enum and const must match in element count                      */
/* the index and thus the order must match the order of Enum above */
#define GUI_TYPE_SPECS  {\
  {GUI_UINT8, 0, 1, 0,},            {GUI_UINT16, 0, 2, 0,},             {GUI_RGB888, 0, 3, 0,},\
  {GUI_UINT32, 0, 4, 0,},           {GUI_INT8, 0, 1, 0,},               {GUI_INT16, 0, 2, 0,},\
  {GUI_INT32, 0, 4, 0,},            {GUI_STRING, 0, 4, 0, },            {GUI_STYLE, 1, 4, GUI_ELEM_STYLE},\
  {GUI_FONT, 1, 4, GUI_ELEM_FONT },\
}

extern const GUI_Edit_TypeSpec_T GUI_TypeSpec[GUI_MAXELEM];

/* Macro to retrieve the byte length of a GUI element, unsafe, bcs no range checking of idx */
#define GET_GUI_ELEM_LEN(e)    (GUI_TypeSpec[e].bytelen)
/* Macro to retrieve the GUI Element Type GUI element, unsafe, bcs no range checking of idx */
#define GET_GUI_ELEM_TYPE(e)   (GUI_TypeSpec[e].gui_elem)
/* check whether a GUI element is an object, unsafe, bcs no range checking of idx */
#define IS_GUI_ELEM_BIN(e)     (GUI_TypeSpec[e].is_obj)

/* For test/edit purposes: One constant element type as default setting        */
/* and one current element type which is modified when editing                 */

extern const GUI_Style_T def_style;             /* default style settings      */
extern       GUI_Style_T cur_style;             /* actual style settings       */

extern const GUI_Label_T def_label;             /* default label settings      */
extern       GUI_Label_T cur_label;             /* actual label settings       */

extern const GUI_Arc_T def_arc;             /* default arc settings */
extern       GUI_Arc_T cur_arc;             /* actual arc settings  */


typedef void (*OnExitFn) ( void );
void      GUI_edit_dump_all    (const GUI_Edit_T *editdata, bool padded );
bool      GUI_Edit_SetItem     (char *arg, size_t argsize, const GUI_Edit_T *edit, uint32_t idx );
uint32_t  GUI_Edit_SetUsedBits ( const GUI_Edit_T *editdata, uint32_t andMask, uint32_t orMask );
void      GUI_Edit             ( const GUI_Edit_T *edit, OnExitFn OnExit );
#endif /* USE_GUI_INTERFACE > 0 */
#endif /* _GUIDEF_H_ */