/**
 ******************************************************************************
 * @file    gui_datapoint.c
 * @author  Rainer
 * @brief   Implementation of GUI Datapoints
 *
 ******************************************************************************
 *
 ******************************************************************************
 */
#include "config/config.h"
#if USE_GUI_INTERFACE > 0
#include "debug/debug_helper.h"
	
#include <string.h>

#include "parser_specs.h"
#include "../GUI/gui_def.h"
#include "../GUI/gui_editdef.h"
#include "../GUI/gui_lists.h"
#include "../GUI/gui_datapoint.h"

#define MAX_DPNUM     10                 /* maximum number of datapoints we can handle */

DP_Datapoint_T Datapoints[MAX_DPNUM];   /* Array of datapoints                  */
static uint8_t  actdp = 0;               /* number of used array elements        */
static uint8_t  dpidcnt = 0;             /* monotonic counter to generate DP-IDs */

static DP_Datapoint_T act;              /* working copy for datapoints, used by varous functions */

/******************************************************************************
 * @brief  Create a new Datapoint and store its data in modul variable "act"
 * @param  all neccessary data to create datapoint
 * @retval ptr to new initiated variable "act"
 ******************************************************************************/
DP_Datapoint_T *DP_New( char *name,  GUI_Edit_Enum elemtype, uint8_t propidx, XML_Datatype_Enum datatype ) 
{
    /* store datapoint in modul variable "act" */
    strncpy(act.DP_name, name, ID_MAXNAMELEN);
    act.DP_Id        = dpidcnt++;
    act.DP_elem_type = elemtype;
    act.DP_propidx   = propidx;
    act.DP_datatype  = datatype;

    return &act;
}

/******************************************************************************
 * @brief  Add a new Datapoint to datapoint list
 * @param newdp - new datapoint to add
 * @retval ID of new datapoint or -1 in case of error
 ******************************************************************************/
int8_t DP_Add( char *name,  DP_Datapoint_T *newdp ) 
{
    /* first, check for free space in Datapoints list */
    if ( actdp >= MAX_DPNUM ) { 
        #if DEBUG_DATAPOINTS > 0
            DEBUG_PRINTF("Err: No space left for Datapoint %s, not added\n",name);
        #endif
        return -1;
    }

    /* copy datapoint to array */
    DP_Datapoint_T *ptr = &Datapoints[actdp++];

    memcpy_fast(ptr, newdp, sizeof(DP_Datapoint_T));

    /* return ID of new datapoint list element */
    return ptr->DP_Id;
}

void DP_Dump_one ( DP_Datapoint_T *dp )
{
    const GUI_Edit_T     *edit  = Find_EditInfoByType( dp->DP_elem_type );
    const Edit_Receipe_T *rec   = FindReceipeByPropIdx( edit, dp->DP_propidx);
    DEBUG_PRINTF("%3d %16s %8s %16s %2d",dp->DP_Id, dp->DP_name, Editinfo[dp->DP_elem_type].name, rec->elem_name, dp->DP_datatype);
}

void DP_Dump_list ( void )
{
  DEBUG_PUTS("List of Datapoints");
  for ( int i=0; i < actdp; i++ ) {
    DP_Dump_one( &Datapoints[i] );
  }
}

#endif /* USE_GUI_INTERFACE > 0 */ 

