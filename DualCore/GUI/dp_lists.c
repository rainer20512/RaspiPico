/**
 ******************************************************************************
 * @file    dp_lists.h
 * @author  Rainer
 * @brief   Handling of Datapoints
 ******************************************************************************
 *
 ******************************************************************************
 */
#include "../GUI/dp_lists.h"

#include <stdio.h>
#include <string.h>

#include "system/util.h"
#include "../GUI/gui_lists.h"
#include "../GUI/lvgl_update.h"

#if DEBUG_DATAPOINTS > 0
    #include "debug/debug_helper.h"
#endif


/* linked list of all datapoints, initially empty */
#if  RP2040_M0_1 || defined(CORE1_SIM)
    DPList_Elem_T * GUI_Dp_list = NULL;
#endif
#if  RP2040_M0_0
    /* Copy if GUI_item_list in Core0, it is read only!! */
    DPList_Elem_T ** REF_Dp_list = NULL;
#endif

#if RP2040_M0_1 || defined(CORE1_SIM) 

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

/*-----------------------------------------------------------------------------
 * @brief Delete all Datapoints in list
 * @param list - Datapoint list to be searched
 *---------------------------------------------------------------------------*/
void DP_Reset(DPList_Elem_T **list)
{
    DPList_Elem_T *current = *list;
    DPList_Elem_T *next;
    while (current) {
      next = current->next;
      my_free(current);
      current = next;
    }
    /* Finally set list ptr to NULL and ID counter to 0*/
    *list  = NULL;
    nextID = 0;
}



/*-----------------------------------------------------------------------------
 * @brief Searches for a datapoint whose ID matches the given ID
 * @param list - Datapoint list to be searched
 * @param id   - ID to search for 
 * @retval - datapoint list element, if found, NULL if not
 *---------------------------------------------------------------------------*/
DPList_Elem_T *DP_Find_ID(DPList_Elem_T *list, uint8_t id)
{
    while (list) {
      if ( list->dtID==id)  return list;
      list = list->next;
    }
    return NULL;
}
#endif /* RP2040_M0_1 || defined(CORE1_SIM) */

/******************************************************************************
*******************************************************************************
* Functions/Structure for transferring Datapoint infos between both cores
*******************************************************************************
******************************************************************************/
#include <assert.h>
#include "system/ipc_msg.h"

/******************************************************************************
 * datapoint IPC transfer buffer 
 * consists of datapoint ID, one Variant plus one string
 * the string storage is neccessary bcs a Variant holds reference to string
 * only, not the string data itself
 * Binary objects cannot be transferred
 *****************************************************************************/
typedef struct {
  uint16_t  buflen;                 /* length of this structure, must be first item */
  uint16_t  xml_version;            /* sender version to ensure compatibilty */
  uint8_t   dpID;                   /* datapoint ID                          */
  Variant_T v;                      /* datapoint value as Variant_T          */
  char      str[ID_MAXNAMELEN];     /* string data in case of VAR_STRING     */
  uint8_t   strlen;                 /* string length in case of VAR_STRING   */
} IPC_DP_Xfer_Buff_T;

/* make sure, the IPC transfer buf is capable of store any GUI element */
static_assert(sizeof(IPC_DP_Xfer_Buff_T) <= IPC_BUFSIZE, "ICP transfer buffer size too small");


#if RP2040_M0_1 || defined(CORE1_SIM)

    static IPC_DP_Xfer_Buff_T ipcrecv;

    /*-----------------------------------------------------------------------------
     * @brief Update datapoint ID <id>
     * @param list - Datapoint list to be searched
     * @param id   - ID to search for 
     * @param v    - new value for that datapoint
     * @retval  true, if datapoint exists, false if not
     *---------------------------------------------------------------------------*/
    bool DP_Update_Core1 ( uint8_t id, Variant_T *v )
    {
        DPList_Elem_T *dp = DP_Find_ID(GUI_Dp_list, id);
        if ( !dp ) return false;

        LVGL_update(dp->lvglobj, dp->propidx, v, dp->elemtype );  
    }

    static bool DP_Unpack( IPC_DP_Xfer_Buff_T *recvbuf,uint8_t *data, uint16_t buflen)
    {
        /* check length */
            if ( buflen != sizeof(IPC_DP_Xfer_Buff_T) ) {
            #if DEBUG_DATAPOINTS > 0
                DEBUG_PRINTF("Err: DP_Unpack: Illegal buf size %d\n", buflen);
            #endif
            return false;
        }

        memcpy_fast(recvbuf, data, buflen);

        /* check version */
        if ( recvbuf->xml_version != XML_PARSER_VERSION_BIN ) {
            #if DEBUG_DATAPOINTS > 0
                DEBUG_PRINTF("Err: DP_Unpack: Parser version mismatch\n");
            #endif
            return false;
        }

        /* in case of string, adjust Variants string pointer */
        if ( recvbuf->v.type == VAR_STRING ) {
            recvbuf->v.str.text = recvbuf->str;
            /* length is unchanged */
       }

        return true;
    }
    /******************************************************************************
     * @brief  Core1 received an Gui element description via IPC. The data is
     *         still stored in IPC_Buff
     *         to free the ipc buf asap, copy content to a IPC_GUI_Xfer_Buff_T
     *         variable and continue processing on that variable
     * @param  data     - ptr to IPC receive buffer 
     * @param  buflen   - number of bytes in that buffer
     ******************************************************************************/
    void Core1_Receive_DP_update(uint8_t *data, uint16_t buflen)
    {
      /* unpack data into recvbuf */
      if ( DP_Unpack( &ipcrecv, data, buflen ) ) {
          /* and update datapoint */
          bool ret = DP_Update_Core1 ( ipcrecv.dpID, &ipcrecv.v );
          #if DEBUG_DATAPOINTS > 0
              DEBUG_PRINTF("Received %ssuccessful DP update from ID %d\n", ret ? "" : "un", ipcrecv.dpID );
          #endif
      }
    }


#endif /* RP2040_M0_1 || defined(CORE1_SIM) */

#if RP2040_M0_0

static IPC_DP_Xfer_Buff_T ipcsend;

static bool DP_Pack( uint8_t id, Variant_T *v )
{
    /* Ensure, only valid types are transferred */
    if ( v->type == VAR_STYLE || v->type == VAR_FONT || v->type == VAR_REF ) {
        #if DEBUG_DATAPOINTS > 0
            DEBUG_PRINTF("Err: DP_Pack: Illegal Variant type %d\n", v->type);
        #endif
        return false;
    }

    ipcsend.buflen      = sizeof(IPC_DP_Xfer_Buff_T);
    ipcsend.xml_version = XML_PARSER_VERSION_BIN;
    ipcsend.dpID        = id;
    ipcsend.v           = *v;

    /* in case of string copy string data */
    if ( v->type == VAR_STRING ) {
      memcpy_fast(ipcsend.str, v->str.text, v->str.len);
      ipcsend.strlen    = v->str.len;
    }

    return true;
}

bool DP_Update_Core0 ( uint8_t id, Variant_T *v )
{
    if ( DP_Pack(id, v) ) Core0_Send_Datapoint ( &ipcsend, NULL );
    return true;
}
#endif /* RP2040_M0_0 */ 

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



