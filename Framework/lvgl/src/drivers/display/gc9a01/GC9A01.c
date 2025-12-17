/**
  * @file GC9A01.c
  *
 **/


/*********************
 *      INCLUDES
 *********************/
#include "GC9A01.h"
#include "./lv_4wire_spi.h"
#if LV_USE_GC9A01


/*********************
 *      DEFINES
 *********************/
#ifndef GC9A01_XSTART
#define GC9A01_XSTART   	0
#endif
#ifndef GC9A01_YSTART
#define GC9A01_YSTART   	0
#endif

#define GC9A01_CMD_MODE		0
#define GC9A01_DATA_MODE     	1

#define GC9A01_HOR_RES  	240
#define GC9A01_VER_RES  	240


/* GC9A01 Commands that we know of.  Limited documentation */
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

/**********************
 *      TYPEDEFS
 **********************/

/* Init script function */
struct GC9A01_function {
	uint16_t cmd;
	uint16_t data;
};

/* Init script commands */
enum GC9A01_cmd {
	GC9A01_START,
	GC9A01_END,
	GC9A01_CMD,
	GC9A01_DATA,
	GC9A01_DELAY
};

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC CONSTANTS
 **********************/
#if 0
/* 
 * init commands for 1.28" round display, old code
 * copied from somewhere in the internet
 */
static const uint8_t init_cmd_list[] = {
    0xEF,       0,
    0xEB,       1,  0x14,
    0xFE,       0,          // Inter Register Enable1
    0xEF,       0,          // Inter Register Enable2
    0xEB,       1,  0x14,
    0x84,       1,  0x40,
    0x85,       1,  0xFF,
    0x86,       1,  0xFF,
    0x87,       1,  0xFF,
    0x88,       1,  0x0A,
    0x89,       1,  0x21,
    0x8A,       1,  0x00,
    0x8B,       1,  0x80,
    0x8C,       1,  0x01,
    0x8D,       1,  0x01,
    0x8E,       1,  0xFF,
    0x8F,       1,  0xFF,
    GC9A01_DISFNCTRL,      // Display Function Control
                2,  0x00, 0x00,
    GC9A01_MADCTL,        // Memory Access Control
                1,  0x48, // Set the display direction 0,1,2,3	four directions
    GC9A01_COLMOD,        // COLMOD: Pixel Format Set
                1,  0x05, // 16 Bits per pixel
    0x90,       4,  0x08, 0x08, 0x08, 0x08,
    0xBD,       1,  0x06,
    0xBC,       1,  0x00,
    0xFF,       3,  0x60, 0x01, 0x04,
    0xC3,               // Power Control 2
                1,  0x13,
    0xC4,               // Power Control 3
                1,  0x13,
    0xC9,               // Power Control 4
                1,  0x22,
    0xBE,       1 , 0x11,
    0xE1,       2,  0x10, 0x0E,
    0xDF,       3,  0x21, 0x0C, 0x02,
    0xF0,               // SET_GAMMA1
                6,  0x45, 0x09, 0x08, 0x08, 0x26, 0x2A,
    0xF1,               // SET_GAMMA2
                6,  0x43, 0x70, 0x72, 0x36, 0x37, 0x6F,
    0xF2,               // SET_GAMMA3
                6,  0x45, 0x09, 0x08, 0x08, 0x26, 0x2A,
    0xF3,               // SET_GAMMA4
                6,  0x43, 0x70, 0x72, 0x36, 0x37, 0x6F,
    0xED,       2,  0x1B, 0x0B,
    0xAE,       1,  0x77,
    0xCD,       1,  0x63,
    0x70,       9,  0x07, 0x07, 0x04, 0x0E, 0x0F, 0x09, 0x07, 0x08, 0x03,
    0xE8,       1,  0x34,
    0x62,       12, 0x18, 0x0D, 0x71, 0xED, 0x70, 0x70,
                    0x18, 0x0F, 0x71, 0xEF, 0x70, 0x70,
    0x63,       12, 0x18, 0x11, 0x71, 0xF1, 0x70, 0x70,
                    0x18, 0x13, 0x71, 0xF3, 0x70, 0x70,
    0x64,       7,  0x28, 0x29, 0xF1, 0x01, 0xF1, 0x00, 0x07,
    0x66,       10, 0x3C, 0x00, 0xCD, 0x67, 0x45, 0x45,
                    0x10, 0x00, 0x00, 0x00,
    0x67,       10, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x01,
                    0x54, 0x10, 0x32, 0x98,
    0x74,       7,  0x10, 0x85, 0x80, 0x00, 0x00, 0x4E, 0x00,
    0x98,       2,  0x3E, 0x07,
    0x35,       0,      // Tearing Effect Line ON
    0x21,       0,      // Display Inversion ON
    0x11,       0,      // Sleep Out Mode
    LV_LCD_CMD_DELAY_MS, 12,        // Delay in multiples of 10(!) ms
    GC9A01_DISPON, 0,   // Display ON
    LV_LCD_CMD_DELAY_MS, 26,
    LV_LCD_CMD_DELAY_MS, LV_LCD_CMD_EOF
};

