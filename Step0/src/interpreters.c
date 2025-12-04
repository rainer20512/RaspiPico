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
#include "system/clockconfig.h"
#include "config/devices_config.h"
#include "interpreters.h"
#include "debug_helper.h"
#include "system/profiling.h"
#include "system/util.h"
#include "wireless.h"
#include "timer.h"
#include "rfm/rfm_packets.h"
#include "task/minitask.h"
#include "com.h"
#include "dev/i2c_dev.h"
#include "system/status.h"
#include "system/hw_util.h"
#include "cmdline.h"
#include "sensors/thp_sensor.h"
#include "eeprom.h"
#include "dev/devices.h"
#include "system/tm1637.h"
#include "version.h"

#if USE_DS18X20  > 0
    #include "onewire.h"
    #include "ds18xxx20.h"
#endif
#if USE_EPAPER > 0
    #include "disp/epaper.h"
#endif
#if USE_DOGM132 > 0
    #include "disp/dogm-graphic.h"
    #include "disp/fonts/lcd_fonts.h"
#endif
#if USE_QENCODER > 0
    #include "dev/qencode.h"
#endif
#if USE_THPSENSOR > 0
    #include "sensors/thp_sensor.h"
#endif

#if USE_HW_PWMTIMER > 0 || USE_USER_PWMTIMER > 0
    #include "dev/pwm_timer.h"
#endif
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

#if DEBUG_FEATURES > 0

#include "debug_sram.h"
#include "debug_gpio_exti.h"
#include "debug_pwr_rcc.h"

void SystemClock_Set(CLK_CONFIG_T clk_config_byte, bool bSwitchOffMSI );

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
      DBG_dump_clocksetting();
      break;
    case 1:
      DBG_dump_rtcclockconfig();
      break;
    case 2:
      DBG_dump_powersetting();
      break;
    case 3:
      DBG_dump_peripheralclocksetting();
      break;
    case 4:
      DBG_dump_peripheralclockconfig();
      break;
    case 5:
      DBG_dump_peripheralclocksetting_insleepmode();
      break;
    case 6:
      for ( uint32_t i = 0; i < 5; i++ )
         Config_Menu( cmdline, len, VOID(i) );
      break;
    case 7:
      if ( CMD_argc() < 1 ) {
          printf("Usage: 'SetSysClk nn \n");
          return false;
      }
      CMD_get_one_word( &word, &wordlen );
      ret = CMD_to_number ( word, wordlen );
      SystemClock_Set(ret, true);
      break;
    case 8:
      DEBUG_PUTS("Calibrating HSI clock");
      HSIClockCalibrate();
      break;
    case 9:
      if ( CMD_argc() < 1 ) {
          printf("Usage:MCO output <n>, 0=Off, 1=SYSCLK, 2=MSI, 3=HSI, 4=HSE, 5=PLL, 6=LSE, 7=LSI");
          return false;
      }
      CMD_get_one_word( &word, &wordlen );
      ret = CMD_to_number ( word, wordlen );
      DEBUG_PRINTF("MCO Source %d\n", ret );
      EnableMCO ( ret );
      break;
    default:
      DEBUG_PUTS("RFM_Menu: command not implemented");
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
  { "Power",     ctype_fn, {Config_Menu},  VOID(2), "Show powersettings" },
  { "PeriClk",   ctype_fn, {Config_Menu},  VOID(3), "Show peripheral clock settings"  },
  { "PeriCfg",   ctype_fn, {Config_Menu},  VOID(4), "Show peripheral clock configuration"  },
  { "SleepClk",  ctype_fn, {Config_Menu},  VOID(5), "Show peripheral clock settings in sleep mode"  },
  { "All",       ctype_fn, {Config_Menu},  VOID(6), "Show Items 0) to 4) "  },
  { "SetSysClk", ctype_fn, {Config_Menu},  VOID(7), "Set system clock to predefined settings"  },
  { "Calib-HSI", ctype_fn, {Config_Menu},  VOID(8), "Calibrate HSI Clock"  },
  { "MCO output",ctype_fn, {Config_Menu},  VOID(9), "Activate MCO Output PA8" },
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

        DBG_init_pin(portletter, portnum, speed, pupd, outval);
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
          DBG_deinit_pin(portletter, portnum);
      }
      break;
    case 2:
        break;
    default:
        DEBUG_PUTS("Not implemented");
  }
  return true;
}

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
  char portletter;   
  uint8_t portnum;
  
  if ( CMD_argc() < 2 ) {
    printf("Usage: Toggle portletter [portnum1 [portnum2[...]]\n");
    return false;
  }

  CMD_get_one_word( &word, &wordlen);
  portletter = *word;

  while (CMD_get_one_word( &word, &wordlen) ) {
      portnum = CMD_to_number(word, wordlen) & 0x0f;
      DBG_dump_toggle_pin(portletter, portnum,(uint32_t)arg==1);
  }
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
  UNUSED(cmdline);UNUSED(len);UNUSED(arg);
  char *word;
  size_t wordlen;

  if ( CMD_argc() < 1 ) {
    printf("Usage: GPIO letter [letter [...]]\n");
    return false;
  }

  while (CMD_get_one_word( &word, &wordlen) ) {
    for ( uint32_t i = 0; i < wordlen; i++ ) {
      DBG_dump_gpio_status(*(word+i));
    }
  }
  return true;
}

