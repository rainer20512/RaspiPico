/**
  ******************************************************************************
  * @file    interpreters.c
  * @author  Rainer 
  * @brief   Commmand line interpreters and execution function
  ******************************************************************************
  *
  ******************************************************************************
  * @addtogroup CMDLINE
  * @{
  */


/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <inttypes.h>

#include "config/config.h"
#include "interpreters.h"
#include "debug/debug_helper.h"
#include "system/profiling.h"
#include "system/status.h"
#include "system/rtc.h"
#include "system/util.h"
#include "task/minitask.h"
#include "cmdline.h"
#include "version.h"

#include "hardware/watchdog.h"
#include "boards/pico.h"  

/** @addtogroup CmdLine
  * @{
  */ 


/* Private macro -------------------------------------------------------------*/

#define VOID(a)  (const void *)(a)

#define ADD_SUBMODULE(name) \
const InterpreterModuleT mdl##name =  {pmt##name, cmd##name, sizeof(cmd##name) / sizeof(CommandSetT) }


/* Private types---- ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/*
 * A "stack" of interpreter modules, only the first level is initialized to a
 * basic commandline interpreter
 */

/* Private functions ---------------------------------------------------------*/
// RHB todo
#if DEBUG_FEATURES > 0


#include "debug/debug_gpio_exti.h"
// #include "debug_pwr_rcc.h"


/*********************************************************************************
  * @brief  Submenu to dump system config
  *         
  * @retval true on success, false otherwise
  *
  * @note   will try to read as many parameters as needed
  ********************************************************************************/
static bool Config_Menu ( char *cmdline, size_t len, const void * arg )
{
  char *word;
  size_t wordlen;
  uint32_t ret;

  switch( (uint32_t)arg )  {
    case 0:
      // RHB todo DBG_dump_clocksetting();
      break;
    case 1:
      // RHB todo DBG_dump_rtcclockconfig();
      break;
    default:  
      DEBUG_PUTS("Config_Menu: command not implemented");
  } /* end switch */

  return true;
} /* Config_menu */

static const char *pmtClkCfg (void)
{
  return "Clock";
}

static const CommandSetT cmdClkCfg[] = {
  { "Clock",     ctype_fn, {Config_Menu},  VOID(0), "Show clocksettings" },
  { "RTCClock",  ctype_fn, {Config_Menu},  VOID(1), "Show RTC clocksettings" },
};
ADD_SUBMODULE(ClkCfg);

  void DBG_init_pin(char portletter, uint8_t portnum, uint32_t speed, uint32_t pupd_status, uint32_t init);
  void DBG_deinit_pin(char portletter, uint8_t portnum);


/*********************************************************************************
  * @brief  Init or Deinit GPIO Pin
  * @retval true on success, false otherwise
  ********************************************************************************/
static bool Init_GPIO ( char *cmdline, size_t len, const void * arg )
{
  UNUSED(cmdline);UNUSED(len);UNUSED(arg);
  char *word;
  size_t wordlen;
  char portletter;   
  uint8_t portnum;
  uint32_t fnarg = (uint32_t)arg;
  uint32_t speed, pupd, outval;
  switch ( fnarg ) {
    case 0:
      if ( CMD_argc() < 5 ) {
        printf("Usage: Pin Init portletter portnum speed pupd outval\n");
        return false;
      }

      CMD_get_one_word( &word, &wordlen);
      portletter = *word;

      CMD_get_one_word( &word, &wordlen);
      portnum = CMD_to_number(word, wordlen) & 0x0f;
      CMD_get_one_word( &word, &wordlen);
      speed = CMD_to_number(word, wordlen) & 0x03;
      CMD_get_one_word( &word, &wordlen);
      pupd = CMD_to_number(word, wordlen) & 0x03;
      CMD_get_one_word( &word, &wordlen);
      outval = CMD_to_number(word, wordlen) & 0x01;

        // RHB todo DBG_init_pin(portletter, portnum, speed, pupd, outval);
        break;
    case 1:
      if ( CMD_argc() < 2 ) {
        printf("Usage: Pin DeInit portletter portnum1 [portnum2 [...]]\n");
        return false;
      }

      CMD_get_one_word( &word, &wordlen);
      portletter = *word;

      while (CMD_get_one_word( &word, &wordlen) ) {
          portnum = CMD_to_number(word, wordlen) & 0x0f;
          // RHB todo DBG_deinit_pin(portletter, portnum);
      }
      break;
    case 2:
        break;
    default:
        DEBUG_PUTS("Not implemented");
  }
  return true;
}

#define DELAY_MS 500
#include "hardware/gpio.h"

