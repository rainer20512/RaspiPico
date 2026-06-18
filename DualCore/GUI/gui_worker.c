#include "config/config.h"
#if USE_LVGL > 0

#include <stdio.h>
#include "../GUI/gui_def.h"
#include "../GUI/gui_edit.h"
#include "../GUI/gui_lists.h"

lv_style_t *mystyle;
lv_obj_t   *mylbl, *mylbl2;


/******************************************************************************
 * @brief Load all known fonts _once_ into GUI elem list
 *****************************************************************************/     
void GUI_Load_Fonts(void)
{
  List_Elem_T *font;
  /* Find first dont in global list. I found, fonts are loaded already, 
   * do nothing then */
  if ( LL_find_nth(GUI_item_list, GUI_ELEM_FONT, 1) ) return;

  /* Otherwise iterate thru all defined fonts and insert them into global item list */
  /* List of defined fonts _MUST BE_ terminated by NULL,NULL */
  uint32_t i = 0; 
  while ( AllFonts[i].font ) {
     font = LL_New_Element(GUI_ELEM_FONT,  (void *)AllFonts[i].font, AllFonts[i].fontname, &AllFonts[i]);
     LL_append(&GUI_item_list, font);
     i++;
  }

  printf("%d fonts loaded\n", i);

}

void DBG_heap_useage(void);
void gui_test_master(uint32_t num)
{
    GUI_Load_Fonts();
	switch(num) {
    	case 1:
        	GUI_Edit(&edit_style, &cur_style, NULL);
            break;
    	case 2:
        	GUI_Edit(&edit_label, &cur_label, NULL);
            break;
    	case 3:
        	GUI_Edit(&edit_arc, &cur_arc, NULL);
            break;
    	case 4:
            /*Change the active screen's background color*/
            lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57), LV_PART_MAIN);

            /*Create a white label, set its text and align it to the center*/
            mylbl2 = lv_label_create(lv_screen_active());
            lv_label_set_text(mylbl2, "Hello world");
            lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
            lv_obj_align(mylbl2, LV_ALIGN_CENTER, 0, -60);
            GUI_dump_coords(mylbl2);
            break;
    	case 5:
            GUI_dump_coords(mylbl);
            GUI_dump_coords(mylbl2);
            break;
    	case 99:
            DBG_heap_useage();
            break;
    	default: 
        	puts("Unknown Test");
            return;
    }
}


#endif /* USE_LVGL > 0 */ 

