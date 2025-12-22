/******************************************************************************
 * Set the version number
 *****************************************************************************/

#ifndef __VERSION_H_
#define __VERSION_H_

#ifdef __cplusplus
extern "C" {
#endif

#define BUILD_CONFIG_STR  1
#include "config/config.h"

#include <stddef.h>

#define APP_NAME "LVGL App"

#define MCU "RP2040"

#define BOARD   "RoundPi"

/*-----------------------------------------------------------------------------
 * Don't change below
 *-----------------------------------------------------------------------------
 */

#define VERSION_NUMBER  "V" STR(MAJOR_VERSION) "." STR(MINOR_VERSION)

#define APP_STRING      APP_NAME " " VERSION_NUMBER 

#define MCU_STRING      "for " MCU " family" 

#define BOARD_STRING    "on board " BOARD

#define BUILD_STRING    "@" __DATE__ " " __TIME__ 

extern const char VersionString[];
void        Dump_VersionInfo(void);
uint32_t    GetConfigNumLines(void);
char        *GetConfigLine(char *retbuf, size_t maxlen, uint32_t idx, bool bAppendCrlf);


#ifdef __cplusplus
}
#endif

#endif // __VERSION_H
