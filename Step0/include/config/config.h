/*
 * CONFIG configuration
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "config/global.h"
#include "debug.h"

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

#define MIN(a, b)       (((a) < (b)) ? (a) : (b))
#define MAX(a, b)       (((a) > (b)) ? (a) : (b))
#define ABS(a)          (((a) < 0  ) ? -1*(a) : (a) )
#define UCASE(x)        (((x) >= 'a' && (x) <= 'z') ? (x) & ~0x20 : (x) )



#endif /* _CONFIG_H_ */