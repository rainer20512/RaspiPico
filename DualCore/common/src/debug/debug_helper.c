/**
  ******************************************************************************
  * @file    debug_helper.c
  * @author  Rainer
  * @brief   Miscellaneous functions to format debug output
  *
  ******************************************************************************
  *
  ******************************************************************************
  */

/** @addtogroup DEBUG_UTIL
  * @{
  */

#include "config/config.h"
#include "debug/debug_helper.h"

#if  DEBUG_PRINT_ADDITIONAL_TIMESTAMPS > 0
  #include "system/rtc.h"
#endif

#if DEBUG_FEATURES > 0 || DEBUG_PROFILING  > 0

#include <stdarg.h>
#include <stdio.h>
#include <string.h>


#if ( DEBUG_PROFILING > 0 && DEBUG_MODE > 0 ) 
    #include "system/profiling.h"
#endif

#define MAX_INDENTLEN 12

static int myIndent=0;
static int myDesiredLen=0;


/*
 *************************************************************
 * Helper functions to dump controller settings
 *************************************************************
 */
int DBG_setIndentRel ( int delta ) {
  int ret = myIndent;
  myIndent += delta;
  return ret;
}

int DBG_setIndentAbs ( int absval ) {
  int ret = myIndent;
  myIndent = absval;
  return ret;
}

int DBG_setPadLen ( int len ) {
  int ret = myDesiredLen;
  myDesiredLen = len;
  return ret; 
}


static void DBG_do_indent ( void )
{
  for ( int i=0; i < myIndent; i++ )
    DEBUG_PUTC(' ');
}

void DBG_strpadright(const char *text, uint32_t desiredlen, char padchar )
{
  int padlen = desiredlen - strlen(text);
  DEBUG_PRINTF("%s", text);
  if ( padlen > 0 ) {
    for ( int i=0; i < padlen; i++ )
      DEBUG_PUTC(padchar);
  }
}

void DBG_strpadright2(const char *text1, const char *text2, uint32_t desiredlen, char padchar )
{
  int padlen = desiredlen - strlen(text1) - strlen(text2) - 1;
  DEBUG_PRINTF("%s %s", text1, text2);
  if ( padlen > 0 ) {
    for ( int i=0; i < padlen; i++ )
      DEBUG_PUTC(padchar);
  }
}


void DBG_printf_indent(const char *format, ...)
{
  DBG_do_indent();
  va_list args;
  va_start(args, format);
  DEBUG_VPRINTF(format, args);
  va_end(args);

}

void DBG_dump_text(const char *text) 
{
  DBG_do_indent();
  DEBUG_PRINTF(" %s\n", text);
}

void DBG_dump_textvalue(const char *text, const char *content ) 
{
  DBG_do_indent();
  if ( content ) {
      DBG_strpadright(text, myDesiredLen, '.');
      DEBUG_PRINTF(" %s\n", content);
  } else {
      DEBUG_PRINTF(" %s\n", text);
  }
}


void DBG_dump_number (const char *text, uint32_t num )
{
  DBG_do_indent();
  DBG_strpadright(text, myDesiredLen, '.');
  DEBUG_PRINTF(" %u\n", num );
}

void DBG_dump_number_and_text (const char *text, uint32_t num, uint32_t numlen, const char *text2 )
{
  char fmt[20];
  snprintf(fmt, 20," %%%du %%s\n", numlen);
  DBG_do_indent();
  DBG_strpadright(text, myDesiredLen, '.');
  DEBUG_PRINTF(fmt, num, text2 );
}
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

void DBG_dump_uint32_binary (const char *text, uint32_t num )
{
  DBG_do_indent();
  DBG_strpadright(text, myDesiredLen, '.');
  DEBUG_PRINTF(BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN"\n",
                BYTE_TO_BINARY(num>>24),BYTE_TO_BINARY(num>>16),BYTE_TO_BINARY(num>>8),BYTE_TO_BINARY(num) );
}
  
void DBG_dump_uint32_hex (const char *text, uint32_t num )
{
  DBG_do_indent();
  DBG_strpadright(text, myDesiredLen, '.');
  DEBUG_PRINTF(" 0x%08x\n", num );
}

void DBG_dump_uint8_hex (const char *text, uint8_t num )
{
  DBG_do_indent();
  DBG_strpadright(text, myDesiredLen, '.');
  DEBUG_PRINTF(" 0x%02x\n", num );
}


