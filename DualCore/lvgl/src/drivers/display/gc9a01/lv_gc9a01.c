/**
 * @file lv_gc9a01.c
 *
 * Low level driver for GCA901 TFT driver chip
 *
 */


/*********************
 *      INCLUDES
 *********************/
#include "string.h"
#include "lv_gc9a01.h"
#include "dev/gc9a01.h"
#include "dev/lv_4wire_spi.h"
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

#define GC9A01_CMD_MODE         0
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
    uint8_t retbuf[6];
    spi_setup_dma();
    spi_enable_dma(true);
    GC9A01_hard_reset();
    GC9A01_run_cfg_script();
    /* Commented out, takes too much time in boot phase */
    // GC9A01_fillScreen(GC9A01_Color565(0x00,0x00,0xFF)); 
     


    lv_display_t * disp = lv_lcd_generic_mipi_create(hor_res, ver_res, flags, gc9a01_send_cmd, gc9a01_send_mass_data);
    // lv_lcd_generic_mipi_send_cmd_list(disp, init_cmd_list);

     /* One buffer for partial rendering*/
    LV_ATTRIBUTE_MEM_ALIGN
    lv_display_set_buffers(disp, buf_1_1, NULL, sizeof(buf_1_1), LV_DISPLAY_RENDER_MODE_PARTIAL);
    // lv_display_set_buffers(disp, buf_1_1, NULL, sizeof(buf_1_1), LV_DISPLAY_RENDER_MODE_DIRECT);

    /* Test code to see, whether TFT driver Communication is working at all */
    GC9A01_fillRect( 60, 60, 80, 80, GC9A01_Color565(0xFF,0x00,0x00));
    /* End test code */
    
    #if GC9A01_DEBUG > 0
        DEBUG_PRINTF("SPI DMA is %senabled...\n", spi_is_dma_enabled() ? "" : "not ");
    #endif 

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

#if GC9A01_DEBUG > 0
  static void dump_vector ( const uint8_t *data, size_t param_size )
  {
    DEBUG_PRINTF("%d data bytes: ", param_size);
    size_t limit=MIN(param_size, 4 );
    for ( size_t i=0; i< limit; i++ ) {
       DEBUG_PRINTF("%02x ", *(data++));
    }
    if ( param_size > limit ) DEBUG_PRINTF("...");
  }
#endif

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
          dump_vector( param, param_size );
        #endif
        for ( i = 0, p=param; i < param_size; p++,i++ ) {
            LV_DRV_DISP_SPI_WR_BYTE(*p);
        }
    }
    #if 0
    mydisp = disp;
    gc9a01_send_done();
    #endif

   LV_DRV_DISP_SPI_CS(1);  
}

/* ----------------------------------------------------------------------------
 * Handle a data block of max size "SWAPWORDSIZE*2" bytes:
 * - copy to temp buffer ( to leave original unchanged )
 * - Send to low level display driver display
 * - notify via callback when done
 * - can be done blocking or no blocking ( when using DMA fro transfer )
 *---------------------------------------------------------------------------*/
static void gc9a01_handle_block  (const uint8_t *block, size_t block_bytesize, bool trigger_done, pfn_spi_done_cb done_cb)
{
  uint32_t i;
  uint16_t *p;

  assert( (block_bytesize&1) == 0 );

  /* Send via SPI interface */
  LV_DRV_DISP_SPI_WR_WORD_ARRAY((uint16_t *)block, block_bytesize / 2, gc9a01_send_done);
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
        dump_vector( param, param_size);
    #endif
    
    /* Make sure, the number of bytes is even, ie integer number of 16bit words */
    assert( (param_size&1) == 0 );

  /* Send via SPI interface */
  LV_DRV_DISP_SPI_WR_WORD_ARRAY((uint16_t *)param, param_size / 2, gc9a01_send_done);

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif  /*LV_USE_GC9A01*/
