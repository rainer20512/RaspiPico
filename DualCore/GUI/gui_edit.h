
#ifndef _GUI_EDIT_H_
#define _GUI_EDIT_H_
#include "config/config.h"

#if USE_LVGL > 0
#include "../../lvgl/lvgl.h"

/* Recognized data types */
typedef enum {
  GUI_UINT8  = 0,
  GUI_UINT16 = 1,
  GUI_RGB888 = 2,
  GUI_UINT32 = 3,
  GUI_STRING = 4,
} GUI_edittype_T;

/* and corresponding length in bytes          */
/* enum and const must match in element count */
#define BYTELENGTHS  {1, 2, 3, 4, 4}

typedef struct {
  const char      *elem_name;
  GUI_edittype_T  elem_type;
  uint16_t        elem_offset;
} GUI_editelem_T;

typedef struct {
  uint16_t count;
  const char *editname;
  GUI_editelem_T gui_element[];
} GUI_Edit_T;

typedef void (*OnExitFn) ( void );

void GUI_Edit( const GUI_Edit_T *edit, void *anyobj, OnExitFn OnExit );

#endif /* USE_LVGL */
#endif /* _GUIDEF_H_ */