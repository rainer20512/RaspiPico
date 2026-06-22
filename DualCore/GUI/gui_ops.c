#include "config/config.h"
#if USE_LVGL > 0

#include <stdio.h>
#include "../GUI/gui_ops.h"
#include "../GUI/gui_lists.h"


#if defined(RP2040_M0_1) || defined(CORE1_SIM)
  static const GUI_Font_T DefinedFonts[] =
  {
    {"Montserrat",   14, &lv_font_montserrat_14 },

    // {"Adlam",      24, &rb_font_adlam_24 },
    // {"Adlam",      40, &rb_font_adlam_40 },
  /*
    {"Adlam",        60, &rb_font_adlam_60 },
    {"Adlam",        80, &rb_font_adlam_80 },
    {"Adlam",      100, &rb_font_adlam_100 },
    {"Adlam",      120, &rb_font_adlam_120 },
  */
    // {"Stencil",     24, &w_font_stencil_24 },
    // {"Stencil",     40, &w_font_stencil_40 },
   /*  {"Stencil",     60, &w_font_stencil_60 },
    {"Stencil",     80, &w_font_stencil_80 },
    {"Stencil",    100, &w_font_stencil_100 },
    {"Stencil",    120, &w_font_stencil_120 },
  */
    {"BahnSbC",      24, &w_font_bahn_sbc_24 },
    {"BahnSbC",      40, &w_font_bahn_sbc_40},
    {"BahnSbC",      60, &w_font_bahn_sbc_60 },
    {"BahnSbC",      80, &w_font_bahn_sbc_80 },
    {"BahnSbC",     100, &w_font_bahn_sbc_100 },
    {"BahnSbC",     120, &w_font_bahn_sbc_120},

  };

  GUI_Font_T *AllFonts1 =(GUI_Font_T *) DefinedFonts;
  uint8_t     AllFontNum1;           /* number of defined fonts */
#endif
#if defined(RP2040_M0_0)
  GUI_Font_T *AllFonts0;             /* Array of all Fonts Core0      */ 
  uint8_t     AllFontNum0;           /* number of defined fonts Core0 */
#endif

#if defined(RP2040_M0_1) || defined(CORE1_SIM)

/******************************************************************************
 * @brief Initialize GUI, to be called after all other initialization steps 
 *        Steps to be done
 *        1) Init list of available Fonts, directly on Core1, by IPC on Core 0
 * @note  Core1 implementation
 *****************************************************************************/     
void GUI_Init_Ops_Core1(void)
{
  AllFontNum1  = sizeof(DefinedFonts) / sizeof(GUI_Font_T);
  GUI_Init_Fonts_Core1();
}
#endif
#if defined(RP2040_M0_0)
  #include "system/ipc_msg.h"

  /******************************************************************************
   * @brief Initialize GUI, to be called after all other initialization steps 
   *        Steps to be done
   *        1) Init list of available Fonts, directly on Core1, by IPC on Core 0
   * @note  Core1 implementation
   *****************************************************************************/     
  void GUI_Init_Ops_Core0(void)
  {
      Core0_Qry_Fontinfo(NULL, NULL);
  }
#endif /* RP2040_M0_0 */

#endif /* USE_LVGL > 0 */ 

