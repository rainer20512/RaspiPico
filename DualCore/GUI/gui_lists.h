
#ifndef _GUI_LISTS_H_
#define _GUI_LISTS_H_
#include "config/config.h"

#if USE_GUI_INTERFACE > 0
#include "../../lvgl/lvgl.h"

#include "../GUI/gui_def.h"

typedef struct List_Elem{
  GUI_Edit_Enum    ll_type;        /* type of list entry */
  char             *ll_name;       /* ptr to friendly name of entr, may be a part of data entry */
  uint16_t         ll_additional;  /* optional second identifying property                      */
  void             *ll_entry;      /* ptr to the entry itself, typeless */
  void             *ll_lvgl_obj;   /* corresponding lvgl object ( style or lvgl_obj ), untyped here */           
  struct List_Elem *ll_next;       /* ptr to next entry in linked list */
} List_Elem_T;

/* linked list of all GUI elements, initially empty */
#if  RP2040_M0_1 || defined(CORE1_SIM)
  extern List_Elem_T* GUI_item_list;    /* Original GUI item List, hosted in Core1 RAM only */
#endif
#if  RP2040_M0_0
  extern List_Elem_T* REF_item_list;    /* Ptr to GUI_item_list in Core1 RAM, used r/o by Core0 */
#endif

/* in CORE_1_SIM mode, GUI objects are accessible via REF_item_list */
#if  RP2040_M0_0
    #define GUI_ITEM_LIST     REF_item_list
#elif  RP2040_M0_1
    #define GUI_ITEM_LIST     GUI_item_list
#endif


List_Elem_T *LL_New_Element( GUI_Edit_Enum type, void *lvgl_obj, char *name, void *entry, uint16_t additional );
List_Elem_T *LL_append( List_Elem_T **llist, List_Elem_T *newentry );
List_Elem_T *LL_find_nth ( List_Elem_T *llist, GUI_Edit_Enum search_type, uint32_t position );
List_Elem_T *LL_next ( List_Elem_T *llist);
List_Elem_T *LL_iterate_by_type ( List_Elem_T *llist, GUI_Edit_Enum search_type );
List_Elem_T *LL_find_by_type_n_name ( List_Elem_T *llist, GUI_Edit_Enum search_type, const char *name );
List_Elem_T *LL_find_by_type_name_additional ( List_Elem_T *llist, GUI_Edit_Enum search_type, const char *name, uint32_t additional );
List_Elem_T *LL_find_by_type_n_obj  ( List_Elem_T *llist, GUI_Edit_Enum search_type, void *lvgl_obj );
void        LL_delete ( List_Elem_T **llist, List_Elem_T *delptr );

#if DEBUG_GUIEDIT > 0
  void LL_Dump ( List_Elem_T *llist  );
#else
  #define LL_Dump()
#endif
#endif /* USE_LVGL */
#endif /* _GUI_LISTS_H_ */