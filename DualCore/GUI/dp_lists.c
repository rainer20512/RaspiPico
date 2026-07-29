#include "../GUI/dp_lists.h"
#if USE_LVGL > 0

#include <stdio.h>
#include <string.h>

#include "system/util.h"
#include "../GUI/gui_lists.h"


/* linked list of all datapoints, initially empty */
#if  RP2040_M0_1 || defined(CORE1_SIM)
    DPList_Elem_T * GUI_Dp_list = NULL;
#endif
#if  RP2040_M0_0
    /* Copy if GUI_item_list in Core0, it is read only!! */
    DPList_Elem_T ** REF_Dp_list = NULL;
#endif

/* static list element, filled by "DP_Create" */

/* module global variable to store on data point list element for various operations */
static DPList_Elem_T work;
/* monotonic upcounter to get unique datapoint IDs */
static uint8_t nextID = 0;

/*-----------------------------------------------------------------------------
 * @brief for a given element and property name find corresponding LVGL obj,
*         element type, property index, and data type and store this three 
*         items in modul global variable "work 
 * @param elemname - GUI element name
 * @param propname - property name
 * @retval - true, if elment and property name are found, false if not     
 *---------------------------------------------------------------------------*/
static bool DP_Find_obj_and_propidx ( char *elemname, char *propname )
{
  /* Find GUI Element by name*/
  List_Elem_T *ll = LL_find_by_type_n_name ( GUI_ITEM_LIST, GUI_ELEM_NOTYPE, elemname );
  if ( !ll ) return false;

  /* get edit Info */
  const GUI_Edit_T *edit = Find_EditInfoByType( ll->ll_type );
  if ( !edit ) return false;

  /* Get prop idx */
  int32_t idx = GetReceipeIdxByName( edit, propname, strlen(propname));
  if ( idx < 0 ) return false;

  /* set all return data */
  work.elemtype = ll->ll_type;
  work.lvglobj  = ll->ll_lvgl_obj;
  work.propidx  = (uint8_t)idx;
  work.datatype = edit->receipe[idx].elem_type;
  return true;
}


/*-----------------------------------------------------------------------------
 * @brief for a given element and property name set up all items of modul  
 *        global variable "work" except next ptr and ID field
 * @param dpname   - datapoint name
 * @param elemname - GUI element name
 * @param propname - property name
 * @retval - true, if element and property name are found, false if not
 *           in the latter case, the content of "work" is invalid     
 *---------------------------------------------------------------------------*/
DPList_Elem_T *DP_Setup_ListElem(char *dpname, char *elemname, char *propname)
{
    bool ret = DP_Find_obj_and_propidx( elemname, propname );
    if ( !ret ) return NULL;

    /* lvgl_obj, elemtype, prop_idx, datatype are set by previous call, */
    /* set dpname, reset ID and "next" ptr          */
    strcpy(work.dpname, dpname );
    work.dtID = 0;
    work.next = NULL;

    return &work;
}

/*-----------------------------------------------------------------------------
 * @brief returns true, if for datapoints l1 and l2 elemtype, lvglobj, propidx 
 *        and name are identical
 *---------------------------------------------------------------------------*/
static bool DP_IsEuqal( DPList_Elem_T *l1, DPList_Elem_T *l2 )
{
    return l1->elemtype == l2->elemtype && l1->lvglobj == l2->lvglobj 
        && l1->propidx  == l2->propidx  && strcmp(l1->dpname,l2->dpname) == 0;
}

/*-----------------------------------------------------------------------------
 * @brief Searches for a datapoint, whose elemtype, lvglobj, propidx and name is 
 *        identical to the values in modul global variable "work"
 * @retval - datapoint list element, if found, NULL if not
 *---------------------------------------------------------------------------*/
DPList_Elem_T *DP_Find(DPList_Elem_T *list)
{
    while (list) {
      if ( DP_IsEuqal(list, &work) ) return list;
      list = list->next;
    }
    return NULL;
}

/*-----------------------------------------------------------------------------
 * @brief Append a copy of modul global variable "work" to the end of datapoint list
 * @param list - ptr of the list to appent to
 * @retval - ptr to new list element if successful, NULL if not 
 *---------------------------------------------------------------------------*/
DPList_Elem_T *DP_Append( DPList_Elem_T **list)
{
    /* Create a copy of "work" on heap */
    DPList_Elem_T *newentry = my_malloc(sizeof(DPList_Elem_T));
    if ( !newentry ) return NULL;

    *newentry = work;
    /* newentry will be the last list entry */
    newentry->next = NULL;
    newentry->dtID = ++nextID;
 
    /* Append to list */
    while ( *list != NULL )
      list = &(*list)->next;

    /* Append */
    *list = newentry;
    return newentry;
}

#if 0

/*-----------------------------------------------------------------------------
 * @brief  Find a list entry by type and position in list 
 * @param  search_type - EntryType to search for, LL_NOTYPE = any type
 * @param  position - nth entry in the list of that type, start from beginning
 *         first position is 1!
 * retval  ptr to found entry, NULL if not found
 *---------------------------------------------------------------------------*/