/*********************************************************************************
  * @brief  Toggle GPIO ports
  *         
  * @retval true on success, false otherwise
  *
  * @note   will try to read as mamy port numbers as possible
  ********************************************************************************/
static bool Toggle_GPIO ( char *cmdline, size_t len, const void * arg )
{
  UNUSED(cmdline);UNUSED(len);UNUSED(arg);
  char *word;
  size_t wordlen;
  uint8_t pin;
  uint32_t times;
  
    switch( (uint32_t)arg )  {
    case 0:
      /* cycle <pin> <num> times */
      if ( CMD_argc() < 2 ) {
        printf("Cycle <pin> <num>\n");
        return false;
      }
      CMD_get_one_word( &word, &wordlen );
      pin  = CMD_to_number ( word, wordlen );
      CMD_get_one_word( &word, &wordlen );
      times  = CMD_to_number ( word, wordlen );
      pin_toggle_nowait( pin, DELAY_MS, times );
      break;
    case 1:
      /* Toggle output pin <pin> */
      if ( CMD_argc() < 1 ) {
        printf("Toggle <pin>\n");
        return false;
      }
      CMD_get_one_word( &word, &wordlen );
      pin  = CMD_to_number ( word, wordlen );
      times = gpio_get(pin) ? 0 : 1;
      gpio_put(pin, times);
      break;
    default:
      DEBUG_PUTS("command not implemented");
  } /* end switch */

  return true;
}

/*********************************************************************************
  * @brief  Dump GPIO ports
  *         
  * @retval true on success, false otherwise
  *
  * @note   will try to read as many GPIO port names as needed
  ********************************************************************************/
static bool Dump_GPIO ( char *cmdline, size_t len, const void * arg )
{
  char *word;
  size_t wordlen;
  uint8_t idx;
  UNUSED(cmdline);UNUSED(len);UNUSED(arg);

  if ( CMD_argc() < 1 ) {
    printf("GPIO [0|1]\n");
    return false;
  }
  CMD_get_one_word( &word, &wordlen );
  idx  = CMD_to_number ( word, wordlen );

  DBG_dump_gpio_status(idx);

  return true;
}

void DBG_sram(void);

static bool Devices_Menu ( char *cmdline, size_t len, const void * arg )
{
  char *word;
  size_t wordlen;
  uint8_t idx;
  uint32_t initarg;
    
  UNUSED(cmdline);UNUSED(len);

  switch( (uint32_t)arg )  {
    case 0:
      DBG_sram();
      break;
    case 1:
      // DBG_dump_exti_config(0);
      break;
    case 2:
      DBG_dump_nvic_config();
      break;
    default:
      DEBUG_PUTS("RFM_Menu: command not implemented");
  } /* end switch */

  return true;
} /* Devices_menu */


static const char *pmtDevices (void)
{
  return "Devices";
}

static const CommandSetT cmdDevices[] = {
  { "SRAM",                   ctype_fn, {Devices_Menu},    VOID(0), "Show SRAM sections and usage" },
  { "EXTI",                   ctype_fn, {Devices_Menu},    VOID(1), "Show EXTI settings" },
  { "NVIC",                   ctype_fn, {Devices_Menu},    VOID(2), "Show NVIC settings" },
  { "GPIO [0|1]",             ctype_fn, {Dump_GPIO},       VOID(0), "Show GPIO 0-15 or 16-29"  },
  { "Cycle <pin> <num>",      ctype_fn, {Toggle_GPIO},     VOID(0), "Cycle GPIO <pin> <num> times"  },
  { "Alter <pin>",            ctype_fn, {Toggle_GPIO},     VOID(1), "Toggle output of GPIO <pin>"  },
  { "Pin Init",               ctype_fn, {Init_GPIO},       VOID(0), "Set Output Pin"  },
  { "Pin DeInit",             ctype_fn, {Init_GPIO},       VOID(1), "Reset Output Pin"  },
  { "Disp Init",              ctype_fn, {Devices_Menu},    VOID(6), "Init 6 DD"  },
};
ADD_SUBMODULE(Devices);


#endif  // DEBUG_FEATURES >0



static bool Test_WD_reset ( char *cmdline, size_t len, const void * arg )
{
  UNUSED(cmdline);UNUSED(len);UNUSED(arg);
  char *word;
  size_t wordlen;

  if ( CMD_argc() < 1 || ( CMD_get_one_word( &word, &wordlen ), *word != 'X' && *word != 'x') ) {
      printf("Usage: 'Reset X\n");
      return false;
  }

  DEBUG_PUTS("Watchdog reset");

  /* the following call will not return */
  watchdog_reboot (0, 0, 100);

  return true;
}
 
