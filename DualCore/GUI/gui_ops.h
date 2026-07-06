
/**
  ******************************************************************************
  * @file    gui_ops.h
  * @author  Rainer
  * @brief   Operations on GUI elements
  *          NOTE: different implementations on Core0 and Core1
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _GUIOPS_H_
#define _GUIOPS_H_
#include "config/config.h"

#if USE_GUI_INTERFACE > 0
#include "../GUI/gui_def.h"

void GUI_Init_Curr_Elems(void);

#if defined(RP2040_M0_1) || defined(CORE1_SIM)
    extern GUI_Font_T  *AllFonts1;
    extern uint8_t      AllFontNum1;          /* number of defined fonts Core1   */
    void      GUI_Init_Ops_Core1(void);
#endif
#if defined(RP2040_M0_0)
    extern GUI_Font_T  *AllFonts0;
    extern uint8_t      AllFontNum0;          /* number of defined fonts Core0   */
    void      GUI_Init_Ops_Core0(void);
#endif

#endif /* USE_GUI_INTERFACE */
#endif /* _GUIOPS_H_ */