void DBG_dump_uint16_hex (const char *text, uint16_t num )
{
  DBG_do_indent();
  DBG_strpadright(text, myDesiredLen, '.');
  DEBUG_PRINTF(" 0x%04x\n", num );
}

void DBG_dump_uint32_hex_dec (const char *text, uint32_t num )
{
  DBG_do_indent();
  DBG_strpadright(text, myDesiredLen, '.');
  DEBUG_PRINTF(" 0x%08x(%d)\n", num,num );
}

void DBG_dump_uint32_kb (const char *text, uint32_t num )
{
  DBG_do_indent();
  DBG_strpadright(text, myDesiredLen, '.');
  DEBUG_PRINTF(" %dkB\n", num >> 10 );
}

void DBG_dump_section(const char *text, uint32_t start, uint32_t end, bool bIn_KByte )
{
  DBG_do_indent();
  DBG_strpadright(text, myDesiredLen, '.');
  if ( bIn_KByte )
    DEBUG_PRINTF(" 0x%08x...0x%08x, Length=%dk Byte\n",start, end, (end - start)>>10); 
  else
    DEBUG_PRINTF(" 0x%08x...0x%08x, Length=%d Byte\n",start, end, end - start); 
}
void DBG_dump_section_useage (const char *text, uint32_t used, uint32_t total ) 
{
  DBG_do_indent();
  DBG_strpadright(text, myDesiredLen, '.');
  DEBUG_PRINTF(" %d Bytes (%d%%)\n", used, (used*100+(total>>1))/total );
}

void DBG_dump_stack_useage( uint32_t act_sp, uint32_t max_useage, uint32_t start, uint32_t end)
{
  uint32_t total = end - start;

  DBG_do_indent();
  DBG_strpadright("Actual SP", myDesiredLen, '.');
  DEBUG_PRINTF(" 0x%08x\n", act_sp );
  DBG_do_indent();
  DBG_strpadright("Max Stack Depth", myDesiredLen, '.');
  DEBUG_PRINTF(" %d Bytes (%d%%)\n", max_useage, (max_useage*100+(total>>1))/total );
}

 
void DBG_dump_bitvalue(const char *text, uint32_t regval, uint32_t bitval ) 
{
  DBG_do_indent();
  DBG_strpadright(text, myDesiredLen, '.');
  DEBUG_PRINTF(" %s\n", regval & bitval ? "Yes" : "No");
}

void DBG_dump_bitvalue2(const char *text1, const char *text2, uint32_t regval, uint32_t bitval ) 
{
  DBG_do_indent();
  DBG_strpadright2(text1, text2, myDesiredLen, '.');
  DEBUG_PRINTF(" %s\n", regval & bitval ? "Yes" : "No");
}

void DBG_dump_onoffvalue(const char *text, uint32_t regval, uint32_t bitval)
{
  DBG_do_indent();
  DBG_strpadright(text, myDesiredLen, '.');
  DEBUG_PRINTF(" %s\n", regval & bitval ? "On" : "Off");
}

void DBG_dump_onoffvalue2(const char *text, uint32_t regval, uint32_t bitval, const char *append) 
{
  DBG_do_indent();
  DBG_strpadright(text, myDesiredLen, '.');
  DEBUG_PRINTF(" %s%s\n", regval & bitval ? "On " : "Off",append);
}

void DBG_dump_setresetvalue(const char *text, uint32_t regval, uint32_t bitval ) 
{
  DBG_do_indent();
  DBG_strpadright(text, myDesiredLen, '.');
  DEBUG_PRINTF(" %s\n", regval & bitval ? "Set" : "Reset");
}

void DBG_dump_endisvalue(const char *text, uint32_t regval, uint32_t bitval ) 
{
  DBG_do_indent();
  DBG_strpadright(text, myDesiredLen, '.');
  DEBUG_PRINTF(" %sabled\n", regval & bitval ? "En" : "Dis");
}

void DBG_dump_onlyifset(const char *text, uint32_t regval, uint32_t bitval ) 
{
  if ( regval & bitval ) {
      DBG_do_indent();
      DBG_strpadright(text, myDesiredLen, '.');
      DEBUG_PRINTF("Set\n");
  }
}


/*
 * Dump a power-of-2 value
 * @param  exp - exponent of 2
 *
 */
