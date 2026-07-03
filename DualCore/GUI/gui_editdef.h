/**
 ******************************************************************************
 * @file    gui_editdef.h
 * @author  Rainer
 * @brief   Edit Receipes for all GUI Elements, ie
 *
 *          IMPORTANT: Used by both Core1 and Core0. When modifying, 
 *          be sure to recompileboth cores
 ******************************************************************************
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _GUI_EDITDEF_H_
#define _GUI_EDITDEF_H_
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
} GUI_E_Datatype_Enum;


/* Receipe for one Edit-Element                                                   */
typedef struct {
  uint8_t            propnum;       /* number of property as defined in gui_def.h */
  const char         *elem_name;    /* User freindly name of GUI-Element          */
  GUI_E_Datatype_Enum elem_type;    /* Data type of GUI-Element                   */
  uint16_t            elem_offset;  /* offs of data element within data vector    */ 
} Edit_Receipe_T;

/* Receipe for a complete GUI-Element                                             */
typedef struct {
  uint16_t count;               /* number of elements in "receipe" array          */
  GUI_Edit_Enum gui_elem_type;  /* type of gui element                            */
  uint16_t   used_ofs;          /* offset of "used" bitfield in data structure    */
  uint16_t   name_ofs;          /* offset of "name" string in data structure      */
  uint16_t   total_size;        /* total size of GUI-Element data structure       */
  Edit_Receipe_T receipe[];     /* array edit receipes for all gui elements       */
} GUI_Edit_T;

/* The edit receipes for all known GUI elements                                   */
extern const GUI_Edit_T edit_style;        /* Edit receipe for style variables    */
extern const GUI_Edit_T edit_label;        /* Edit receipe for label variables    */
extern const GUI_Edit_T edit_arc;          /* Edit receipe for arc variables      */

/* Get the receipe for a GUI elemnt type                                          */
const GUI_Edit_T    *GUI_edit_get_receipe_for_elemtype( GUI_Edit_Enum gui_elemtype );
/* Find a Receipe by name or property index                                       */
const Edit_Receipe_T *FindReceipeByName   ( const GUI_Edit_T *edit, const char *name);
const Edit_Receipe_T *FindReceipeByPropIdx( const GUI_Edit_T *edit, uint8_t prop_idx);

#endif /* USE_GUI_INTERFACE > 0 */
#endif /* _GUIEDITDEF_H_ */