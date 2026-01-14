/**
  ******************************************************************************
  * @file    rtc.c
  * @author  Rainer 
  * @brief   Handle the RTC stuff
  ******************************************************************************
  *
  ******************************************************************************
  * @addtogroup RTC
  * @{
  */
#include "config/config.h"
#include "debug/debug_helper.h"
#include "system/profiling.h"
#include "task/minitask.h"
#include "system/rtc.h"

#include "pico/time.h"


#define DEBUG_RTC           0

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Some checks */


/* 
 * year mod 100 = 0 is only every 400 years a leap year. 
 * we calculate only till the year 2255, so don't care
 */
#define RTC_NoLeapyear() (rtc.YY % 4) 

/* Private variables ---------------------------------------------------------*/
/* 
 * Flag for "switch back to wintertime has already been done" 
 * This is neccessary, because we have two times 03 a.m. in the night when
 * summertime ends
 */
static uint8_t rtc_DS = 0; 

uint32_t rtc_ms;        //!< \brief millisecond Counter

/* Private function prototypes -----------------------------------------------*/
static uint32_t RTC_IsLastSunday(void);

/* Exported variables --------------------------------------------------------*/
uint32_t uptime_mins = 0;
#if USE_SECONDTIMER > 0
    uint32_t secsFromStart=0;        /* counts the seconds since system start */
#endif 

/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
rtc_t rtc = { 26, 12, 12, 11, 59, 56, 1, 0 };

