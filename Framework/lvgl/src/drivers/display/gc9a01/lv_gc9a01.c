/**
 * @file lv_gc9a01.c
 *
 * Low level driver for GCA901 TFT driver chip
 *
 */


/*********************
 *      INCLUDES
 *********************/
#include "lv_gc9a01.h"
#include "gc9a01.h"
#include "lv_4wire_spi.h"
#include "debug/debug_io.h"

#include "system/util.h"

#if LV_USE_GC9A01

/* Do output of graphic data via DMA, Commands are always written directly / blocking */
#define LV_USE_SPI_DMA          0

/*********************
 *      DEFINES
 *********************/

#define MY_DISP_HOR_RES         240
#define MY_DISP_VER_RES         240
#define BYTE_PER_PIXEL          2

/* GC9A01 Commands that we know of.  Limited documentation */
#define GC9A01_INVOFF		0x20
#define GC9A01_INVON		0x21
#define GC9A01_DISPON		0x29
#define GC9A01_CASET		0x2A
#define GC9A01_RASET		0x2B
#define GC9A01_RAMWR		0x2C
#define GC9A01_COLMOD		0x3A
#define GC9A01_MADCTL		0x36
#define GC9A01_MADCTL_MY  	0x80
#define GC9A01_MADCTL_MX  	0x40
#define GC9A01_MADCTL_MV  	0x20
#define GC9A01_MADCTL_RGB 	0x00
#define GC9A01_DISFNCTRL	0xB6

#define GC9A01_CMD_MODE		0
#define GC9A01_DATA_MODE     	1


#define GC9A01_DEBUG            0
#if GC9A01_DEBUG
  #include "debug/debug_helper.h"
#endif



/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *      Forward decls
 **********************/
static void disp_flush(lv_display_t * disp_drv, const lv_area_t * area, uint8_t * px_map);
void gc9a01_send_cmd(lv_display_t * disp, const uint8_t * cmd, size_t cmd_size, 
                       const uint8_t * param, size_t param_size);
void gc9a01_send_mass_data(lv_display_t * disp, const uint8_t * cmd, size_t cmd_size, 
                       uint8_t * param, size_t param_size);


/**********************
 *   Frame buffer
 **********************/
// #define GRMEM __attribute((section(".bss2")))
#define GRMEM 
#define NUMROWS     30          // Number of rows in partial buffer
static GRMEM uint8_t buf_1_1[MY_DISP_HOR_RES * NUMROWS * BYTE_PER_PIXEL];            /*A buffer for NUMROWS rows*/
// static GRMEM uint8_t buf_1_1[MY_DISP_HOR_RES * MY_DISP_VER_RES * BYTE_PER_PIXEL];       /*A full buffer       */
/**********************
 *   GLOBAL FUNCTIONS
 **********************/
lv_display_t * lv_gc9a01_create(uint32_t hor_res, uint32_t ver_res, lv_lcd_flag_t flags )
{
    spi_setup_dma();
    GC9A01_hard_reset();
    GC9A01_run_cfg_script();

    lv_display_t * disp = lv_lcd_generic_mipi_create(hor_res, ver_res, flags, gc9a01_send_cmd, gc9a01_send_mass_data);
    // lv_lcd_generic_mipi_send_cmd_list(disp, init_cmd_list);

     /* One buffer for partial rendering*/
    LV_ATTRIBUTE_MEM_ALIGN
    lv_display_set_buffers(disp, buf_1_1, NULL, sizeof(buf_1_1), LV_DISPLAY_RENDER_MODE_PARTIAL);
    // lv_display_set_buffers(disp, buf_1_1, NULL, sizeof(buf_1_1), LV_DISPLAY_RENDER_MODE_DIRECT);

    /* Test code to see, whether TFT driver Communication is working at all */
    uint16_t temp = (uint16_t)get_ms_since_start();
    GC9A01_fillScreen(GC9A01_Color565(temp, temp>>4, temp>>8)); // ?
    GC9A01_fillRect( 60, 60, 80, 80, GC9A01_Color565(0xFF,0x00,0x00));
    /* End test code */

    return disp;
}

