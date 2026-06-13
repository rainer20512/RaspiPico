#include "../GUI/gui_lists.h"
#if USE_LVGL > 0

#include <stdio.h>
#include <string.h>

/* linked list of all GUI elements, initially empty */
List_Elem_T * GUI_item_list = NULL;

/* Append a new List entry at the end of list */
List_Elem_T *LL_append( List_Elem_T **llist, GUI_Elem_T type, char *name, void *entry )
{
    List_Elem_T *newentry = malloc(sizeof(List_Elem_T));
    if ( !newentry )  { 
      printf("malloc failed");
      return NULL;
    }

    /* create new entry */
    newentry->ll_type  = type;
    newentry->ll_name  = name; 
    newentry->ll_entry = entry;
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
        if ( position-- == 0 ) break;
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