#if DEBUG_MODE > 0
    static bool Test_Sleep ( char *cmdline, size_t len, const void * arg )
    {
      UNUSED(cmdline);UNUSED(len);UNUSED(arg);
      char *word;
      size_t wordlen;
      char c;

      if ( CMD_argc() < 1 || ( CMD_get_one_word( &word, &wordlen ), c = *word,  c != '0' && c != '1') ) {
          printf("Usage: 'Sleep {0|1}\n");
          return false;
      }

      if ( c == '0') {
          bAllowStop = false;
          DEBUG_PUTS("Inhibit Stop");
      } else {
          bAllowStop = true;
          DEBUG_PUTS("Allow Stop");
      }
  
      return true;
    } 
#endif

static uint8_t tmrID;
void TimerCB ( uint32_t arg)
{
   UNUSED(arg);
   COM_print_time('*', true);
}

static bool Test_TmrAbs ( char *cmdline, size_t len, const void * arg )
{
  UNUSED(cmdline);UNUSED(len);UNUSED(arg);
  char *word;
  size_t wordlen;

  if ( CMD_argc() < 1 ) {
      printf("Usage: 'TmrAbs nn [1] \n");
      return false;
  }

  CMD_get_one_word( &word, &wordlen );
  uint32_t ret = CMD_to_number ( word, wordlen );

  COM_print_time('?', true);
  DEBUG_PRINTF("Under Construction\n");

  return true;
}

static bool Test_TmrRel ( char *cmdline, size_t len, const void * arg )
{
  UNUSED(cmdline);UNUSED(len);UNUSED(arg);
  char *word;
  size_t wordlen;
  bool bPeriodic;

  if ( CMD_argc() < 1 ) {
      printf("Usage: 'TmrRel nn [1] \n");
      return false;
  }

  CMD_get_one_word( &word, &wordlen );
  uint32_t ret = CMD_to_number ( word, wordlen );
  uint32_t work = 0;

  if ( CMD_argc() >= 1 ) {
      CMD_get_one_word( &word, &wordlen );
      work = CMD_to_number ( word, wordlen );
      bPeriodic = work > 0;
      DEBUG_PRINTF("Periodic Rel Timer to %d\n", ret );
  } else {
      DEBUG_PRINTF("One Shot Rel Timer to %d\n", ret );
      bPeriodic = false;
  }
  COM_print_time('?', true);
  // tmrID = MsTimerSetRel ( MILLISEC_TO_TIMERUNIT(ret), bPeriodic, TimerCB, 0 );
  DEBUG_PRINTF("Under Construction\n");

  return true;
}

static bool Test_TmrDel ( char *cmdline, size_t len, const void * arg )
{
  UNUSED(cmdline);UNUSED(len);UNUSED(arg);
  DEBUG_PRINTF("Under Construction\n");
  // MsTimerDelete(tmrID);
  return true;
}

void TestCB(uint32_t arg)
{
    DEBUG_PRINTF("@%03d Tmr activation by iteration %d\n", RTC_GetMillis(), arg);
}

static bool Test_TmrMulti ( char *cmdline, size_t len, const void * arg )
{
  UNUSED(cmdline);UNUSED(len);UNUSED(arg);
  char *word;
  size_t wordlen;

  if ( CMD_argc() < 1 ) {
      printf("Usage: 'TmrMulti nn \n");
      return false;
  }

  CMD_get_one_word( &word, &wordlen );
  uint32_t ret = CMD_to_number ( word, wordlen );

  /* int8_t TmrID = MsTimerAllocate(NO_TIMER_ID);
  if ( TmrID == NO_TIMER_ID ) {
    DEBUG_PUTS("Could not allocate TimerID");
    return false;
  }
  */
  DEBUG_PRINTF("Under Construction\n");

  /*
  for ( uint32_t i = 0; i < ret; i++ ) {
    if ( MsTimerReUseRel(TmrID, MILLISEC_TO_TIMERUNIT(20), 0, TestCB, i) == NO_TIMER_ID ) 
        DEBUG_PUTS("Could not ReSet Timer");
    uint32_t cnt = RTC_GetS256()+2;
    while ( RTC_GetS256() != cnt );
  }
  */
  return true;
}