#endif
/**********************
 *  STATIC VARIABLES
 **********************/
// Documentation on op codes for GC9A01 are very hard to find.
// Will document should they be found.
static struct GC9A01_function GC9A01_cfg_script[] = {
	{ GC9A01_START, GC9A01_START},
	{ GC9A01_CMD, 0xEF},

	{ GC9A01_CMD, 0xEB},
	{ GC9A01_DATA, 0x14},

	{ GC9A01_CMD, 0xFE}, // Inter Register Enable1
	{ GC9A01_CMD, 0xEF}, // Inter Register Enable2

	{ GC9A01_CMD, 0xEB},
	{ GC9A01_DATA, 0x14},

	{ GC9A01_CMD, 0x84},
	{ GC9A01_DATA, 0x40},

	{ GC9A01_CMD, 0x85},
	{ GC9A01_DATA, 0xFF},

	{ GC9A01_CMD, 0x86},
	{ GC9A01_DATA, 0xFF},

	{ GC9A01_CMD, 0x87},
	{ GC9A01_DATA, 0xFF},

	{ GC9A01_CMD, 0x88},
	{ GC9A01_DATA, 0x0A},

	{ GC9A01_CMD, 0x89},
	{ GC9A01_DATA, 0x21},

	{ GC9A01_CMD, 0x8A},
	{ GC9A01_DATA, 0x00},

	{ GC9A01_CMD, 0x8B},
	{ GC9A01_DATA, 0x80},

	{ GC9A01_CMD, 0x8C},
	{ GC9A01_DATA, 0x01},

	{ GC9A01_CMD, 0x8D},
	{ GC9A01_DATA, 0x01},

	{ GC9A01_CMD, 0x8E},
	{ GC9A01_DATA, 0xFF},

	{ GC9A01_CMD, 0x8F},
	{ GC9A01_DATA, 0xFF},

	{ GC9A01_CMD, GC9A01_DISFNCTRL}, // Display Function Control
	{ GC9A01_DATA, 0x00},
	{ GC9A01_DATA, 0x00},

	{ GC9A01_CMD, GC9A01_MADCTL}, // Memory Access Control
	{ GC9A01_DATA, 0x48}, // Set the display direction 0,1,2,3	four directions

	{ GC9A01_CMD, GC9A01_COLMOD}, // COLMOD: Pixel Format Set
	{ GC9A01_DATA, 0x05}, // 16 Bits per pixel

	{ GC9A01_CMD, 0x90},
	{ GC9A01_DATA, 0x08},
	{ GC9A01_DATA, 0x08},
	{ GC9A01_DATA, 0x08},
	{ GC9A01_DATA, 0x08},

	{ GC9A01_CMD, 0xBD},
	{ GC9A01_DATA, 0x06},

	{ GC9A01_CMD, 0xBC},
	{ GC9A01_DATA, 0x00},

	{ GC9A01_CMD, 0xFF},
	{ GC9A01_DATA, 0x60},
	{ GC9A01_DATA, 0x01},
	{ GC9A01_DATA, 0x04},

	{ GC9A01_CMD, 0xC3}, // Power Control 2
	{ GC9A01_DATA, 0x13},
	{ GC9A01_CMD, 0xC4}, // Power Control 3
	{ GC9A01_DATA, 0x13},

	{ GC9A01_CMD, 0xC9}, // Power Control 4
	{ GC9A01_DATA, 0x22},

	{ GC9A01_CMD, 0xBE},
	{ GC9A01_DATA, 0x11},

	{ GC9A01_CMD, 0xE1},
	{ GC9A01_DATA, 0x10},
	{ GC9A01_DATA, 0x0E},

