#include "config/config.h"
#if USE_GUI_INTERFACE > 0

#include <stdio.h>
#include "../GUI/gui_ops.h"
#include "../GUI/gui_edit.h"
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

  GUI_Font_T *AllFonts1;
  uint8_t     AllFontNum1;           /* number of defined fonts */
#endif
#if defined(RP2040_M0_0)
  GUI_Font_T *AllFonts0;             /* Array of all Fonts Core0      */ 
  uint8_t     AllFontNum0;           /* number of defined fonts Core0 */
  #define ALL_FONTS     AllFonts0
  #define ALL_FONTNUM   AllFontNum0 
#else
  #define ALL_FONTS     AllFonts1
  #define ALL_FONTNUM   AllFontNum1 
#endif

/* Font idx to use in default style */
#define DEFAULT_FONTNUM     5        /* Bahn Semibold Condensed 100 */
/* Font to use, if above font does not exist */
#define FALLBACK_FONTNUM    0        /* Montserrat 14 */

/******************************************************************************
 * @brief  Initialize cur_style, cur_label, cur_act, ...  from def_style, 
 *         def_label, def_arc, ...
 *         Only sense is to have a handy preset for manual editing
 ******************************************************************************/
void GUI_Init_Curr_Elems(void)
{
    /* initialize all current elements with their static default setting */
    cur_style = def_style;
    cur_label = def_label;
    cur_arc   = def_arc;

    /* Now the default setting may be enhanced */

    /* Set the textfont of the default style */
    /* First try to use DEFAULT_FONTNUM, if not exit FALLBACK_FONTNUM */
    uint8_t fontidx = ALL_FONTNUM >= DEFAULT_FONTNUM ? DEFAULT_FONTNUM : FALLBACK_FONTNUM;
    cur_style.textfont = ALL_FONTS[fontidx].font;
    STYLE_SET_PROP(&cur_style, STYLE_TEXTFONT);

}


#if defined(RP2040_M0_1) || defined(CORE1_SIM)

/******************************************************************************
 * @brief Initialize GUI, to be called after all other initialization steps 
 *        Steps to be done
 *        1) Init list of available Fonts, directly on Core1, by IPC on Core 0
 * @note  Core1 implementation
 *****************************************************************************/     
void GUI_Init_Ops_Core1(void)
{
  AllFonts1     = (GUI_Font_T *)DefinedFonts;
  AllFontNum1  = sizeof(DefinedFonts) / sizeof(GUI_Font_T);
  GUI_Init_Fonts_Core1();

  /* Update curent elements wirh font settings */  
  GUI_Init_Curr_Elems();
}
#endif
#if defined(RP2040_M0_0)
  #include "system/ipc_msg.h"

  /******************************************************************************
   * @brief Initialize GUI, to be called after all other initialization steps 
   *        Steps to be done
   *        1) Init list of available Fonts, directly on Core1, by IPC on Core 0
   *        2) store font info in global GUI element list of Core 0
   * @note  Core0 implementation
   *****************************************************************************/     
  void GUI_Init_Ops_Core0(void)
  {
      Core0_Qry_Fontinfo(NULL, GUI_Init_Fonts_Core0);
      /* The call of "GUI_Init_Curr_Elems()" is done asynchronically at the */
      /* end of "GUI_Init_Fonts_Core0" !                                    */
  }
#endif /* RP2040_M0_0 */

#endif /* USE_LVGL > 0 */ 

