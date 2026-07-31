
#ifndef _DP_LISTS_H_
#define _DP_LISTS_H_
#include "config/config.h"

#if USE_LVGL > 0
#include "../../lvgl/lvgl.h"

#include "parser_specs.h"
#include "../GUI/gui_def.h"
#include "../GUI/gui_editdef.h"
#include "../GUI/variant.h"


typedef struct DPList_Elem{
  struct DPList_Elem  *next;                      /* ptr to next entry in linked list         */
  char                dpname[ID_MAXNAMELEN];      /* name of datapoint                        */
  lv_obj_t            *lvglobj ;                  /* associated lvgl-object                   */
  GUI_Edit_Enum       elemtype;                   /* element type of lvgl object              */
  uint8_t             propidx;                    /* property idx datapoint refers to         */
  uint8_t             dtID;                       /* unique ID of dp, assigned by insert      */
  GUI_E_Datatype_Enum datatype;                   /* Datatype the datapoint expects           */
} DPList_Elem_T;

/* linked list of all GUI elements, initially empty */
#if  RP2040_M0_1 || defined(CORE1_SIM)
  extern DPList_Elem_T* GUI_Dp_list;    /* Original datapoint List, hosted in Core1 RAM only */
#endif
#if  RP2040_M0_0
  extern DPList_Elem_T** REF_Dp_list;    /* Ptr to GUI_Dp_list in Core1 RAM, used r/o by Core0 */
#endif

/* in CORE_1_SIM mode, GUI objects are accessible via REF_item_list */
#if  RP2040_M0_0
    #define GUI_DP_LIST     *REF_Dp_list
#elif  RP2040_M0_1
    #define GUI_DP_LIST     GUI_Dp_list
#endif

DPList_Elem_T *DP_Setup_ListElem (char *dpname, char *elemname, char *propname);
DPList_Elem_T *DP_Find           (DPList_Elem_T *list);
DPList_Elem_T *DP_Find_ID        (DPList_Elem_T *list, uint8_t id);
DPList_Elem_T *DP_Append         ( DPList_Elem_T **list);
void          DP_Reset           (DPList_Elem_T **list);
bool          DP_Update          ( uint8_t id, Variant_T *v );

#if DEBUG_GUIEDIT > 0
  void DP_Dump ( DPList_Elem_T *llist  );
#else
  #define DP_Dump()
#endif
#endif /* USE_LVGL */
#endif /* _DP_LISTS_H_ */