	{ GC9A01_CMD, 0xDF},
	{ GC9A01_DATA, 0x21},
	{ GC9A01_DATA, 0x0C},
	{ GC9A01_DATA, 0x02},

	{ GC9A01_CMD, 0xF0}, // SET_GAMMA1
	{ GC9A01_DATA, 0x45},
	{ GC9A01_DATA, 0x09},
	{ GC9A01_DATA, 0x08},
	{ GC9A01_DATA, 0x08},
	{ GC9A01_DATA, 0x26},
	{ GC9A01_DATA, 0x2A},

	{ GC9A01_CMD, 0xF1}, // SET_GAMMA2
	{ GC9A01_DATA, 0x43},
	{ GC9A01_DATA, 0x70},
	{ GC9A01_DATA, 0x72},
	{ GC9A01_DATA, 0x36},
	{ GC9A01_DATA, 0x37},
	{ GC9A01_DATA, 0x6F},

	{ GC9A01_CMD, 0xF2}, // SET_GAMMA3
	{ GC9A01_DATA, 0x45},
	{ GC9A01_DATA, 0x09},
	{ GC9A01_DATA, 0x08},
	{ GC9A01_DATA, 0x08},
	{ GC9A01_DATA, 0x26},
	{ GC9A01_DATA, 0x2A},

	{ GC9A01_CMD, 0xF3}, // SET_GAMMA4
	{ GC9A01_DATA, 0x43},
	{ GC9A01_DATA, 0x70},
	{ GC9A01_DATA, 0x72},
	{ GC9A01_DATA, 0x36},
	{ GC9A01_DATA, 0x37},
	{ GC9A01_DATA, 0x6F},

	{ GC9A01_CMD, 0xED},
	{ GC9A01_DATA, 0x1B},
	{ GC9A01_DATA, 0x0B},

	{ GC9A01_CMD, 0xAE},
	{ GC9A01_DATA, 0x77},

	{ GC9A01_CMD, 0xCD},
	{ GC9A01_DATA, 0x63},

	{ GC9A01_CMD, 0x70},
	{ GC9A01_DATA, 0x07},
	{ GC9A01_DATA, 0x07},
	{ GC9A01_DATA, 0x04},
	{ GC9A01_DATA, 0x0E},
	{ GC9A01_DATA, 0x0F},
	{ GC9A01_DATA, 0x09},
	{ GC9A01_DATA, 0x07},
	{ GC9A01_DATA, 0x08},
	{ GC9A01_DATA, 0x03},

	{ GC9A01_CMD, 0xE8},
	{ GC9A01_DATA, 0x34},

	{ GC9A01_CMD, 0x62},
	{ GC9A01_DATA, 0x18},
	{ GC9A01_DATA, 0x0D},
	{ GC9A01_DATA, 0x71},
	{ GC9A01_DATA, 0xED},
	{ GC9A01_DATA, 0x70},
	{ GC9A01_DATA, 0x70},
	{ GC9A01_DATA, 0x18},
	{ GC9A01_DATA, 0x0F},
	{ GC9A01_DATA, 0x71},
	{ GC9A01_DATA, 0xEF},
	{ GC9A01_DATA, 0x70},
	{ GC9A01_DATA, 0x70},

	{ GC9A01_CMD, 0x63},
	{ GC9A01_DATA, 0x18},
	{ GC9A01_DATA, 0x11},
	{ GC9A01_DATA, 0x71},
	{ GC9A01_DATA, 0xF1},
	{ GC9A01_DATA, 0x70},
	{ GC9A01_DATA, 0x70},
	{ GC9A01_DATA, 0x18},
	{ GC9A01_DATA, 0x13},
	{ GC9A01_DATA, 0x71},
	{ GC9A01_DATA, 0xF3},
	{ GC9A01_DATA, 0x70},
	{ GC9A01_DATA, 0x70},

	{ GC9A01_CMD, 0x64},
	{ GC9A01_DATA, 0x28},
	{ GC9A01_DATA, 0x29},
	{ GC9A01_DATA, 0xF1},
	{ GC9A01_DATA, 0x01},
	{ GC9A01_DATA, 0xF1},
	{ GC9A01_DATA, 0x00},
	{ GC9A01_DATA, 0x07},

