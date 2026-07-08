/**
  ******************************************************************************
  * @file    rtc.h
  * @author  Rainer
  * @brief   Handle the RTC stuff
  *
  ******************************************************************************
  * @addtogroup RTC
  * @{
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H
#define __RTC_H

#include "config/config.h"


/* Public define ------------------------------------------------------------*/
/* 
 * Define the timer that will be used for time generation on LSE Osc. base 
 * Its granularity will be 1/256 s, or coarse: 4ms
 * Change all defines, if counter is changed 
 * Also Change Clock Selection in PeriphClockConfig in clockconfig.c
 */

#ifdef __cplusplus
 extern "C" {
#endif

//! day of week
typedef enum {
    MON=0, TUE, WED, THU, FRI, SAT, SUN,  
} rtc_dow_t;

/***********************************************************************************************//**
 * \struct rtc_t
 * \brief time structure for a complete date/time structure 
 *
 * may be used for encryption, in this case it must be 8 byte long identical in master and slaves 
 **************************************************************************************************/ 
typedef struct {
    uint8_t YY;         //!< \brief Date: Year (0-255) -> 2000 - 2255
    uint8_t MM;         //!< \brief Date: Month
    uint8_t DD;         //!< \brief Date: Day
    uint8_t hh;         //!< \brief Time: Hours
    uint8_t mm;         //!< \brief Time: Minutes
    uint8_t ss;         //!< \brief Time: Seconds
    uint8_t DOW;        //!< \brief Time: Day of week 
    uint8_t pkt_cnt;    //!< \brief Rolling packet counter
} rtc_t;

extern uint32_t         rtc_ms;        //!< \brief Date: Year (0-255) -> 2000 - 2255
extern rtc_t            rtc;
extern uint32_t         uptime_mins;

uint32_t RTC_GetMs(void);

void RTC_SetDateTime(uint8_t dd, uint8_t mm, uint8_t yy, uint8_t hr, uint8_t mi, uint8_t sec, uint16_t ms);

char* RTC_GetStrSecMicros(uint32_t *retlen);
char* RTC_GetStrDateTimeMillis(uint32_t *retlen);
char* RTC_GetStrDateTime(uint32_t *retlen);
char* RTC_GetStrTimeMillis(uint32_t *retlen);
char* RTC_GetStrTime(uint32_t *retlen);

void task_handle_rtc(uint32_t);
bool task_init_rtc(void);

#define RTC_GetMillis()               (rtc_ms)
#define RTC_GetSecond()               (rtc.ss)
#define RTC_GetMinute()               (rtc.mm)
#define RTC_GetHour()                 (rtc.hh)
#define RTC_GetDay()                  (rtc.DD)
#define RTC_GetMonth()                (rtc.MM)
#define RTC_GetYearYY()               (rtc.YY)

/* Implementation of a simple stopwatch */
void     RTC_StopWatch_Start          (void);
uint32_t RTC_StopWatch_GetTime        (void);
uint32_t RTC_StopWatch_Stop           (void);
bool     RTC_StopWatch_InUse          (void);

#if DEBUG_PROFILING > 0
    /* 
     * if profiling is acive, we need functions to access the RTC registers directly 
     * because time is read when rtc variable may not be synced 
     */
    #define HW_GetMillis()            RTC_GetMillis()
    #if USE_RTC > 0
        #define HW_GetMinute()            (\
                                              (( RTC->TR & RTC_TR_MNT_Msk ) >> RTC_TR_MNT_Pos) * 10 \
                                            + (( RTC->TR & RTC_TR_MNU_Msk ) >> RTC_TR_MNU_Pos) \
                                          )
        #define HW_GetSecond()            (\
                                              (( RTC->TR & RTC_TR_ST_Msk ) >> RTC_TR_ST_Pos) * 10 \
                                            + (( RTC->TR & RTC_TR_SU_Msk ) >> RTC_TR_SU_Pos) \
                                          )
    
    #elif USE_TIMER > 0
        #define HW_GetMinute()      RTC_GetMinute()
        #define HW_GetSecond()      RTC_GetSecond()
    #endif
#endif

union CTIME_struct {
    struct {
        uint32_t   timeint;
    };
    struct {
        uint8_t    timebyte[4];
    }; 
};
void compress_datetime   ( union CTIME_struct *ctime, rtc_t *rtc );
void uncompress_datetime ( union CTIME_struct *ctime, rtc_t *rtc );



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RTC_H */

/**
  * @}
  */