void DBG_dump_po2 (const char *text, uint32_t exp )
{
  DBG_do_indent();
  DBG_strpadright(text, myDesiredLen, '.');
  DEBUG_PRINTF(" %d\n", 1 << exp );
}


#endif // #if DEBUG_FEATURES > 0


void CRLF(void) {
	 DEBUG_PUTC('\r'); DEBUG_PUTC('\n');
}


void print_hexXX(uint8_t i) 
{
  uint8_t x = i>>4;
  if (x>=10) {
    DEBUG_PUTC(x+'a'-10);	
  } else {
    DEBUG_PUTC(x+'0');
  }	
  x = i & 0xf;
  if (x>=10) {
    DEBUG_PUTC(x+'a'-10);	
  } else {
    DEBUG_PUTC(x+'0');
  }	
}

const char * const hal_err_txt[]={"Ok", "Error", "Busy", "Timeout"};
/******************************************************
 * return error text for HAL return status value
 *****************************************************/
/* RHB commented out
const char* DBG_get_hal_errtxt(HAL_StatusTypeDef code )
{
  if ( code < sizeof(hal_err_txt)/sizeof(char *) ) 
    return hal_err_txt[code];
  else
    return "??? ";
}
*/

#if DEBUG_MODE > 0 || DEBUG_MEM_CHECK > 0
  /*!
   *******************************************************************************
   *  \brief helper function print 4 digit dec number
   *
   *  \note only unsigned numbers
   ******************************************************************************/

    void print_hexXXXX(uint16_t i) {
        print_hexXX(i>>8);
        print_hexXX(i&0xff);
    }

#endif

#if ( DEBUG_FEATURES > 0 || DEBUG_RFM_STATUS > 0 || DEBUG_CHECK_SEC > 0 || DEBUG_PRINT_RTC_TICKS > 0 || DEBUG_MEM_CHECK > 0 || DEBUG_SLEEP_MODE > 0 || DEBUG_PRINT_ADDITIONAL_TIMESTAMPS > 0 ) 
        #define MAX_NUMBUF 33
        static char numbuf[MAX_NUMBUF];
        static char *numptr;
        static bool _print_number_int ( uint16_t num, uint8_t digits, bool bLeadingZeros)
        {
                bool ret=false;
                if ( --digits && (num || bLeadingZeros) ) 
                        ret = _print_number_int( num/10, digits, bLeadingZeros);
	
                uint8_t digit= num % 10;
                if ( bLeadingZeros || ret || digit ) {
                        // uart_putc( digit+'0');
                        *numptr++ = '0'+digit;
                        return true;
                } else {
                        return false;
                }
        }


        static void print_str ( char *str ) {
            while ( *str ) {
               DEBUG_PUTC(*str);
               str++;
            }
        }

        void print_dec_number ( uint16_t num, uint8_t digits, bool bLeadingZeros)
        {
                numptr = numbuf;
                if  ( !bLeadingZeros && !num )
                    *numptr++ = '0';
                else
                    (void)_print_number_int ( num, digits, bLeadingZeros);
                *numptr='\0';
                print_str(numbuf);
        }

        char *my_itoa ( uint16_t num, char *retbuf, uint8_t digits, bool bLeadingZeros)
        {
                numptr = retbuf;
                if  ( !bLeadingZeros && !num )
                    *numptr++ = '0';
                else
                    (void)_print_number_int ( num, digits, bLeadingZeros);

                // Terminate string and return pointer to terminating \0
                *numptr='\0';
                return numptr;
        }


	/*!
	 *******************************************************************************
	 *  \brief helper function print 2 digit dec number
	 *
	 *  \note Signed number ( i.e. -128 .. +127 )
	 ******************************************************************************/
	void print_decSXX(int8_t i) {
		if ( i < 0 ) {
			DEBUG_PUTC('-');
			print_decXX( (uint8_t)(i*-1) );
		} else {
			print_decXX( (uint8_t)i );
		}
	}

#endif



#if DEBUG_PRINT_ADDITIONAL_TIMESTAMPS > 0
    void COM_print_time(uint8_t c, uint32_t bWithCRLF) {
        print_dec0XX(RTC_GetSecond());
        DEBUG_PUTC('.');
        print_dec0XX(RTC_GetMillis());
        DEBUG_PUTC('-');
        DEBUG_PUTC(c);
        if (bWithCRLF) 
          DEBUG_PUTS("");
        else
          DEBUG_PUTC(' ');
   }
#endif



/**
  * @}
  */