	{ GC9A01_CMD, 0x66},
	{ GC9A01_DATA, 0x3C},
	{ GC9A01_DATA, 0x00},
	{ GC9A01_DATA, 0xCD},
	{ GC9A01_DATA, 0x67},
	{ GC9A01_DATA, 0x45},
	{ GC9A01_DATA, 0x45},
	{ GC9A01_DATA, 0x10},
	{ GC9A01_DATA, 0x00},
	{ GC9A01_DATA, 0x00},
	{ GC9A01_DATA, 0x00},

	{ GC9A01_CMD, 0x67},
	{ GC9A01_DATA, 0x00},
	{ GC9A01_DATA, 0x3C},
	{ GC9A01_DATA, 0x00},
	{ GC9A01_DATA, 0x00},
	{ GC9A01_DATA, 0x00},
	{ GC9A01_DATA, 0x01},
	{ GC9A01_DATA, 0x54},
	{ GC9A01_DATA, 0x10},
	{ GC9A01_DATA, 0x32},
	{ GC9A01_DATA, 0x98},

	{ GC9A01_CMD, 0x74},
	{ GC9A01_DATA, 0x10},
	{ GC9A01_DATA, 0x85},
	{ GC9A01_DATA, 0x80},
	{ GC9A01_DATA, 0x00},
	{ GC9A01_DATA, 0x00},
	{ GC9A01_DATA, 0x4E},
	{ GC9A01_DATA, 0x00},

	{ GC9A01_CMD, 0x98},
	{ GC9A01_DATA, 0x3E},
	{ GC9A01_DATA, 0x07},

	{ GC9A01_CMD, 0x35}, // Tearing Effect Line ON
	{ GC9A01_CMD, 0x21}, // Display Inversion ON

	{ GC9A01_CMD, 0x11}, // Sleep Out Mode
	{ GC9A01_DELAY, 120},
	{ GC9A01_CMD, GC9A01_DISPON}, // Display ON
	{ GC9A01_DELAY, 255},
	{ GC9A01_END, GC9A01_END},
};


/**********************
 *  STATIC PROTOTYPES
 **********************/
static void GC9A01_set_addr_win(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Write a command to the GC9A01
 * @param cmd the command
 */
static void GC9A01_command(uint8_t cmd)
{
	LV_DRV_DISP_CMD_DATA(GC9A01_CMD_MODE);
	LV_DRV_DISP_SPI_WR_BYTE(cmd);
}

/**
 * Write data to the GC9A01
 * @param data the data
 */
static void GC9A01_data(uint8_t data)
{
	LV_DRV_DISP_CMD_DATA(GC9A01_DATA_MODE);
	LV_DRV_DISP_SPI_WR_BYTE(data);
}

//***********************************************************
// hard reset of the tft controller
// ----------------------------------------------------------
void GC9A01_hard_reset( void )
{
    LV_DRV_DISP_RST(1);
    LV_DRV_DELAY_MS(50);
    LV_DRV_DISP_RST(0);
    LV_DRV_DELAY_MS(50);
    LV_DRV_DISP_RST(1);
    LV_DRV_DELAY_MS(50);
}

// Configuration of the tft controller
// ----------------------------------------------------------
void GC9A01_run_cfg_script(void)
{
	int i = 0;
	int end_script = 0;

        LV_DRV_DISP_SPI_CS(0); // Low to listen to us

	do {
		switch (GC9A01_cfg_script[i].cmd)
		{
			case GC9A01_START:
				break;
			case GC9A01_CMD:
				GC9A01_command( GC9A01_cfg_script[i].data & 0xFF );
				break;
			case GC9A01_DATA:
				GC9A01_data( GC9A01_cfg_script[i].data & 0xFF );
				break;
			case GC9A01_DELAY:
                                LV_DRV_DELAY_MS(GC9A01_cfg_script[i].data);
				break;
			case GC9A01_END:
				end_script = 1;
		}
		i++;
	} while (!end_script);

        LV_DRV_DISP_SPI_CS(1); // Deselect
}


void GC9A01_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  // Rudimentary clipping
  if((x >= GC9A01_HOR_RES) || (y >= GC9A01_VER_RES)) return;
  if((y+h-1) >= GC9A01_VER_RES) h = GC9A01_VER_RES - y;

    LV_DRV_DISP_SPI_CS(0); // Listen to us

    GC9A01_set_addr_win(x, y, x, y + h - 1);

    uint8_t hi = color >> 8, lo = color;

    while (h--) {
        GC9A01_data(hi);
        GC9A01_data(lo);
    }

    LV_DRV_DISP_SPI_CS(1);
}

