#include "../GUI/gui_def.h"
#if USE_LVGL > 0
	
#define GUI_TEST 	1
static lv_style_t defstyle;

lv_style_t * gui_set_style ( GUI_Style_T *act )
{
	/* Check, whether the Object has been allocated already */
	if ( !act->style ) { 
    	lv_style_init(&defstyle);
		act->style = &defstyle;
    }  

	/* assign _All_ style properties */
    lv_style_set_bg_color		(&defstyle, act->bgcolor);
    lv_style_set_bg_opa			(&defstyle, act->bgopa);
    lv_style_set_text_color		(&defstyle, act->txtcolor);
	lv_style_set_width			(&defstyle, act->def_width);
	lv_style_set_height			(&defstyle, act->def_height);
    lv_style_set_border_color	(&defstyle, act->bordercolor);
    lv_style_set_border_width	(&defstyle, act->borderwidth);
    lv_style_set_radius			(&defstyle, act->borderradius);
    lv_style_set_shadow_color	(&defstyle, act->shadowcolor);
    lv_style_set_shadow_offset_x(&defstyle, act->sh_x);
    lv_style_set_shadow_offset_y(&defstyle, act->sh_y);
    lv_style_set_shadow_width	(&defstyle, act->shadow_width);
    lv_style_set_shadow_opa		(&defstyle, act->shadow_opa);
    lv_style_set_text_align	    (&defstyle, act->align);
    	
	return &defstyle;	
}

lv_obj_t *gui_set_label( GUI_Label_T *lbl )
{
	lv_style_value_t ret;
	/* Check, whether lbl has been allocated at all */
	if( ! lbl->obj ) {
	    lbl->obj = lv_label_create(lv_screen_active());
    }
    /* If a style is assigned, activate it */
	if ( lbl->style ) lv_obj_add_style(lbl->obj, lbl->style, 0); 	

    lv_label_set_text(lbl->obj, lbl->txt);

    /* if we have a label assigned alignment, then use it instead of styles alignment */
    if ( lbl->align != LV_ALIGN_DEFAULT ) {
	    lv_obj_align(lbl->obj, lbl->align, lbl->x0, lbl->y0);
    } else {
    	lv_obj_set_pos(lbl->obj, lbl->x0, lbl->y0);
    }

    return lbl->obj;
}
#if GUI_TEST > 0

#include <stdio.h>

lv_style_t *mystyle;
lv_obj_t   *mylbl, *mylbl2;

char lbltext[]  = "Ein Label";
char lbltext2[] = "Label zwei";

GUI_Style_T st = {0};
GUI_Label_T lbl = {0};
GUI_Label_T lbl2 = {0};

void gui_test1(void)
{
	st.def_width	= 120;
	st.def_height	= 20;
  	st.align		= LV_ALIGN_TOP_MID;
	st.bgcolor 		= lv_color_hex(0x404040);  
    st.bgopa   	 	= 128;
  	st.txtcolor		= lv_color_hex(0xff0000);
  	st.bordercolor	= lv_color_hex(0x000000);
 	st.borderwidth	= 0;
  	st.borderradius = 5;
  	st.shadowcolor  = lv_color_hex(0x808080);
    st.shadow_opa   = 128;
    st.shadow_width = 8;
    st.sh_x         = 4;
    st.sh_y         = 4;

    mystyle = gui_set_style(&st);

    lbl.style 	= mystyle;
    lbl.align   = LV_ALIGN_CENTER;
  	lbl.txt	= lbltext;

	mylbl = gui_set_label(&lbl);

}

void gui_test2(void)
{

  	lbl2.align	= LV_ALIGN_CENTER;
  	lbl2.txt	= lbltext2;
	lbl2.y0     = 60;
	mylbl2 = gui_set_label(&lbl2);
}

void gui_test_master(uint32_t num)
{
	printf("Gui Test %d starting\n", num);
	switch(num) {
    	case 1:
        	gui_test1();
            break;
    	case 2:
        	gui_test2();
            break;
    	default: 
        	puts("Unknown Test");
            return;
    }
	printf("Gui Test %d finished\n", num);
}

#endif /* GUI_TEST */

#endif /* USE_LVGL > 0 */ 

