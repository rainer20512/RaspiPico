
/**
 ******************************************************************************
 * @file    gui_datapoint.h
 * @author  Rainer
 * @brief   Interface for GUI Datapoints
 *
 ******************************************************************************
 *
 ******************************************************************************
 */
#ifndef _GUI_DATAPOINT_H_
#define _GUI_DATAPOINT_H_
#include "config/config.h"

#if USE_GUI_INTERFACE > 0
#include "../../lvgl/lvgl.h"
#include "gui_def.h"
#include "gui_editdef.h"

typedef struct {
  uint8_t             DP_Id;                    /* unique ID of Datapoint */
  char                DP_name[ID_MAXNAMELEN];   /* Null-terminated Name of Datapoint */
  GUI_Edit_Enum       DP_elem_type;             /* Addressed GUI element             */
  uint8_t             DP_propidx;               /* Property index for that element   */
  GUI_E_Datatype_Enum DP_datatype;              /* Datatype of Datapoint             */
} GUI_Datapoint_T;




GUI_Datapoint_T  *DP_New( char *name,  GUI_Edit_Enum elemtype, uint8_t propidx, GUI_E_Datatype_Enum datatype );
int8_t            DP_Add( char *name,  GUI_Datapoint_T *newdp );

#endif /* USE_GUI_INTERFACE > 0 */
#endif /* _GUI_DATAPOINT_H_ */