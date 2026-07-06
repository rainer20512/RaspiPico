#include "../GUI/gui_lists.h"
#if USE_GUI_INTERFACE > 0

#include <stdio.h>
#include <string.h>

#include "system/util.h"

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
List_Elem_T *LL_New_Element( GUI_Edit_Enum type, void *lvgl_obj, char *name, void *entry, uint16_t additional )
{
    List_Elem_T *newentry = my_malloc(sizeof(List_Elem_T));
    if ( newentry )  { 
      newentry->ll_type       = type;
      newentry->ll_lvgl_obj   = lvgl_obj;
      newentry->ll_name       = name; 
      newentry->ll_entry      = entry;
      newentry->ll_additional = additional;
      /* Not insertet in any list at this point */
      newentry->ll_next       = NULL;
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
List_Elem_T *LL_find_nth ( List_Elem_T *llist, GUI_Edit_Enum search_type, uint32_t position  )
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
List_Elem_T *LL_iterate_by_type ( List_Elem_T *llist, GUI_Edit_Enum search_type )
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
List_Elem_T *LL_find_by_type_n_name ( List_Elem_T *llist, GUI_Edit_Enum search_type, const char *name )
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
List_Elem_T *LL_find_by_type_name_additional ( List_Elem_T *llist, GUI_Edit_Enum search_type, const char *name, uint32_t additional )
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
List_Elem_T *LL_find_by_type_n_obj  ( List_Elem_T *llist, GUI_Edit_Enum search_type, void *lvgl_obj )
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
          my_free(delptr);
          return;
      }
      llist = &(*llist)->ll_next;
    }
}

#if DEBUG_GUIEDIT > 0

  #include "debug/debug_helper.h"

  /*-----------------------------------------------------------------------------
   * @brief  delete the list entry delptr points to 
   * @param  llist  - linked list to use
   * @param  delptr - element to be removed
   *         initialize to list head
   * @retval ptr to found entry
   * @note   to progress, caller must use LL_next
   *---------------------------------------------------------------------------*/
  void LL_Dump ( List_Elem_T *llist  )
  {
      uint32_t cnt=0;
       while ( llist) {
         DEBUG_PRINTF("%2d: %15s %20s ", cnt, EditNames[llist->ll_type], llist->ll_name);
         if ( llist->ll_type == GUI_ELEM_FONT )
            DEBUG_PRINTF("%5d",llist->ll_additional);
         else
            DEBUG_PRINTF("     ");
         DEBUG_PRINTF(" 0x%p\n", llist->ll_lvgl_obj); 
         cnt++;
         llist = llist->ll_next;
      }
  }
    
#endif /* DEBUG_GUIEDIT > 0 */    




#endif /* USE_LVGL > 0 */ 