void lv_gc9a01_set_gap(lv_display_t * disp, uint16_t x, uint16_t y)
{
    lv_lcd_generic_mipi_set_gap(disp, x, y);
}

void lv_gc9a01_set_invert(lv_display_t * disp, bool invert)
{
    lv_lcd_generic_mipi_set_invert(disp, invert);
}

void lv_gc9a01_set_gamma_curve(lv_display_t * disp, uint8_t gamma)
{
    lv_lcd_generic_mipi_set_gamma_curve(disp, gamma);
}

void lv_gc9a01_send_cmd_list(lv_display_t * disp, const uint8_t * cmd_list)
{
    lv_lcd_generic_mipi_send_cmd_list(disp, cmd_list);
}


static lv_display_t * mydisp;

/******************************************************************************
* Send is done: Pull nCS up, infrom lvgl about send done
******************************************************************************/
void gc9a01_send_done ( void )
{
    LV_DRV_DISP_SPI_CS(1);

    #if GC9A01_DEBUG > 0
        DEBUG_PRINTTS(" Send done\n");
    #endif

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_display_flush_ready(mydisp);
}


/******************************************************************************
* lowlevel routine for writing n command and m data bytes, n,m >= 0
******************************************************************************/
void gc9a01_send_cmd(lv_display_t * disp, const uint8_t * cmd, size_t cmd_size, 
                       const uint8_t * param, size_t param_size)
{
    const uint8_t *p;
    size_t i;
    LV_UNUSED(disp);

   LV_DRV_DISP_SPI_CS(0);  // Listen to us

   if ( cmd_size > 0 ) {
	LV_DRV_DISP_CMD_DATA(GC9A01_CMD_MODE);
        for ( i = 0, p=cmd; i < cmd_size; p++,i++ ) {
            LV_DRV_DISP_SPI_WR_BYTE(*p);
            #if GC9A01_DEBUG > 0
                DEBUG_PRINTF("C 0x%02x ", *p);
            #endif
        }
	LV_DRV_DISP_CMD_DATA(GC9A01_DATA_MODE);
    }
    if ( param_size > 0 ) {
        #if GC9A01_DEBUG > 0
            DEBUG_PRINTF("%d data bytes starting with 0x%02x", param_size, *param);
        #endif
        for ( i = 0, p=param; i < param_size; p++,i++ ) {
            LV_DRV_DISP_SPI_WR_BYTE(*p);
        }
    }
    mydisp = disp;
    gc9a01_send_done();
}


void gc9a01_send_mass_data(lv_display_t * disp, const uint8_t * cmd, size_t cmd_size, 
                       uint8_t * param, size_t param_size)
{
    LV_UNUSED(disp);
    uint32_t byte_left;
    const uint8_t *pt;
    size_t i;

    mydisp = disp;

    LV_DRV_DISP_SPI_CS(0);  // Listen to us

   if ( cmd_size > 0 ) {
	LV_DRV_DISP_CMD_DATA(GC9A01_CMD_MODE);
        for ( i = 0, pt=cmd; i < cmd_size; pt++,i++ ) {
            LV_DRV_DISP_SPI_WR_BYTE(*pt);
            #if GC9A01_DEBUG > 0
                DEBUG_PRINTF("C 0x%02x ", *pt);
            #endif
        }
	LV_DRV_DISP_CMD_DATA(GC9A01_DATA_MODE);
    }
    
    /* No data? so we are done */
    if (param_size == 0) {
      gc9a01_send_done();
      return;
    }     

    #if GC9A01_DEBUG > 0
        DEBUG_PRINTTS("Blockwise: %d data bytes starting with 0x%02x", param_size, *param);
    #endif
    #if 0
      byte_left = param_size;
      pt = param;
      while (byte_left)
      {
              if (byte_left > 64) {
                      LV_DRV_DISP_SPI_WR_ARRAY(pt, 64);
                      byte_left = byte_left - 64;
                      pt = pt + 64;
              }
              else
              {
                      LV_DRV_DISP_SPI_WR_ARRAY(pt, byte_left);
                      byte_left=0;
              }
      }
    #endif

    LV_DRV_DISP_SPI_WR_ARRAY(param, param_size, gc9a01_send_done);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif  /*LV_USE_GC9A01*/
