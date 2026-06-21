#include "config/config.h"
#if USE_LVGL > 0

#include <stdio.h>
#include "../GUI/gui_def.h"
#include "../GUI/gui_edit.h"
#include "../GUI/gui_lists.h"

lv_style_t *mystyle;
lv_obj_t   *mylbl, *mylbl2;



void DBG_heap_useage(void);
void GUI_Init_Ops_Core0(void);
void GUI_test_master(uint32_t num)
{
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

#if defined(RP2040_M0_0)
    	case 98:
            GUI_Init_Ops_Core0();
            break;
#endif
    	case 99:
            DBG_heap_useage();
    	default: 
        	puts("Unknown Test");
            return;
    }
}


#endif /* USE_LVGL > 0 */ 

