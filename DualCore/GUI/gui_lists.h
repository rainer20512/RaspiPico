
#ifndef _GUI_LISTS_H_
#define _GUI_LISTS_H_
#include "config/config.h"

#if USE_LVGL > 0
#include "../../lvgl/lvgl.h"

#include <malloc.h>

#include "../GUI/gui_def.h"


#define LL_MAX_NAMELEN 20

typedef struct List_Elem{
  GUI_Elem_T         ll_type;      /* type of list entry */
  char             *ll_name;       /* ptr to friendly name of entr, may be a part of data entry */
  uint32_t         ll_additional;  /* optional second identifying property                      */
  void             *ll_entry;      /* ptr to the entry itself, typeless */
  void             *ll_lvgl_obj;   /* corresponding lvgl object ( style or lvgl_obj ), untyped here */           
  struct List_Elem *ll_next;       /* ptr to next entry in linked list */
} List_Elem_T;

/* linked list of all GUI elements, initially empty */
extern List_Elem_T* GUI_item_list;

List_Elem_T *LL_New_Element( GUI_Elem_T type, void *lvgl_obj, char *name, void *entry, uint32_t additional );
List_Elem_T *LL_append( List_Elem_T **llist, List_Elem_T *newentry );
List_Elem_T *LL_find_nth ( List_Elem_T *llist, GUI_Elem_T search_type, uint32_t position );
List_Elem_T *LL_next ( List_Elem_T *llist);
List_Elem_T *LL_iterate_by_type ( List_Elem_T *llist, GUI_Elem_T search_type );
List_Elem_T *LL_find_by_type_n_name ( List_Elem_T *llist, GUI_Elem_T search_type, const char *name );
List_Elem_T *LL_find_by_type_name_additional ( List_Elem_T *llist, GUI_Elem_T search_type, const char *name, uint32_t additional );
List_Elem_T *LL_find_by_type_n_obj  ( List_Elem_T *llist, GUI_Elem_T search_type, void *lvgl_obj );

void        LL_delete ( List_Elem_T **llist, List_Elem_T *delptr );


#endif /* USE_LVGL */
#endif /* _GUI_LISTS_H_ */