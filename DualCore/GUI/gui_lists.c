#include "../GUI/gui_lists.h"
#if USE_LVGL > 0

#include <stdio.h>
#include <string.h>

/* linked list of all GUI elements, initially empty */
List_Elem_T * GUI_item_list = NULL;

/*-----------------------------------------------------------------------------
 * @brief Create a new List entry from heap and initialize all fields, 
 * @param type     - Type of associated GUI element
 * @param lvgl_obj - corresponding lvgl object ( lv_font_t, style or lv_obj_t ), or NULL
 *                   if no lvgl object is associated so far
 * @param name     - ptr to objects name within object data 
 * @param entry    - typeless ptr to GUI element data
 * @retval - new entry or NULL when no memory on heap     

 * @note  no insertion into list
 *---------------------------------------------------------------------------*/
List_Elem_T *LL_New_Element( GUI_Elem_T type, void *lvgl_obj, char *name, void *entry )
{
    List_Elem_T *newentry = malloc(sizeof(List_Elem_T));
    if ( !newentry )  { 
      printf("malloc failed");
    } else {
      newentry->ll_type      = type;
      newentry->ll_lvgl_obj  = lvgl_obj;
      newentry->ll_name      = name; 
      newentry->ll_entry     = entry;
      /* Not insertet in any list at this point */
      newentry->ll_next      = NULL;
    }
    return newentry;
}

/* Append a new List entry at the end of list */
List_Elem_T *LL_append( List_Elem_T **llist, List_Elem_T *newentry )
{
    /* Its the last entry ! */
    newentry->ll_next  = NULL;

    /* Append to list */
    while ( *llist != NULL )
      llist = &(*llist)->ll_next;

    /* Append */
    *llist = newentry;
    return newentry;
}

/*-----------------------------------------------------------------------------
 * @brief  Find a list entry by type and position in list 
 * @param  search_type - EntryType to search for, LL_NOTYPE = any type
 * @param  position - nth entry in the list of that type, start from beginning
 *         first position is 1!
 * retval  ptr to found entry, NULL if not found
 *---------------------------------------------------------------------------*/
List_Elem_T *LL_find_nth ( List_Elem_T *llist, GUI_Elem_T search_type, uint32_t position  )
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
List_Elem_T *LL_next ( List_Elem_T *llist)
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
List_Elem_T *LL_iterate_by_type ( List_Elem_T *llist, GUI_Elem_T search_type )
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
List_Elem_T *LL_find_by_type_n_name ( List_Elem_T *llist, GUI_Elem_T search_type, const char *name )
{   
     while ( llist) {
      if ( (search_type == GUI_ELEM_NOTYPE || search_type == llist->ll_type ) && strcmp(llist->ll_name, name) == 0 ) return llist;
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
List_Elem_T *LL_find_by_type_n_obj  ( List_Elem_T *llist, GUI_Elem_T search_type, void *lvgl_obj )
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
void LL_delete ( List_Elem_T **llist, List_Elem_T *delptr )
{
    if ( !delptr ) return;
     while ( *llist) {
      if ( *llist == delptr ) {
          /* unlink and delete List element */
          *llist = delptr->ll_next;
          free(delptr);
          return;
      }
      llist = &(*llist)->ll_next;
    }
}
    




#endif /* USE_LVGL > 0 */ 