static bool Test_TmrKill ( char *cmdline, size_t len, const void * arg )
{
  UNUSED(cmdline);UNUSED(len);UNUSED(arg);
 
  int8_t TmrID;
  uint16_t cnt;
  uint32_t i = 0;

#define DELTA_CNT   4

  /* 
  TmrID=MsTimerSetRel(MILLISEC_TO_TIMERUNIT(15), 0, TestCB, i++ );
  if ( TmrID == NO_TIMER_ID ) {
    DEBUG_PUTS("TmrKill: Could not Set Timer 0");
    return false;
  }
   cnt = DELTA_CNT + RTC_GetTimer();
   while ( RTC_GetS256() == cnt );
  
  TmrID=MsTimerReUseRel(TmrID, MILLISEC_TO_TIMERUNIT(20), 0, TestCB, i++ );
  if ( TmrID == NO_TIMER_ID ) {
    DEBUG_PUTS("TmrKill: Could not Set Timer 1");
    return false;
  }
   cnt = DELTA_CNT + RTC_GetTimer();
   while ( RTC_GetS256() == cnt );
  
  TmrID=MsTimerReUseRel(TmrID, MILLISEC_TO_TIMERUNIT(25), 0, TestCB, i++ );
  if ( TmrID == NO_TIMER_ID ) {
    DEBUG_PUTS("TmrKill: Could not set timer 2");
    return false;
  }
   cnt = DELTA_CNT + RTC_GetTimer();
   while ( RTC_GetS256() == cnt );
  
  TmrID=MsTimerReUseRel(TmrID, MILLISEC_TO_TIMERUNIT(30), 0, TestCB, i++ );
  if ( TmrID == NO_TIMER_ID ) {
    DEBUG_PUTS("TmrKill: Could not set timer 3");
    return false;
  }
   cnt = DELTA_CNT + RTC_GetTimer();
   while ( RTC_GetS256() == cnt );
  
  TmrID=MsTimerReUseRel(TmrID, MILLISEC_TO_TIMERUNIT(35), 0, TestCB, i++ );
  if ( TmrID == NO_TIMER_ID ) {
    DEBUG_PUTS("TmrKill: Could not set timer 4");
    return false;
  }

   cnt = DELTA_CNT + RTC_GetTimer();
   while ( RTC_GetS256() == cnt );
  
  TmrID=MsTimerReUseRel(TmrID, MILLISEC_TO_TIMERUNIT(40), 0, TestCB, 0 );
  if ( TmrID == NO_TIMER_ID ) {
    DEBUG_PUTS("TmrKill: Could not set timer 5");
    return false;
  }
  */
  DEBUG_PRINTF("Under Construction\n");

  return true;
}

static bool Test_Toggle ( char *cmdline, size_t len, const void * arg )
{
  UNUSED(cmdline);UNUSED(len);UNUSED(arg);
  char *word;
  size_t wordlen;

  if ( CMD_argc() < 2 ) {
      printf("Usage: 'Toggle <steps> <period> \n");
      return false;
  }

  CMD_get_one_word( &word, &wordlen );
  uint32_t p1 = CMD_to_number ( word, wordlen );

  CMD_get_one_word( &word, &wordlen );
  uint32_t p2 = CMD_to_number ( word, wordlen );

  DEBUG_PRINTF("%d Steps, Period %d\n", p1,p2  );
  pin_toggle_wait( PICO_DEFAULT_LED_PIN, p2, p1 );
  return true;
}


#include "system/periodic.h"
/*********************************************************************************
  * @brief  Submenu for system functions
  *         
  * @retval true on success, false otherwise
  *
  * @note   will try to read as many parameters as needed
  ********************************************************************************/
static bool System_Menu ( char *cmdline, size_t len, const void * arg )
{
  typedef void (*func) (void);
  func fn=NULL;
  uint32_t *test = NULL;  
  UNUSED(cmdline);UNUSED(len);

  switch((uint32_t)arg) {
    case 0:
        TaskDumpList();
        break;
    case 1:
        PeriodicDumpList();
        break;
    case 2:
        fn();
        break;
    case 3:
        *test = 17;
        break;

    /* sample entry
    case 1:
        if ( CMD_argc() < 1 ) {
        printf("Usage: 'ePaper Init <pattern> \n");
        return false;
        }
        CMD_get_one_word( &word, &wordlen );
        pattern = CMD_to_number ( word, wordlen );
        // BSP_EPD_Init();
        EPD_Test(pattern);
    break;
    sample entry */
    default:
      DEBUG_PUTS("System_Menu: command not implemented");
  } /* end switch */

  return true;

} /* System_Menu */


static const char *pmtTest (void)
{
  return "T&S";
}

