/**
 ******************************************************************************
 * @file    dp_lists.h
 * @author  Rainer
 * @brief   Handling of Datapoints
 ******************************************************************************
 *
 ******************************************************************************
 */

#ifndef _DP_LISTS_H_
#define _DP_LISTS_H_
#include "config/config.h"

#include "parser_specs.h"
#include "../GUI/gui_def.h"
#include "../GUI/gui_editdef.h"
#include "../GUI/variant.h"

/******************************************************************************
 *  Structure to describe one Datapoint ( plus list ptr )
 *****************************************************************************/     
typedef struct DPList_Elem{
  struct DPList_Elem  *next;                      /* ptr to next entry in linked list         */
  char                dpname[ID_MAXNAMELEN];      /* name of datapoint                        */
  void                *lvglobj ;                  /* associated lvgl-object                   */
  GUI_Edit_Enum       elemtype;                   /* element type of lvgl object              */
  uint8_t             propidx;                    /* property idx datapoint refers to         */
  uint8_t             dtID;                       /* unique ID of dp, assigned by insert      */
  XML_Datatype_Enum datatype;                   /* Datatype the datapoint expects           */
} DPList_Elem_T;

#if  RP2040_M0_1 || defined(CORE1_SIM)
  /* linked list of all data points, Core1 view, initially empty */
  extern DPList_Elem_T* GUI_Dp_list;    /* Original datapoint List, hosted in Core1 RAM only */
#endif
#if  RP2040_M0_0
  /* linked list of all data points, Core0 view, R/O for Core0 */
  extern DPList_Elem_T** REF_Dp_list;    /* Ptr to GUI_Dp_list in Core1 RAM, used r/o by Core0 */
#endif

/* in CORE_1_SIM mode, GUI objects are accessible via REF_item_list */
#if  RP2040_M0_0
    #define GUI_DP_LIST     *REF_Dp_list
#elif  RP2040_M0_1
    #define GUI_DP_LIST     GUI_Dp_list
#endif

/******************************************************************************
 *  Functions on data points (and data point list)
 *****************************************************************************/     
DPList_Elem_T *DP_Setup_ListElem      (char *dpname, char *elemname, char *propname);
DPList_Elem_T *DP_Find                (DPList_Elem_T *list);
DPList_Elem_T *DP_Find_ID             (DPList_Elem_T *list, uint8_t id);
DPList_Elem_T *DP_Append              (DPList_Elem_T **list);
void          DP_Reset                (DPList_Elem_T **list);
bool          DP_Update_Core0         (uint8_t id, Variant_T *v);
bool          DP_Update_Core1         (uint8_t id, Variant_T *v);
void          Core1_Receive_DP_update (uint8_t *data, uint16_t buflen);

#if DEBUG_GUIEDIT > 0
  void DP_Dump ( DPList_Elem_T *llist  );
#else
  #define DP_Dump()
#endif

#endif /* _DP_LISTS_H_ */