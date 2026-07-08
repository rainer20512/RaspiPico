/**
 * @file GC9A01.h
 *
 **/

#ifndef GC9A01_H
#define GC9A01_H

#include "config/config.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#define LV_DRV_NO_CONF
#ifndef LV_DRV_NO_CONF
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_drv_conf.h"
#else
#include "../../lv_drv_conf.h"
#endif
#endif

#include "../../lvgl/lvgl.h"

/*********************
 *      DEFINES
 *********************/
// #define lv_color_t  uint32_t
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void GC9A01_hard_reset( void );
void GC9A01_run_cfg_script(void);
void GC9A01_cmd_data ( uint8_t cmd, uint8_t data );
void GC9A01_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color);
void GC9A01_setRotation(uint8_t m);
void GC9A01_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void GC9A01_fillScreen(uint16_t color);
uint16_t GC9A01_Color565(uint8_t r, uint8_t g, uint8_t b);
void GC9A01_drawPixel(int16_t x, int16_t y, uint16_t color);
void GC9A01_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void GC9A01_drawFastVLine(int16_t x, int16_t y, int16_t w, uint16_t color);

/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GC9A01_H */