static const CommandSetT cmdTest[] = {
  { "Reset",           ctype_fn, .exec.fn = Test_WD_reset,  VOID(0), "Perform a watchdog reset" },
#if DEBUG_MODE > 0
  { "Sleep",           ctype_fn, .exec.fn = Test_Sleep,     VOID(0), "Allow/Inhibit sleep mode" },
#endif
  { "Task list",       ctype_fn, .exec.fn = System_Menu,    VOID(0), "Show task list" },
  { "Periodic lists",  ctype_fn, .exec.fn = System_Menu,    VOID(1), "Show periodic lists" },
  { "Call 0",          ctype_fn, .exec.fn = System_Menu,    VOID(2), "Provoke NULL ptr error" },
  { "Access 0",        ctype_fn, .exec.fn = System_Menu,    VOID(3), "write to NULL ptr" },

  { "TmrAbs",          ctype_fn, .exec.fn = Test_TmrAbs,    VOID(0), "Set Abs Timer" },
  { "TmrRel",          ctype_fn, .exec.fn = Test_TmrRel,    VOID(0), "Set Rel Timer" },
  { "TmrDel",          ctype_fn, .exec.fn = Test_TmrDel,    VOID(0), "Delete priodic timer" },
  { "Multi Tmr",       ctype_fn, .exec.fn = Test_TmrMulti,  VOID(0), "Lots of timers" }, 
  { "Kill Tmr",        ctype_fn, .exec.fn = Test_TmrKill,   VOID(0), "Generates Error" }, 
  { "Toggle",          ctype_fn, .exec.fn = Test_Toggle,    VOID(0), "Toggle UserLed2" }, 
};
ADD_SUBMODULE(Test);

#if defined(USE_ADC1)
    /*********************************************************************************
      * @brief  Submenu for FM24 Access functions
      *         
      * @retval true on success, false otherwise
      *
      * @note   will try to read as many parameters as needed
      ********************************************************************************/
    static bool ADC_Menu ( char *cmdline, size_t len, const void * arg )
    {
      char *word;
      size_t wordlen;
      uint8_t ch;

      UNUSED(cmdline);UNUSED(len);

      switch((uint32_t)arg) {
        case 0:
            ADC_Calibrate(&HW_ADC1);
            /* fall through */
        case 1:
            ADC_MeasureVdda ((void *)&HW_ADC1);
            printf("Vdda = %dmV\n", ADC1Handle.vdda); 
            break;
        case 2:
            ADC_MeasureChipTemp(&HW_ADC1);
            printf("Chip Temp = %dC\n", ADC1Handle.chiptemp); 
            break;
        case 3:
            if ( CMD_argc() < 1 ) {
              printf("Usage: 'ADC channel <num> \n");
              return false;
            }
            CMD_get_one_word( &word, &wordlen );
            ch = CMD_to_number ( word, wordlen );
            ADC_MeasureChannel(&HW_ADC1, ch);
            break;
        case 4:
            puts("Measure group manually"); 
            ADC_SetupGroup(&HW_ADC1, true);
            ADC_MeasureGroup(&HW_ADC1);
            ADC_ShowStatus(&HW_ADC1);
            break;
        case 8:
            puts("Measure group auto"); 
            ADC_SetupGroup(&HW_ADC1, false);
            ADC_MeasureGroup(&HW_ADC1);
            ADC_ShowStatus(&HW_ADC1);
            break;
        case 5:
            ADC_DisableRefintCh(&HW_ADC1);
            puts("Refint channel disabled"); 
            break;
        case 6:
            ADC_DisableAllInternalCh(&HW_ADC1);
            puts("Refint channel disabled"); 
            break;
        case 7:
            if ( CMD_argc() < 1 ) {
              printf("Usage: 'Peripheral 1/0\n");
              return false;
            }
            CMD_get_one_word( &word, &wordlen );
            ch = CMD_to_number ( word, wordlen );
            PeriphTimer_StartStop(ch!=0);
            break;
        default:
          DEBUG_PUTS("Test_Menu: command not implemented");
      } /* end switch */

      return true;
    }

    static const char *pmtADC (void)
    {
      return "ADC";
    }


    static const CommandSetT cmdADC[] = {
        { "Measure Vdda+Calibrate", ctype_fn, .exec.fn = ADC_Menu,VOID(0), "Calibrate ADC & Measure Vdda" },
        { "Measure Vdda",           ctype_fn, .exec.fn = ADC_Menu,VOID(1), "Measure Vdda w/o Calibration" },
        { "Measure Temp",           ctype_fn, .exec.fn = ADC_Menu,VOID(2), "Measure Chip temperature" },
        { "Measure channel",        ctype_fn, .exec.fn = ADC_Menu,VOID(3), "Measure channel <n>" },
        { "Measure group manually", ctype_fn, .exec.fn = ADC_Menu,VOID(4), "Measure whole sequence once manually" },
        { "Measure group auto",     ctype_fn, .exec.fn = ADC_Menu,VOID(8), "Measure whole sequence automatically repeated" },
        { "Refint disable",         ctype_fn, .exec.fn = ADC_Menu,VOID(5), "Disable Refint ADC channel" },
        { "All int. Ch. Disable",   ctype_fn, .exec.fn = ADC_Menu,VOID(6), "Disable all internal ADC channels" },
        { "Periph Timer Start/Stop",ctype_fn, .exec.fn = ADC_Menu,VOID(7), "Start/Stop the perpheral timer" },
    };
    ADD_SUBMODULE(ADC);