DPList_Elem_T *LL_find_nth ( DPList_Elem_T *llist, GUI_Edit_Enum search_type, uint32_t position  )
{
     
     if ( position < 1 ) return NULL;
     while ( llist) {
      if ( search_type == GUI_ELEM_NOTYPE || search_type == llist->ll_type ) {
        if ( --position == 0 ) break;
      }
      llist = llist->ll_next;
     }

     return llist;
}
    
/*-----------------------------------------------------------------------------
 * @brief  return ptr to next list element
 * @note   only to hide the internal 'next' field
 *---------------------------------------------------------------------------*/
DPList_Elem_T *LL_next ( DPList_Elem_T *llist)
{
  if ( llist ) 
    return llist->ll_next;
  else
    return NULL;
}

/*-----------------------------------------------------------------------------
 * @brief  Iterate thru all list entries of a certain type 
 * @param  search_type - EntryType to search for, LL_NOTYPE = any type
 * @param  ptr - pointer to the actual list element,
 *         initialize to list head
 * @retval ptr to found entry
 * @note   to progress, caller must use LL_next
 *---------------------------------------------------------------------------*/
DPList_Elem_T *LL_iterate_by_type ( DPList_Elem_T *llist, GUI_Edit_Enum search_type )
{
     while ( llist) {
      if ( search_type == GUI_ELEM_NOTYPE || search_type == llist->ll_type ) return llist;
      llist = llist->ll_next;
    }
    return llist;
}
    
/*-----------------------------------------------------------------------------
 * @brief  Find element of certain Type and Name
 * @param  llist       - ptr to linked list head
 * @param  search_type - EntryType to search for, LL_NOTYPE = any type
 * @param  name        - name to search for (case sensitive )
 * @retval ptr to found entry, NULL if no match
 *---------------------------------------------------------------------------*/
DPList_Elem_T *LL_find_by_types_n_name ( DPList_Elem_T *llist)
{   
     while ( llist) {
      if ( (search_type == GUI_ELEM_NOTYPE || search_type == llist->ll_type ) && strcmp(llist->ll_name, name) == 0 ) return llist;
      llist = llist->ll_next;
    }
    return NULL;
}

/*-----------------------------------------------------------------------------
 * @brief  Find element of certain Type and additional info and optionally Name
 * @param  llist       - ptr to linked list head
 * @param  search_type - EntryType to search for, LL_NOTYPE = any type
 * @param  name        - name to search for (optional, case sensitive )
 * @param  additional  - additional property 
 * @retval ptr to found entry, NULL if no match
 *---------------------------------------------------------------------------*/
DPList_Elem_T *LL_find_by_type_name_additional ( DPList_Elem_T *llist, GUI_Edit_Enum search_type, const char *name, uint32_t additional )
{   
    bool found;
    while ( llist) {
       /* An object is found, if no type specified or types match */     
       found = (search_type == GUI_ELEM_NOTYPE || search_type == llist->ll_type );
       /* additional must match exactly */
       if ( found ) found = additional == llist->ll_additional;
       /* if name is specified, they must match exactly */
       if ( found && name ) found = strcmp(llist->ll_name, name) == 0;
       /* additional must match exactly */
       if ( found ) found = additional == llist->ll_additional;
       if ( found ) return llist;

       llist = llist->ll_next;
    }
    return NULL;
}


/*-----------------------------------------------------------------------------
 * @brief  Find element of certain Type and associated lvgl object
 * @param  llist       - ptr to linked list head
 * @param  search_type - EntryType to search for, LL_NOTYPE = any type
 * @param  lvgl_obj    - ptr to lvgl obj
 * @retval ptr to found entry, NULL if no match
 *---------------------------------------------------------------------------*/
DPList_Elem_T *LL_find_by_type_n_obj  ( DPList_Elem_T *llist, GUI_Edit_Enum search_type, void *lvgl_obj )
{
     while ( llist) {
      if ( (search_type == GUI_ELEM_NOTYPE || search_type == llist->ll_type ) && llist->ll_lvgl_obj == lvgl_obj ) return llist;
      llist = llist->ll_next;
    }
    return NULL;
}

/*-----------------------------------------------------------------------------
 * @brief  delete the list entry delptr points to 
 * @param  llist  - linked list to use
 * @param  delptr - element to be removed
 *         initialize to list head
 * @retval ptr to found entry
 * @note   to progress, caller must use LL_next
 *---------------------------------------------------------------------------*/
void LL_delete ( DPList_Elem_T **llist, DPList_Elem_T *delptr )
{
    if ( !delptr ) return;
     while ( *llist) {
      if ( *llist == delptr ) {
          /* unlink and delete List element */
          *llist = delptr->ll_next;
          my_free(delptr);
          return;
      }
      llist = &(*llist)->ll_next;
    }
}
#endif

#if DEBUG_GUIEDIT > 0

  #include "debug/debug_helper.h"

  void DP_Dump ( DPList_Elem_T *llist  )
  {
      const GUI_Edit_T *edit;
      while ( llist) {
         edit = Find_EditInfoByType( llist->elemtype );
         DEBUG_PRINTF("%3d: %16s %16s %16s %2d\n", llist->dtID, llist->dpname, Editinfo[llist->elemtype].name, edit->receipe[llist->propidx].elem_name, llist->datatype);
         llist = llist->next;
      }
  }
    
#endif /* DEBUG_GUIEDIT > 0 */    




#endif /* USE_LVGL > 0 */ 

