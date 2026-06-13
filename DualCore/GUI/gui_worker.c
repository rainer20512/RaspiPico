#include "config/config.h"
#if USE_LVGL > 0

#include <stdio.h>
#include "../GUI/gui_def.h"
#include "../GUI/gui_edit.h"

lv_style_t *mystyle;
lv_obj_t   *mylbl, *mylbl2;

void AssignStyle ( void )
{
  mystyle = gui_new_style ( &cur_style, NULL );
}

void AssignLabel ( void )
{
  mylbl = gui_new_label ( &cur_label, mystyle, NULL );
  GUI_dump_coords(mylbl);
}


void gui_test_master(uint32_t num)
{
	printf("Gui Test %d starting\n", num);
	switch(num) {
    	case 1:
        	GUI_Edit(&edit_style, &cur_style, AssignStyle);
            break;
    	case 2:
        	GUI_Edit(&edit_label, &cur_label, AssignLabel);
            break;
    	case 3:
            /*Change the active screen's background color*/
            lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57), LV_PART_MAIN);

            /*Create a white label, set its text and align it to the center*/
            mylbl2 = lv_label_create(lv_screen_active());
            lv_label_set_text(mylbl2, "Hello world");
            lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
            lv_obj_align(mylbl2, LV_ALIGN_CENTER, 0, -60);
            GUI_dump_coords(mylbl2);
            break;
    	case 4:
            GUI_dump_coords(mylbl);
            GUI_dump_coords(mylbl2);
            break;
    	default: 
        	puts("Unknown Test");
            return;
    }
	printf("Gui Test %d finished\n", num);
}


#endif /* USE_LVGL > 0 */ 