/* ---------------------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* ---------------------------------------------------------------------------*/

  /** day of month for each month from january to december */
  static const uint8_t RTC_DayOfMonthTable[] =
  { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  /**
   * @brief get the number of days of the actual month (1-12) and year (0-255: 2000-2255)
   *
   * @retval number of days for actual month (1-12) and year (0-255: 2000-2255*
   */
  static uint8_t RTC_DaysOfMonth()
  {
      uint8_t dom = RTC_DayOfMonthTable[rtc.MM-1];
      if ((rtc.MM == 2)&&(!RTC_NoLeapyear()))
          return 29; // leapyear feb=29
      return dom;
  }

static const uint16_t daysInYear [12] = {
	0, 
	31, 
	31+28, 
	31+28+31, 
	31+28+31+30,  
	31+28+31+30+31,
	31+28+31+30+31+30,
	31+28+31+30+31+30+31,
	31+28+31+30+31+30+31+31,
	31+28+31+30+31+30+31+31+30,
        31+28+31+30+31+30+31+31+30+31,
	31+28+31+30+31+30+31+31+30+31+30};

/******************************************************************************
 *  @brief set day of week for actual date
 *
 *  @note   valid for years from 2000 to 2255
 * 
 *  @retval 1=monday to 7=sunday, this is compatible to the RTC DOW-Coding
 *
 ******************************************************************************/
static void RTC_SetDayOfWeek(void)
{
    uint16_t day_of_year;
    uint16_t tmp_dow;

    // Day of year
    day_of_year = daysInYear[rtc.MM-1] + rtc.DD;
    if (rtc.MM > 2) { // february
        if (! RTC_NoLeapyear() ){
            day_of_year ++;
        }
    }
    // calc weekday
    tmp_dow = rtc.YY + ((rtc.YY-1) / 4) - ((rtc.YY-1) / 100) + day_of_year;
    // set DOW
    rtc.DOW = (uint8_t) ((tmp_dow + 5) % 7) +1;
    
}

/******************************************************************************
 *  @brief Check actual date/time for being a switch point to Summertime or to Wintertime
 *         i.e. last Sunday in March or last sunday in october
 * @param  None 
 *         
 * @retval None
 ******************************************************************************/
static void RTC_TimerCheckDS(void)
{
    // start of summertime: March, 2:00:00 ?
    if ((rtc.MM==3)&&(rtc.hh==2)){
        // Last Sunday ?
        if (RTC_IsLastSunday()){
            rtc.hh++; // 2:00 -> 3:00
        }
    }
    // end of summertime: October, 03:00, rtc_DS == 0
    if ((rtc.MM==10)&&(rtc.hh==3)&&(rtc_DS==0)){
        // Last Sunday ?
        if (RTC_IsLastSunday()){
            rtc.hh--; // 3:00 -> 2:00
            rtc_DS=1;
        }
    }
}


/*!
 *******************************************************************************
 *
 *  Add one day to date variables
 *
 *  \note
 *       calculate overflows, regarding leapyear, etc.
 *
 ******************************************************************************/
static void RTC_AddOneDay(void)
{
    uint8_t dom;
    // How many day has actual month
    dom = RTC_DaysOfMonth();

    if (++rtc.DD > dom) {                       // Next Month
        rtc.DD = 1;
        if (++rtc.MM > 12) {                    // Next year
            rtc.MM = 1;
            rtc.YY++;
        }
    // Clear Daylight saving Flag
    rtc_DS=0;
    }
    // next day of week
    rtc.DOW = (rtc.DOW %7)+1; // Monday = 1 Sat=7
}

/*!
 *******************************************************************************
 *
 *  Add one second to clock variables
 *
 * \note
 *    - calculate overflows, regarding leapyear, etc.
 *    - process daylight saving
 *       - last sunday in march 1:59:59 -> 3:00:00
 *       - last sunday in october 2:59:59 -> 2:00:00
 *         ONLY ONE TIME -> set FLAG RTC_DS
 *         reset FLAG RTC_DS on next day
 * \note Is executed in interrupt context, keep it short!
 ******************************************************************************/
void RTC_AddOneSecond(void)
{
    // Paket counter starts at zero with every new slave, so reset it every second
    rtc.pkt_cnt=0;
    if (++rtc.ss >= 60) {
        rtc.ss = 0;
        uptime_mins++;
        // add one minute
        if (++rtc.mm >= 60) {
            rtc.mm = 0;
            // add one hour
            if (++rtc.hh >= 24) {
                rtc.hh = 0;
                RTC_AddOneDay();
            }
            RTC_TimerCheckDS();        
        }
    }
    #if USE_SECONDTIMER > 0
        // increment rotating counter for seconds
        secsFromStart++;
    #endif
}



/* ---------------------------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/* ---------------------------------------------------------------------------*/

/**
 *******************************************************************************
 *
 *  \returns true if actual date is last sunday in march or october
 *
 ******************************************************************************/
static uint32_t RTC_IsLastSunday(void)
{
    /* Not a sunday ? */
    if (rtc.DOW != 7) return 0;
   
    // March or October ?
    if (rtc.MM == 3){
        // last seven days of month
        return (rtc.DD > (31-7));
    } else if (rtc.MM == 10){
        // last seven days of month
        return (rtc.DD > (30-7));
    } else {
        return 0;  // not march or october
    }    
}





void RTC_SetDateTime(uint8_t dd, uint8_t mm, uint8_t yy, uint8_t hr, uint8_t mi, uint8_t sec, uint16_t ms) 
{
  bool bChanged = false;

/*local*/ void modify ( uint8_t *curval, uint8_t newval )
/*local*/ {
/*local*/    if ( *curval != newval ) {
/*local*/      bChanged = true;
/*local*/      *curval = newval;
/*local*/    }
/*local*/ }

  /* store to my data structure */
  modify (&rtc.DD, dd);
  modify (&rtc.MM, mm);
  modify (&rtc.YY, yy);
  
  rtc.hh =hr;
  rtc.mm =mi;
  rtc.ss =sec;

  /* Update DoW, if date was changed */
  if ( bChanged ) RTC_SetDayOfWeek();

  rtc_ms =ms;


}


/* 
 * Some time formating functions with different types of output.
 * All functions use "rtcbuf" to return their NUL-terminated results
 * if passed parameter "retlen" is not NULL, the length of the time
 * string is written here in
 */

static char rtcbuf[30];

char* RTC_GetStrDateTimeMillis(uint32_t *retlen)
{
  sprintf(rtcbuf, "%02d.%02d.%02d %02d:%02d:%02d.%03d", rtc.DD, rtc.MM,rtc.YY, rtc.hh, rtc.mm, rtc.ss, RTC_GetMillis() );
  if ( retlen ) *retlen = 21;
  return rtcbuf;
}

char* RTC_GetStrDateTime(uint32_t *retlen)
{
#if USE_RTC > 0
  RTC_CopytoVar();
#endif
  sprintf(rtcbuf, "%02d.%02d.%02d %02d:%02d:%02d", rtc.DD, rtc.MM,rtc.YY, rtc.hh, rtc.mm, rtc.ss );
  if ( retlen ) *retlen = 18;
  return rtcbuf;
}

char* RTC_GetStrTimeMillis(uint32_t *retlen)
{
  sprintf(rtcbuf, "%02d:%02d:%02d.%03d", rtc.hh, rtc.mm, rtc.ss, RTC_GetMillis() );
  if ( retlen ) *retlen = 12;
  return rtcbuf;
}

char* RTC_GetStrTime(uint32_t *retlen)
{
  sprintf(rtcbuf, "%02d:%02d:%02d", rtc.hh, rtc.mm, rtc.ss );
  if ( retlen ) *retlen = 8;
  return rtcbuf;
}

#if 0
void RTC_DumpDateTime(void)
{  
  DEBUG_PUTS(RTC_GetStrDateTime(NULL));
}
#endif

char* RTC_GetStrSecMicros(uint32_t *retlen)
{
#if USE_BASICTIMER > 0
    register uint32_t work = rtc.ss % 10;
    register uint32_t work10 = rtc.ss / 10;

    /* first 3 chars are seconds + ':' */
    char *ptr = rtcbuf;
    *ptr++  = '0' + work10;
    *ptr++  = '0' + work;
    *ptr++  = ':';

    /* Now 7 chars from right to left for micro- and milliseconds */
    ptr+= 7;
    *ptr-- = '\0';
    work = ProfilerGetMicrosecond();
    for ( register int32_t i = 6; i > 0; i-- ) {
        work10 = work/10;
        *ptr-- = '0' + (char)(work-work10*10);
        if ( i == 4) *ptr-- = '.';
        work = work10;
    }
    if ( retlen ) *retlen = 10;
    return rtcbuf;
#else
    if ( retlen ) *retlen = 10;
    return "*NoMicros*";
#endif
    
}

/******************************************************************************
 * Implements a simple stopwatch that counts milliseconds since start of stop
 * watch. 
 * \note The granularity is the granularity of the system timer
 * \note maximum delta-t is one hour
 *****************************************************************************/

/******************************************************************************
 * @brief  Returns the number of milliseconds since beginning of hour
 ******************************************************************************/
uint32_t RTC_GetMillisOfHour(void)
{
    register uint32_t ret;

    ret = (uint32_t)   RTC_GetMinute();
    ret = ret * 60   + RTC_GetSecond();
    ret = ret * 1000 + RTC_GetMillis();

    return ret;
}

#define RTC_MAX_MILLIS_OF_HOUR        (60*60*1000)
static uint32_t starttime;                              // Starttime of stopwatch 
static bool bStopWatchInUse = false;                    // flag for "stopwatch in use"

/******************************************************************************
 * \brief  Compute the delta-t between start of stopwatch and now
 * \note   Checks whether stopwatch is started, if not 0 will be returned
 ******************************************************************************/
static uint32_t StopWatch_GetDelta(void)
{
    /* check if stopwatch is started */
    if ( !bStopWatchInUse ) {
        DEBUG_PUTS("RTC Stopwatch not started!");
        return 0;
    }    

    /* get delta t from start of stopwatch to now */
    uint32_t tnow = RTC_GetMillisOfHour();
    if ( tnow < starttime ) tnow += RTC_MAX_MILLIS_OF_HOUR;

    return tnow-starttime;
}

/******************************************************************************
 * \brief  Start the simple stopwatch
 ******************************************************************************/
void RTC_StopWatch_Start(void)
{
    if ( bStopWatchInUse ) {
        DEBUG_PUTS("Error: RTC Stopwatch in Use!");
        return;
    }    

    starttime       = RTC_GetMillisOfHour();
    bStopWatchInUse = true;
}

/******************************************************************************
 * \brief return the milliseconds since start of stopwatch
 ******************************************************************************/
uint32_t RTC_StopWatch_GetTime(void)
{
    return StopWatch_GetDelta();
}

/******************************************************************************
 * \brief stop the stopwatch and return the milliseconds since start of stopwatch
 ******************************************************************************/
uint32_t RTC_StopWatch_Stop(void)
{
    register uint32_t ret   =   StopWatch_GetDelta();
    bStopWatchInUse         = false;

    return ret;
}

/******************************************************************************
 * \brief returns true, iff the stopwatch is in use
 ******************************************************************************/
bool RTC_StopWatch_InUse(void)
{
    return bStopWatchInUse;
}


/*********************************************************************************
 * @brief  Increment second by one and activate RTC task. This routine will be
 *         called normally ( once a second ) out of the secondly interrupt handler
 *         or manually in case of negative time correction
 ********************************************************************************/
// static inline __attribute__((always_inline))
void RTC_IncrementSecond ( void )
{
    RTC_AddOneSecond();
    // DEBUG_PRINTF("+ %d\n",HAL_GetTick());
    TaskNotify(TASK_RTC);
    TaskNotify(TASK_PERIODIC);
}


/* ---------------------------------------------------------------------------*/
/* Initialization ------------------------------------------------------------
 * get a repeating timer, that fires every ms 
 * ---------------------------------------------------------------------------*/


static repeating_timer_t mstimer;
static repeating_timer_t sectimer;

#if USE_LVGL > 0
  #include "../../lvgl/lv_conf.h"
  void lv_tick_inc(uint32_t);
#endif

bool mstimer_callback(repeating_timer_t *rt) {
    UNUSED(rt);
    ProfilerPush(JOB_IRQ_RTC);
    ++rtc_ms;
    #if USE_LVGL > 0
      lv_tick_inc(1);
      // TaskNotify(TASK_LVGL);
    #endif
    ProfilerPop();
    return true; // keep repeating
}

bool sectimer_callback(repeating_timer_t *rt) {
    UNUSED(rt);
    ProfilerPush(JOB_IRQ_RTC);
    rtc_ms = 0;
    RTC_IncrementSecond();
    ProfilerPop();
    return true; // keep repeating
}

bool task_init_rtc(void) {
  rtc_ms=0;
  // negative timeout means exact delay (rather than delay between callbacks)
  if (!add_repeating_timer_ms(-1, mstimer_callback, NULL, &mstimer)) {
      DEBUG_PRINTF("Failed to add millisecond timer\n");
      return false;
  }
  if (!add_repeating_timer_ms(-1000, sectimer_callback, NULL, &sectimer)) {
      DEBUG_PRINTF("Failed to add second timer\n");
      return false;
  }

  return true;
}

void handle_sectimer_periodic(void);

/*********************************************************************************
  * @brief  callback for every rtc event, i.e. with every second increment
  *         what todo: check the second timers         
  *         
  * @param  None
  *                       
  * @retval None
  *
  ********************************************************************************/
void task_handle_rtc(uint32_t arg)
{

    UNUSED(arg);

    #if DEBUG_PRINT_ADDITIONAL_TIMESTAMPS > 0
        COM_print_time('+', true); 
    #endif


}


/****************************************************************
 Compress the actual date in a 4-byte-integer
 Byte 0: yy yy yy yy yy yy mm mm
 Byte 1: mm mm dd dd dd dd dd hh
 Byte 2: hh hh hh hh mi mi mi mi
 Byte 3: mi mi ss ss ss ss ss ss
****************************************************************/
 
void compress_datetime ( union CTIME_struct *ctime, rtc_t *rtc )
{
    uint8_t i=0;
    uint8_t w1                  = rtc->MM;
    ctime->timebyte[i++]	= (rtc->YY<<2) | (w1 >> 2);
    uint8_t w2 			= rtc->hh; 
    ctime->timebyte[i++]	= (w1 << 6) | (rtc->DD<<1) | (w2 >> 4);
    w1				= rtc->mm;
    ctime->timebyte[i++] 	= (w2<<4)|(w1>>2); 
    ctime->timebyte[i  ]	= (w1<<4)| rtc->ss;
}

/****************************************************************
 Uncompress a compressed datetime-value
****************************************************************/
void uncompress_datetime ( union CTIME_struct *ctime, rtc_t *rtc )
{
	uint8_t i=0;
	uint8_t w1,w2;
	
	w1 = ctime->timebyte[i++];				// Byte 0
	w2 = ctime->timebyte[i++];				// Byte 1
	rtc->YY = w1>>2;
	rtc->MM = ((w1<<2)&0b1100) | (w2>>6);
	rtc->DD = (w2>>1)&0b11111;

	w1 = ctime->timebyte[i++];				// Byte 2
	rtc->hh = ((w2&0b1)<<4) | (w1>>4);
	w2 = ctime->timebyte[i  ];				// Byte 3
	rtc->mm = ((w1&0b1111)<<2) | (w2>>6);
	rtc->ss = (w2&0b111111);
}



/**
  * @}
  */
