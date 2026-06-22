
#ifndef _GUIDEF_H_
#define _GUIDEF_H_
#include "config/config.h"

#if USE_LVGL > 0
#include "../../lvgl/lvgl.h"

void GUI_test_master  (uint32_t num);

#if GUI_TEST > 0
//
#endif /* GUI_TEST > 0 */
#endif /* USE_LVGL */
#endif /* _GUIDEF_H_ */