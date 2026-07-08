/**
  ******************************************************************************
  * @file    debug_io.h
  * @author  Rainer
  * @brief   Implementation of a circular output buffer, that will automatically
  *          be written to DebugUart
  *
  ******************************************************************************
  *
  ******************************************************************************
  */

/** @addtogroup DEBUG_OUTBUF
  * @{
  */

#ifndef _DEBUG_IO_H_
#define _DEBUG_IO_H_
#include "config/config.h"
#include "debug.h"


void task_handle_out(uint32_t arg);
bool task_init_io(void);
void task_handle_com( uint32_t param);
bool rx_chars_available(void);

#if DEBUG_FEATURES > 0 && LOGTO_CONSOLE > 0
    uint8_t Console_Write(const char *data, uint32_t len );
    void Console_CRLF(void);
    uint8_t Console_Write_CRLF(const char *data, uint32_t len);
#endif

#endif /* _DEBUG_IO_H_ */

/**
  * @}
  */