#endif

#if USE_LVGL > 0
#include "../lvgl/lvgl.h"

void lv_example_scale_3(void);
extern lv_style_t my_style;
static lv_obj_t * obj=NULL;
bool bSpiDMA=false;

    /*********************************************************************************
      * @brief  Submenu for LVGL test functions
      *         
      * @retval true on success, false otherwise
      *
      * @note   will try to read as many parameters as needed
      ********************************************************************************/
    static bool LVGL_Menu ( char *cmdline, size_t len, const void * arg )
    {
      char *word;
      size_t wordlen;
      uint8_t r,g,b;
      uint32_t temp;
      UNUSED(cmdline);UNUSED(len);

      switch((uint32_t)arg) {
        case 0:
            lv_obj_clean(lv_scr_act());
            break;
        case 1:
            if ( CMD_argc() < 1 ) {
              printf("Usage: 'BG color <24bit-RGB>\n");
              return false;
            } 
            CMD_get_one_word( &word, &wordlen );
            temp = CMD_to_number ( word, wordlen );
            /*Change the active screen's background color*/
            lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(temp), LV_PART_MAIN);
            break;
        case 2:
            if ( CMD_argc() < 3 ) {
              printf("Usage: 'BG color <r> <g> <b>\n");
              return false;
            } 
            CMD_get_one_word( &word, &wordlen );
            r = CMD_to_number ( word, wordlen );
            CMD_get_one_word( &word, &wordlen );
            g = CMD_to_number ( word, wordlen );
            CMD_get_one_word( &word, &wordlen );
            b = CMD_to_number ( word, wordlen );
            /*Change the active screen's background color*/
            lv_obj_set_style_bg_color(lv_screen_active(), lv_color_make(r,g,b), LV_PART_MAIN);
            break;
        case 3:
            if ( ! obj ) {
              obj = lv_obj_create(lv_screen_active());
              lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
              lv_obj_set_style_bg_color(obj,  lv_color_hex(0xFF0000), 0);
              lv_obj_add_style(obj, &my_style, LV_PART_MAIN);
              // lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, 0);
              lv_obj_set_size(obj, 120, 100); 
              lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
            }
            break;
        case 4:
            if ( obj ) {
              
              lv_obj_delete( obj );
              obj = NULL;
            }
            break;
        case 5:
            if ( !obj ) return false;
            if ( CMD_argc() < 3 ) {
              printf("Usage: 'Kr color <r> <g> <b>\n");
              return false;
            } 
            CMD_get_one_word( &word, &wordlen );
            r = CMD_to_number ( word, wordlen );
            CMD_get_one_word( &word, &wordlen );
            g = CMD_to_number ( word, wordlen );
            CMD_get_one_word( &word, &wordlen );
            b = CMD_to_number ( word, wordlen );
            /*Change the active screen's background color*/
            lv_obj_set_style_bg_color(obj,  lv_color_make(r,g,b), 0);
        case 6:
            if ( !obj ) return false;
            if ( CMD_argc() < 1 ) {
              printf("Usage: 'Border wid <w>[Pixel]\n");
              return false;
            } 
            CMD_get_one_word( &word, &wordlen );
            temp = CMD_to_number ( word, wordlen );
            /*Change the Border Width*/
            lv_obj_set_style_border_width(obj, temp, LV_PART_MAIN);
            break;
        case 7:
            if ( !obj ) return false;
            if ( CMD_argc() < 1 ) {
              printf("Usage: 'Border rad <r>[Pixel]\n");
              return false;
            } 
            CMD_get_one_word( &word, &wordlen );
            temp = CMD_to_number ( word, wordlen );
            /*Change the obj radius*/
            lv_obj_set_style_radius(obj, temp, LV_PART_MAIN);
            break;
            break;
        case 8:
            if ( CMD_argc() < 1 ) {
              printf("Usage: 'SPI-DMA [0|1]\n");
              return false;
            } 
            CMD_get_one_word( &word, &wordlen );
            temp = CMD_to_number ( word, wordlen );
            bSpiDMA = temp != 0;
            printf("SPI DMA is %s\n", bSpiDMA ? "On" : "Off"); 
            break;
        case 9:
            lv_example_scale_3();
            printf("Scale Example\n"); 
            break;
        default:
          DEBUG_PUTS("Lvgl-Menu: command not implemented");
      } /* end switch */

      return true;
    }

    static const char *pmtLVGL (void)
    {
      return "Lvgl";
    }


    static const CommandSetT cmdLVGL[] = {
        { "Clear Display",          ctype_fn, .exec.fn = LVGL_Menu,VOID(0), "Clear Display" },
        { "BG Color <24b-hex>",     ctype_fn, .exec.fn = LVGL_Menu,VOID(1), "Set Background color" },
        { "BG Color <r> <g> <b>",   ctype_fn, .exec.fn = LVGL_Menu,VOID(2), "Set Background color" },
        { "Draw red kringel",       ctype_fn, .exec.fn = LVGL_Menu,VOID(3), "Draw kringel" },
        { "Delete kringel",         ctype_fn, .exec.fn = LVGL_Menu,VOID(4), "Delete kringel" },
        { "Kr Color <r> <g> <b>",   ctype_fn, .exec.fn = LVGL_Menu,VOID(5), "Set Kringel color" },
        { "Set Border Width <w>",   ctype_fn, .exec.fn = LVGL_Menu,VOID(6), "Border width <n> Pixels" },
        { "Set Border Radius <r>",  ctype_fn, .exec.fn = LVGL_Menu,VOID(7), "Border radius <r> Pixels" },
        { "SPI-DMA [0|1]",          ctype_fn, .exec.fn = LVGL_Menu,VOID(8), "Enable/Disable SPI-DMA" },
        { "Scale-Example",          ctype_fn, .exec.fn = LVGL_Menu,VOID(9), "Display scale example" },
    };
    ADD_SUBMODULE(LVGL);
