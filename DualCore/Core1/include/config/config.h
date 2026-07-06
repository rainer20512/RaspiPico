/*
 * CONFIG configuration
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "config/global.h"
#include "debug.h"


/*
 ********************************************************************************
 * Code Versioning
 ********************************************************************************
 */ 
#define MAJOR_VERSION   0
#define MINOR_VERSION   07

/*
 ********************************************************************************
 * Configuration
 ********************************************************************************
 */ 

#define USE_LVGL          1           /* Set to 1 if LVGL is running in this part       */
#define USE_GUI_INTERFACE 1           /* Set to 1 is declarative GUI interface is used  */
#define USE_UART0         0
#define USE_UART1         1
#define DEBUG_UART        1
#define USE_SPI0          0  
#define USE_SPI1          1    


/*
 ********************************************************************************
 * useful macros
 ********************************************************************************
 */
  
#ifndef __cplusplus
    #include <stdbool.h>
#endif


#define UNUSED(a)       (void)(a)
#define _CONCAT(a,b)    a##b
#define CONCAT(a,b)     _CONCAT(a,b)

#define _STR(x) #x
#define STR(x) _STR(x)

// #define MIN(a, b)       (((a) < (b)) ? (a) : (b))
//#define MAX(a, b)       (((a) > (b)) ? (a) : (b))
#define ABS(a)          (((a) < 0  ) ? -1*(a) : (a) )
#define UCASE(x)        (((x) >= 'a' && (x) <= 'z') ? (x) & ~0x20 : (x) )

#if BUILD_CONFIG_STR > 0
    #define PPCAT_NXE(A, B)   A " = " #B "\r\n"
    #define PPCAT_E(A)     PPCAT_NXE(#A,A)

#define MK_CONFIGSTR(a,i) \
    const char ConfigStr##i[] = #a; \
    const uint8_t ConfigVal##i = a; 

    MK_CONFIGSTR(DEBUG_MODE,1)
    MK_CONFIGSTR(DEBUG_FEATURES,2)
    MK_CONFIGSTR(USE_LVGL,3)
    MK_CONFIGSTR(USE_UART0,4)
    MK_CONFIGSTR(USE_UART1,5)
    MK_CONFIGSTR(USE_SPI0,6)
    MK_CONFIGSTR(USE_SPI1,7)
    
    #define MAX_CONFIGSTR   7

    const char *ConfigStrings[MAX_CONFIGSTR] = 
        {
            ConfigStr1,  ConfigStr2,  ConfigStr3,  ConfigStr4,  ConfigStr5,  ConfigStr6,  ConfigStr7,  /* ConfigStr8,
            ConfigStr9,  ConfigStr10, ConfigStr11, ConfigStr12, ConfigStr13, ConfigStr14, ConfigStr15, ConfigStr16,
            ConfigStr17, ConfigStr18, ConfigStr19, ConfigStr20, ConfigStr21, ConfigStr22, ConfigStr23, ConfigStr24,
            ConfigStr25, ConfigStr26, ConfigStr27, ConfigStr28, ConfigStr29, ConfigStr30, ConfigStr31, ConfigStr32, */
        };
    const uint8_t ConfigValues[MAX_CONFIGSTR] = 
        {
            ConfigVal1,  ConfigVal2,  ConfigVal3,  ConfigVal4,  ConfigVal5,  ConfigVal6,  ConfigVal7,  /* ConfigVal8,
            ConfigVal9,  ConfigVal10, ConfigVal11, ConfigVal12, ConfigVal13, ConfigVal14, ConfigVal15, ConfigVal16,
            ConfigVal17, ConfigVal18, ConfigVal19, ConfigVal20, ConfigVal21, ConfigVal22, ConfigVal23, ConfigVal24,
            ConfigVal25, ConfigVal26, ConfigVal27, ConfigVal28, ConfigVal29, ConfigVal30, ConfigVal31, ConfigVal32, */
        };

#endif


#endif /* _CONFIG_H_ */