void GC9A01_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    // Rudimentary clipping
    if((x >= GC9A01_HOR_RES) || (y >= GC9A01_VER_RES)) return;
    if((x+w-1) >= GC9A01_HOR_RES)  w = GC9A01_HOR_RES - x;

    LV_DRV_DISP_SPI_CS(0); // Listen to us

    GC9A01_set_addr_win(x, y, x + w - 1, y);

    uint8_t hi = color >> 8, lo = color;

    while (w--) {
        GC9A01_data(hi);
        GC9A01_data(lo);
    }

    LV_DRV_DISP_SPI_CS(1);
}


// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t GC9A01_Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void GC9A01_drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if((x < 0) ||(x >= GC9A01_HOR_RES) || (y < 0) || (y >= GC9A01_VER_RES)) return;

	LV_DRV_DISP_SPI_CS(0); // Listen to us
  GC9A01_set_addr_win(x, y, x, y);

  uint8_t hi = color >> 8, lo = color;

	GC9A01_data(hi);
	GC9A01_data(lo);

	LV_DRV_DISP_SPI_CS(1);
}

void GC9A01_fillScreen(uint16_t color) {
    GC9A01_fillRect(0, 0,  GC9A01_HOR_RES, GC9A01_VER_RES, color);
}

// fill a rectangle
void GC9A01_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= GC9A01_HOR_RES) || (y >= GC9A01_VER_RES)) return;
  if((x + w - 1) >= GC9A01_HOR_RES)  w = GC9A01_HOR_RES  - x;
  if((y + h - 1) >= GC9A01_VER_RES) h = GC9A01_VER_RES - y;

  LV_DRV_DISP_SPI_CS(0);  // Listen to us

  GC9A01_set_addr_win(x, y, x + w - 1, y + h - 1);

	uint8_t hi = color >> 8, lo = color;

  for (y = h; y > 0; y--)
  {
    for (x = w; x > 0; x--)
    {
      GC9A01_data(hi);
      GC9A01_data(lo);
    }
  }
  LV_DRV_DISP_SPI_CS(1);
}

void GC9A01_setRotation(uint8_t m) {

  GC9A01_command(GC9A01_MADCTL);
  m %= 4; // can't be higher than 3
  switch (m) {
   case 0:
     GC9A01_data(GC9A01_MADCTL_MX | GC9A01_MADCTL_MY | GC9A01_MADCTL_RGB);

    //  _xstart = _colstart;
    //  _ystart = _rowstart;
     break;
   case 1:
     GC9A01_data(GC9A01_MADCTL_MY | GC9A01_MADCTL_MV | GC9A01_MADCTL_RGB);

    //  _ystart = _colstart;
    //  _xstart = _rowstart;
     break;
  case 2:
     GC9A01_data(GC9A01_MADCTL_RGB);

    //  _xstart = _colstart;
    //  _ystart = _rowstart;
     break;

   case 3:
     GC9A01_data(GC9A01_MADCTL_MX | GC9A01_MADCTL_MV | GC9A01_MADCTL_RGB);

    //  _ystart = _colstart;
    //  _xstart = _rowstart;
     break;
  }
}

/* only for DEBUG_PRINTF Below */
#include "debug/debug_helper.h"

static void  GC9A01_set_addr_win(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  uint16_t x_start = x0 + GC9A01_XSTART, x_end = x1 + GC9A01_XSTART;
  uint16_t y_start = y0 + GC9A01_YSTART, y_end = y1 + GC9A01_YSTART;

  GC9A01_command(GC9A01_CASET); // Column addr set
  GC9A01_data(x_start >> 8);
  GC9A01_data(x_start & 0xFF);     // XSTART
  GC9A01_data(x_end >> 8);
  GC9A01_data(x_end & 0xFF);       // XEND

  GC9A01_command(GC9A01_RASET); // Row addr set
  GC9A01_data(y_start >> 8);
  GC9A01_data(y_start & 0xFF);     // YSTART
  GC9A01_data(y_end >> 8);
  GC9A01_data(y_end & 0xFF);     // YEND
    
  DEBUG_PRINTF("AddrWin=(%d,%d) (%d,%d)\n", x_start, y_start, x_end, y_end);

  GC9A01_command(GC9A01_RAMWR);
}

#endif