#endif

#if USE_SPI1 > 0
#include "dev/GC9A01.h"


    /*********************************************************************************
      * @brief  Submenu for LVGL test functions
      *         
      * @retval true on success, false otherwise
      *
      * @note   will try to read as many parameters as needed
      ********************************************************************************/
    static bool Spi1_Menu ( char *cmdline, size_t len, const void * arg )
    {
      char *word;
      size_t wordlen;
      uint8_t r,g,b;
      uint32_t temp;
      UNUSED(cmdline);UNUSED(len);
      uint8_t retbuf[6];

      switch((uint32_t)arg) {
        case 0:
            GC9A01_run_cfg_script();
            break;
        case 1:
           if ( CMD_argc() < 1 ) {
              printf("Usage: 'FillScr <color> - fill Screen\n");
              return false;
            } 
            CMD_get_one_word( &word, &wordlen );
            temp = CMD_to_number ( word, wordlen );
            GC9A01_fillScreen(temp);
            break;
        case 2:
           if ( CMD_argc() < 1 ) {
              printf("Usage: 'MADCTL byte - Set MADCTL byte\n");
              return false;
            } 
            CMD_get_one_word( &word, &wordlen );
            r = CMD_to_number ( word, wordlen );
            GC9A01_cmd_data(0x36, r);
            printf("MADCTL=0x%02x\n", r);
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        default:
          DEBUG_PUTS("Lvgl-Menu: command not implemented");
      } /* end switch */

      return true;
    }

    static const char *pmtSpi1 (void)
    {
      return "Disp-Spi";
    }


    static const CommandSetT cmdSpi1[] = {
        { "GC9A01 Init Sequence",   ctype_fn, .exec.fn = Spi1_Menu,VOID(0), "GC9A01 Init" },
        { "Fill Screen <color>",    ctype_fn, .exec.fn = Spi1_Menu,VOID(1), "Fill Scr w 565 color code" },
        { "MADCTL <x>",             ctype_fn, .exec.fn = Spi1_Menu,VOID(2), "Write MADCTL byte" },
    };
    ADD_SUBMODULE(Spi1);
#endif



/*********************************************************************************
  * @brief  Show/Alter the persistent EEPROM settings
  *         
  * @retval true on success, false otherwise
  ********************************************************************************/
bool Settings(char *cmdline, size_t len, const void * arg )
{
  UNUSED(cmdline); UNUSED(len); UNUSED(arg);
  char *word;
  size_t wordlen;
  uint32_t argnum = CMD_argc();
  uint8_t idx;
  uint8_t val;
  bool result;

  if ( argnum >= 2 ) {
      while ( argnum > 1 ) {
        CMD_get_one_word( &word, &wordlen );
        idx = CMD_to_number ( word, wordlen );
        CMD_get_one_word( &word, &wordlen );
        val = CMD_to_number ( word, wordlen );
        // RHB todo result = Config_SetVal(idx, val); 
        DEBUG_PRINTF("%s No. %d to 0x%02x\n", result? "Set" : "Failed to set", idx, val );
        argnum -= 2;
      }
  }
  
  /* Show all settings */
  // RHB todo Config_Dump();

  return true;
}
#ifdef RP2040_M0_0
  #define CORE1_VECTORTABLE   0x10100000
  #include "system/ipc.h"
  void main_core1(void);
  void Start_Core1(uint32_t launch)
  {
    uint32_t *c1vectors = (uint32_t *)CORE1_VECTORTABLE;
    /* First word is stack, second word is Reset_Vector */   
    IPC_entryfn startfn = (IPC_entryfn)c1vectors[1];
    printf("Core1 Start = 0x%08x\n", startfn);
    if ( launch > 0 ) {
     IPC_StartCore1 (startfn);
      printf("Core1 Started...\n");
    }
  }