static bool Devices_Menu ( char *cmdline, size_t len, const void * arg )
{
  char *word;
  size_t wordlen;
  uint8_t idx;
  uint32_t initarg;
  ARD_PinT dio, clk;
    
  UNUSED(cmdline);UNUSED(len);

  switch( (uint32_t)arg )  {
    case 0:
      DBG_sram();
      break;
    case 1:
      DBG_dump_exti_config(0);
      break;
    case 2:
      DBG_dump_nvic_config();
      break;
    case 3:
      DBG_dump_devices(true);
      break;
    case 4:
    case 5:
      if ( CMD_argc() < 1 ) {
        printf("Usage: 'De/ReInit <devicenum>\n");
        return false;
      }
      CMD_get_one_word( &word, &wordlen );
      idx = CMD_to_number ( word, wordlen );
      if ( (uint32_t)arg == 5) {
        initarg = 0;
        if ( CMD_argc() > 1 ) {
          CMD_get_one_word( &word, &wordlen );
          initarg = CMD_to_number ( word, wordlen );
        }
        DeviceInitByIdx(idx, (void *)initarg);
      } else {
        DeviceDeInitByIdx(idx);
      }
      break;
    case 6:
      clk.gpio = GPIOA; clk.pin = 0;  
      dio.gpio = GPIOA; dio.pin = 1;  
      TM1637_Init(clk,dio, 100);
      break;
    case 7:
    case 10:
      if ( CMD_argc() < 1 ) {
        printf("Usage: 'Display <number>'\n");
        return false;
      }
      CMD_get_one_word( &word, &wordlen );
      initarg = CMD_to_number ( word, wordlen );
      if ( (uint32_t)arg == 7 )
          TM1637_displayInteger(initarg,0, 99);
      else
        TM1637_displayHex(initarg,0, 99);
      break;
    case 8:
      if ( CMD_argc() < 1 ) {
        printf("Usage: 'Display <number>'\n");
        return false;
      }
      CMD_get_one_word( &word, &wordlen );
      initarg = CMD_to_number ( word, wordlen );
      TM1637_displayInteger(initarg,1, 2);
      break;
    case 9:
      TM1637_clearDisplay();
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
  { "SRAM",      ctype_fn, {Devices_Menu},    VOID(0), "Show SRAM sections and usage" },
  { "EXTI",      ctype_fn, {Devices_Menu},    VOID(1), "Show EXTI settings" },
  { "NVIC",      ctype_fn, {Devices_Menu},    VOID(2), "Show NVIC settings" },
  { "GPIO",      ctype_fn, {Dump_GPIO},       VOID(0), "Show GPIO settings"  },
  { "Devices",   ctype_fn, {Devices_Menu},    VOID(3), "Dump Devices"  },
  { "Dev Init",  ctype_fn, {Devices_Menu},    VOID(4), "DeInit Device i"  },
  { "Dev DeInit",ctype_fn, {Devices_Menu},    VOID(5), "ReInit Device i"  },
  { "Toggle 8x", ctype_fn, {Toggle_GPIO},     VOID(0), "Toggle GPIO pin 8x"  },
  { "Alter",     ctype_fn, {Toggle_GPIO},     VOID(1), "Alter output value of pin"  },
  { "Pin Init",  ctype_fn, {Init_GPIO},       VOID(0), "Set Output Pin"  },
  { "Pin DeInit",ctype_fn, {Init_GPIO},       VOID(1), "Reset Output Pin"  },
  { "Disp Init", ctype_fn, {Devices_Menu},    VOID(6), "Init 6 DD"  },
  { "Disp Num4", ctype_fn, {Devices_Menu},    VOID(7), "Write <num> to 6 DD"  },
  { "Disp Num6", ctype_fn, {Devices_Menu},    VOID(8), "Write <num> to 6 DD"  },
  { "Disp hex",  ctype_fn, {Devices_Menu},    VOID(10),"Write <hex> to 6 DD"  },
  { "Disp Clr",  ctype_fn, {Devices_Menu},    VOID(9), "Clear display"  },
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
  TimerWatchdogReset(500);

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
  tmrID = MsTimerSetAbs ( MILLISEC_TO_TIMERUNIT(ret), TimerCB, 0 );

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
  tmrID = MsTimerSetRel ( MILLISEC_TO_TIMERUNIT(ret), bPeriodic, TimerCB, 0 );

  return true;
}

static bool Test_TmrDel ( char *cmdline, size_t len, const void * arg )
{
  UNUSED(cmdline);UNUSED(len);UNUSED(arg);
  MsTimerDelete(tmrID);
  return true;
}

void TestCB(uint32_t arg)
{
    DEBUG_PRINTF("@%02x Tmr activation by iteration %d\n", RTC_GetS256(), arg);
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

  int8_t TmrID = MsTimerAllocate(NO_TIMER_ID);
  if ( TmrID == NO_TIMER_ID ) {
    DEBUG_PUTS("Could not allocate TimerID");
    return false;
  }

  for ( uint32_t i = 0; i < ret; i++ ) {
    if ( MsTimerReUseRel(TmrID, MILLISEC_TO_TIMERUNIT(20), 0, TestCB, i) == NO_TIMER_ID ) 
        DEBUG_PUTS("Could not ReSet Timer");
    uint32_t cnt = RTC_GetS256()+2;
    while ( RTC_GetS256() != cnt );
  }
  return true;
}

static bool Test_TmrKill ( char *cmdline, size_t len, const void * arg )
{
  UNUSED(cmdline);UNUSED(len);UNUSED(arg);
 
  int8_t TmrID;
  uint16_t cnt;
  uint32_t i = 0;

#define DELTA_CNT   4

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
  UserPinSignalN(p1, p2);
  return true;
}

static bool Test_Bitband ( char *cmdline, size_t len, const void * arg )
{
  uint32_t *start, *stop;
  UNUSED(cmdline);UNUSED(len);UNUSED(arg);
  DEBUG_PRINTF("LPUART1->CR1=%04x\n",(uint16_t)LPUART1->CR1);
  
  start = HW_GetPeriphBitBandAddr(&LPUART1->CR1,0);
  stop  = HW_GetPeriphBitBandAddr(&LPUART1->CR1,15);
  for ( ;start <= stop; start++ )
    DEBUG_PRINTF("%04x ",(uint16_t)*start);
  DEBUG_PUTS("");
 
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
  { "Bit-band test",   ctype_fn, .exec.fn = Test_Bitband,   VOID(0), "Test Bit-banding"}, 
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
        result = Config_SetVal(idx, val); 
        DEBUG_PRINTF("%s No. %d to 0x%02x\n", result? "Set" : "Failed to set", idx, val );
        argnum -= 2;
      }
  }
  
  /* Show all settings */
  Config_Dump();

  return true;
}
/*********************************************************************************
  * @brief  MainMenu
  *         
  * @retval true on success, false otherwise
  ********************************************************************************/
static bool MainMenu(char *cmdline, size_t len, const void * arg )
{
    char *word;
    size_t wordlen;

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
#if DEBUG_MODE > 0
  { "Level",           ctype_fn,  .exec.fn = MainMenu,        VOID(0),  "Set Debuglevel"  },
#endif
  { "UID",             ctype_fn,  .exec.fn = HW_DumpID,       VOID(0),  "Print Hardware UID"  },
  { "Version",         ctype_fn,  .exec.fn = MainMenu,        VOID(1),  "Show version info"  },
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