#endif
/*********************************************************************************
  * @brief  MainMenu
  *         
  * @retval true on success, false otherwise
  ********************************************************************************/
static bool MainMenu(char *cmdline, size_t len, const void * arg )
{
    char *word;
    size_t wordlen;
    uint32_t val;

    UNUSED(cmdline); UNUSED(len);
    switch((uint32_t)arg) {
#if DEBUG_MODE > 0
        case 0:
            if ( CMD_argc() < 1 ) {
              printf("Usage: 'Level [<n>] - Show/Set Debuglevel\n");
            } else {
                CMD_get_one_word( &word, &wordlen );
                console_debuglevel = CMD_to_number ( word, wordlen );
            }
            printf("Console Debuglevel=%d\n", console_debuglevel);
            printf("FatFS   Debuglevel=%d\n", fatfs_debuglevel);
            break;
#endif
        case 1:
            Dump_VersionInfo();
            break;
#ifdef RP2040_M0_0
        case 2:
            if ( CMD_argc() < 1 ) {
              printf("Usage: 'Start 1 {0|1{} - 1=Start Core, 0=StartInfo of Core1\n");
              return false;
            } 
            CMD_get_one_word( &word, &wordlen );
            Start_Core1(CMD_to_number ( word, wordlen ) );
            break;
        case 3:
            if ( CMD_argc() < 1 ) {
              printf("Usage: 'FIFO Send <x> - send <x> to Core1\n");
              return false;
            } 
            CMD_get_one_word( &word, &wordlen );
            val = CMD_to_number ( word, wordlen );
            IPC_SignalCore0to1();
            printf("%d sent to Core1\n", val);
              
            break;
#endif
        default:
            DEBUG_PUTS("MainMenu: command not implemented");
            return false;
    }
    return true;
} 
static const char *pmtBasic (void)
{
  return "";
}


static const CommandSetT cmdBasic[] = {
  { "Settings",        ctype_fn,  .exec.fn = Settings,        VOID(0),  "Persistent settings"  },
#if DEBUG_FEATURES > 0
  { "Clock&Pwr",       ctype_sub, .exec.sub = &mdlClkCfg,      0,       "Clock & Power Config submenu" },
  { "Devices",         ctype_sub, .exec.sub = &mdlDevices,     0,       "Peripheral devices submenu" },
#endif
#if USE_SPI1 > 0
  { "SPi1 Test",       ctype_sub, .exec.sub = &mdlSpi1,        0,       "SPI1 test submenu" },
#endif
#if USE_LVGL > 0
  { "LVGL TEst",       ctype_sub, .exec.sub = &mdlLVGL,        0,       "LVGL test submenu" },
  { "Devices",         ctype_sub, .exec.sub = &mdlDevices,     0,       "Peripheral devices submenu" },
#endif

#if DEBUG_MODE > 0
  { "Level",           ctype_fn,  .exec.fn = MainMenu,        VOID(0),  "Set Debuglevel"  },
#endif
  { "Version",         ctype_fn,  .exec.fn = MainMenu,        VOID(1),  "Show version info"  },
#ifdef RP2040_M0_0
  { "StartCore1 {0|1}",ctype_fn,  .exec.fn = MainMenu,        VOID(2),  "Start core 1"  },
  { "Send Core1 <x>"  ,ctype_fn,  .exec.fn = MainMenu,        VOID(3),  "Send <x> to core 1"  },
#endif
#if defined(USE_ADC1)
  { "ADC"    ,         ctype_sub, .exec.sub = &mdlADC,         0,       "ADC submenu" },
#endif
#if DEBUG_PROFILING > 0
  { "Profile",         ctype_fn,  .exec.fn = ProfilerDump,     VOID(0), "Dump profiling info" },
#endif
  { "Test&System",     ctype_sub, .exec.sub = &mdlTest,        0,       "Test & System submenu" },
};
ADD_SUBMODULE(Basic);


/**
  * @}
  */
