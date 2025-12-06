/**
  ******************************************************************************
  * @file    debug_util.c
  * @author  Rainer
  * @brief   functions for debugging/dumping GPIO and EXTI Settings
  *
  ******************************************************************************
  *
  ******************************************************************************
  */

/** @addtogroup DEBUG_UTILS
  * @{
  */

#include "config/config.h"

#if DEBUG_FEATURES > 0

#include <string.h>

#include "debug/debug_helper.h"

/*
 *************************************************************
 * defines
 *************************************************************
 */

/*
 *************************************************************
 * local helper functions 
 *************************************************************
 */


/*
 *************************************************************
 * Functions to dump GPIO status
 *************************************************************
 */


const char * const moder_txt[]={"Inp ", "Out ", "AF  ", "Ana ", "ADC " };
static const char* DBG_get_moder_txt(uint32_t sel, uint16_t ascr )
{
  // If Analog mode and connected to ADC, then annotate accordingly
  if ( sel == 0b11 && ascr ) sel = 4;
  if ( sel < sizeof(moder_txt)/sizeof(char *) ) 
    return moder_txt[sel];
  else
    return "??? ";
}

const char * const pupdr_txt[]={"--  ", "PU  ", "PD  ", "Res " };
static const char* DBG_get_pupdr_txt(uint32_t sel)
{
  if ( sel < sizeof(pupdr_txt)/sizeof(char *) ) 
    return pupdr_txt[sel];
  else
    return "??? ";
}

const char * const ospeedr_txt[]={"Low ", "Med ", "Hi  ", "Vhi " };
static const char* DBG_get_ospeedr_txt(uint32_t sel)
{
  if ( sel < sizeof(ospeedr_txt)/sizeof(char *) ) 
    return ospeedr_txt[sel];
  else
    return "??? ";
}



static void DBG_gpio_dump_moder ( uint32_t moder, uint32_t ascr )
{
  const uint32_t moder_mask = 0xC0000000;
  const uint16_t ascr_mask  = 0x8000;
  uint32_t cnt;
  for ( cnt=0; cnt < 16; cnt++ ) {
    DEBUG_PRINTF("%s",DBG_get_moder_txt((moder & moder_mask) >> 30, ( ascr &  ascr_mask ) >> 15 ));
    moder <<= 2;
    ascr <<= 1;
  }
}

static void DBG_gpio_dump_ospeedr ( uint32_t val )
{
  const uint32_t mask = 0xC0000000;
  uint32_t cnt;
  for ( cnt=0; cnt < 16; cnt++ ) {
    DEBUG_PRINTF("%s",DBG_get_ospeedr_txt((val & mask) >> 30 ));
    val <<= 2;
  }
}

static void DBG_gpio_dump_pupdr ( uint32_t val )
{
  const uint32_t mask = 0xC0000000;
  uint32_t cnt;
  for ( cnt=0; cnt < 16; cnt++ ) {
    DEBUG_PRINTF("%s",DBG_get_pupdr_txt((val & mask) >> 30 ));
    val <<= 2;
  }
}

static void DBG_gpio_dump_afr ( uint32_t val )
{
  const uint32_t mask = 0xF0000000;
  uint32_t cnt;
  uint32_t current;
  for ( cnt=0; cnt  < 8; cnt++ ) {
    current = (val & mask) >> 28;
    print_dec_number ( current, 2, true);DEBUG_PRINTF("  ");
    val <<= 4;
  }
}

static void DBG_gpio_dump_otyper ( uint32_t val )
{
  uint32_t position = 0x8000;
  uint32_t cnt;

  for ( cnt = 0; cnt <16; cnt++ ) {
    DEBUG_PRINTF( position & val ? "OD  " : "PP  " );
    position >>= 1;
  }
}

static void DBG_gpio_dump_bitwise ( uint32_t val )
{
  uint32_t position = 0x8000;
  uint32_t cnt;

  for ( cnt = 0; cnt <16; cnt++ ) {
    DEBUG_PRINTF( position & val ? "1   " : "0   " );
    position >>= 1;
  }
}

static void DBG_gpio_dump_bitnumbers ( char gpio_letter )
{
  int8_t cnt;

  for ( cnt = 15; cnt >= 0; cnt-- ) {
    DEBUG_PUTC ( gpio_letter );
    print_dec_number ( cnt, 2, true);
    DEBUG_PUTC(' ');
  }
}



void DBG_dump_gpio_status(char gpio_letter )
{
  /* RHB todo
    bool gp_clck_ena; 
    

    DEBUG_PRINTF  ("Status of GPIO%c----------------------------------------------------------\n",gpio_letter);
    if ( !gp ) {
      DEBUG_PUTS  ("  ??? undefined ???");
      return;
    }

    gp_clck_ena = HW_GetHWClockStatus ( gp );
    if ( ! gp_clck_ena ) {
      DEBUG_PUTS  ("  Not clocked ");
      return;
    }

    int oldIndent = DBG_setIndentRel(+2);

    DEBUG_PRINTF("        ");DBG_gpio_dump_bitnumbers(gpio_letter);DEBUG_PUTC('\n');
    DEBUG_PRINTF("MODER  :");DBG_gpio_dump_moder(
                gp->MODER, 
            #if defined(STM32476xx)
                gp->ASCR);
            #else
                0);
            #endif 
    DEBUG_PUTC('\n');
    DEBUG_PRINTF("OTYPER :");DBG_gpio_dump_otyper(gp->OTYPER);DEBUG_PUTC('\n');
    DEBUG_PRINTF("OSPEEDR:");DBG_gpio_dump_ospeedr(gp->OSPEEDR);DEBUG_PUTC('\n');
    DEBUG_PRINTF("OPUPDR :");DBG_gpio_dump_pupdr(gp->PUPDR);DEBUG_PUTC('\n');
    DEBUG_PRINTF("AFR    :");DBG_gpio_dump_afr(gp->AFR[1]);DBG_gpio_dump_afr(gp->AFR[0]);DEBUG_PUTC('\n');
    DEBUG_PRINTF("IDR    :");DBG_gpio_dump_bitwise(gp->IDR);DEBUG_PUTC('\n');
    DEBUG_PRINTF("ODR    :");DBG_gpio_dump_bitwise(gp->ODR);DEBUG_PUTC('\n');
  
    DBG_setIndentAbs(oldIndent);
    ---*/
}



#if EXTI_EXI
/*
 *************************************************************
 * Functions to dump EXTI settings
 *************************************************************
 */
#define EXTI_NAMELEN          12

const char exti_gpio_name[] = "GPIO";

#if defined(STM32L4_FAMILY)
    #if defined(STM32L476xx) || defined(STM32L496xx)
        const char * const exti_line_name[]= { 
        /* insert pattern ( max length is 12 ) 
          "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", */
          "PVD",          "OTG FS wkup",  "RTC alarms",   "RTC tamper  ", "RTC wkup tmr", "COMP1 output", "COMP2 output", "I2C1 wkup",
          "I2C2 wkup",    "I231 wkup",    "USART1 wkup",  "USART2 wkup",  "USART3 wkup",  "UART4 wkup",   "UART5 wkup",   "LPUART1 wkup",
          "LPTIM1   ",    "LPTIM2   ",    "SWPMI1 wkup",  "PVM1 wkup",    "PVM2 wkup",    "PVM3 wkup",    "PVM4 wkup",    "LCD wkup",
          "I2C4 wkup",
        };
    #elif defined(STM32L43xx) 
        const char * const exti_line_name[]= { 
        /* insert pattern ( max length is 12 ) 
          "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", */
          "PVD",          "???",          "RTC alarms",   "RTC tamper  ", "RTC wkup tmr", "COMP1 output", "COMP2 output", "I2C1 wkup",
          "I2C2 wkup",    "I231 wkup",    "USART1 wkup",  "USART2 wkup",  "USART3 wkup",  "??? ",         "???",          "LPUART1 wkup",
          "LPTIM1   ",    "LPTIM2   ",    "SWPMI1 wkup",  "PVM1 wkup",    "???",          "PVM3 wkup",    "PVM4 wkup",    "???",
          "???",
        };
    #elif defined(STM32L4PLUS_FAMILY)
        const char * const exti_line_name[]= { 
        /* insert pattern ( max length is 12 ) 
          "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", */
          "PVD",          "OTG FS wkup",  "RTC alarms",   "RTC tamper  ", "RTC wkup tmr", "COMP1 output", "COMP2 output", "I2C1 wkup",
          "I2C2 wkup",    "I2C3 wkup",    "USART1 wkup",  "USART2 wkup",  "USART3 wkup",  "UART4 wkup",   "UART5 wkup",   "LPUART1 wkup",
          "LPTIM1   ",    "LPTIM2   ",    "Reserved   ",  "PVM1 wkup",    "PVM2 wkup",    "PVM3 wkup",    "PVM4 wkup",    "Reserved",
          "I2C4 wkup",
        };
    #else
        #error "missing EXTI-line names"
    #endif
const char * const exti_domain_name[]= { "EXTI   " };
#define EXTI_MAXNUM             40
#define EXTI_MAXDOMAIN          1
#define EXTI1_IS_GPIO           0b00000000000000001111111111111111
#define EXTI2_IS_GPIO           0b00000000
#define EXTI1_IS_CONFIGURABLE   0b00000000011111011111111111111111
#define EXTI2_IS_CONFIGURABLE   0b01111000
#define EXTI_IS_GPIO(i)         ( i < 16 )
#define EXTI_IS_CFGABLE(i)      ( i > 31 ? EXTI2_IS_CONFIGURABLE : EXTI1_IS_CONFIGURABLE )


#define GET_IMR(d,i)  ( i > 31 ? EXTI->IMR2  : EXTI->IMR1 )
#define GET_EMR(d,i)  ( i > 31 ? EXTI->EMR2  : EXTI->EMR1 )
#define GET_PR(d,i)   ( i > 31 ? EXTI->PR2   : EXTI->PR1 )

#define GET_FTSR(i)   ( i > 31 ? EXTI->FTSR2 : EXTI->FTSR1 )
#define GET_RTSR(i)   ( i > 31 ? EXTI->RTSR2 : EXTI->RTSR1 )

#elif defined(STM32H747xx) || defined(STM32H745xx)
const char * const exti_line_name[]= { 
/* insert pattern ( max length is 12 ) 
  "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", */
  "PVD/AVD",      "RTC alarms",   "RTC tamper  ", "RTC wkup tmr", "COMP1 output", "COMP2 output", "I2C1 wkup",    "I2C2 wkup",    
  "I2C3 wkup",    "I2C4 wkup",    "USART1 wkup",  "USART2 wkup",  "USART3 wkup",  "USART3 wkup",  "UART4 wkup",   "UART5 wkup",   
  "UART7 wkup",   "UART8 wkup",   "LPUART1 rx",   "LPUART1 tx",   "SPI1 wkup",    "SPI2 wkup",    "SPI3 wkup",    "SPI4 wkup",
  "SPI7 wkup",    "SPI6 wkup",    "MDIO wkup",    "USB1 wkup",    "USB2 wkup",    "resvd",        "DSI wkup",     "LPTIM1 wkup",    
  "LPTIM2 wkup",  "LPTIM2 outp",  "LPTIM3 wkup",  "LPTIM3 outp",  "LPTIM4 wkup",  "LPTIM5 wkup",  "SWPMI wkup",   "WKUP0 pin", 
  "WKUP1 pin",    "WKUP2 pin",    "WKUP3 pin",    "WKUP4 pin",    "WKUP5 pin",    "RCC Interr.",  "I2C4 Event",   "I2C4 Error",     
  "LPUART1 wkup", "SPI6 Interr.", "BDMA CH0 Int", "BDMA CH1 Int", "BDMA CH2 Int", "BDMA CH3 Int", "BDMA CH4 Int", "BDMA CH5 Int", 
  "BDMA CH6 Int", "BDMA CH7 Int", "DMAMUX2 Int",  "ADC3 Int",     "SAI4 Int",     "HSEM0 Int",    "HSEM1 Int",    "M4 SEV Int",
  "M7 SEV Int",   "resvd",        "WWDG1 reset",  "resvd",        "WWDG2 reset",  "HDMICEC wkup", "ETHER wkup",   "HSECSS wkup",
  "resvd", 
};
const char * const exti_domain_name[]= { "EXTI_D1", "EXTI_D2", "EXTI_D3" };
#define EXTI_MAXNUM             88
#define EXTI_MAXDOMAIN          3

#define EXTI1_IS_CONFIGURABLE   0b00000000001111111111111111111111
#define EXTI2_IS_CONFIGURABLE   0b00000000000010100000000000000000 
#define EXTI3_IS_CONFIGURABLE   0b00000000011101000000000000000000 
#define EXTI_IS_GPIO(i)         ( i < 16 )
#define EXTI_IS_CFGABLE2(i)     ( i > 63 ? EXTI3_IS_CONFIGURABLE : EXTI2_IS_CONFIGURABLE )
#define EXTI_IS_CFGABLE(i)      ( i > 31 ? EXTI_IS_CFGABLE2(i)   : EXTI1_IS_CONFIGURABLE )

#define GET_INSTNAME(inst)  ( inst > 1 ? EXTI_D2 : EXTI_D1 )
#define GET_IMR(inst,num)   ( num > 31 ? num > 63 ? GET_INSTNAME(inst)->IMR3 : GET_INSTNAME(inst)->IMR2  : GET_INSTNAME(inst)->IMR1 )
#define GET_EMR(inst,num)   ( num > 31 ? num > 63 ? GET_INSTNAME(inst)->EMR3 : GET_INSTNAME(inst)->EMR2  : GET_INSTNAME(inst)->EMR1 )
#define GET_PR(inst,num)    ( num > 31 ? num > 63 ? GET_INSTNAME(inst)->PR3  : GET_INSTNAME(inst)->PR2   : GET_INSTNAME(inst)->PR1  )

#define GET_FTSR(i)  ( i > 31 ? i > 63 ? EXTI->FTSR3 : EXTI->FTSR2 : EXTI->FTSR1 )
#define GET_RTSR(i)  ( i > 31 ? i > 63 ? EXTI->RTSR3 : EXTI->RTSR2 : EXTI->RTSR1 )

#elif defined(STM32H742xx) || defined(STM32H743xx)
const char * const exti_line_name[]= { 
/* insert pattern ( max length is 12 ) 
  "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", */
  "PVD/AVD",      "RTC alarms",   "RTC tamper  ", "RTC wkup tmr", "COMP1 output", "COMP2 output", "I2C1 wkup",    "I2C2 wkup",    
  "I2C3 wkup",    "I2C4 wkup",    "USART1 wkup",  "USART2 wkup",  "USART3 wkup",  "USART3 wkup",  "UART4 wkup",   "UART5 wkup",   
  "UART7 wkup",   "UART8 wkup",   "LPUART1 rx",   "LPUART1 tx",   "SPI1 wkup",    "SPI2 wkup",    "SPI3 wkup",    "SPI4 wkup",
  "SPI5 wkup",    "SPI6 wkup",    "MDIO wkup",    "USB1 wkup",    "USB2 wkup",    "resvd",        "resvd",        "LPTIM1 wkup",    
  "LPTIM2 wkup",  "LPTIM2 outp",  "LPTIM3 wkup",  "LPTIM3 outp",  "LPTIM4 wkup",  "LPTIM5 wkup",  "SWPMI wkup",   "WKUP0 pin", 
  "WKUP1 pin",    "WKUP2 pin",    "WKUP3 pin",    "WKUP4 pin",    "WKUP5 pin",    "RCC Interr.",  "I2C4 Event",   "I2C4 Error",     
  "LPUART1 wkup", "SPI6 Interr.", "BDMA CH0 Int", "BDMA CH1 Int", "BDMA CH2 Int", "BDMA CH3 Int", "BDMA CH4 Int", "BDMA CH5 Int", 
  "BDMA CH6 Int", "BDMA CH7 Int", "DMAMUX2 Int",  "ADC3 Int",     "SAI4 Int",     "resvd",        "resvd",        "resvd",
  "resvd",        "resvd",        "resvd",        "resvd",        "resvd",        "HDMICEC wkup", "ETHER wkup",   "HSECSS wkup",
  "resvd", 
};
const char * const exti_domain_name[]= { "EXTI   " };
#define EXTI_MAXNUM             88
#define EXTI_MAXDOMAIN          1

#define EXTI1_IS_CONFIGURABLE   0b00000000001111111111111111111111
#define EXTI2_IS_CONFIGURABLE   0b00000000000010100000000000000000 
#define EXTI3_IS_CONFIGURABLE   0b00000000011000000000000000000000 
#define EXTI_IS_GPIO(i)         ( i < 16 )
#define EXTI_IS_CFGABLE2(i)     ( i > 63 ? EXTI3_IS_CONFIGURABLE : EXTI2_IS_CONFIGURABLE )
#define EXTI_IS_CFGABLE(i)      ( i > 31 ? EXTI_IS_CFGABLE2(i)   : EXTI1_IS_CONFIGURABLE )

#define GET_INSTNAME(inst)  EXTI
#define GET_IMR(inst,num)   ( num > 31 ? num > 63 ? GET_INSTNAME(inst)->IMR3 : GET_INSTNAME(inst)->IMR2  : GET_INSTNAME(inst)->IMR1 )
#define GET_EMR(inst,num)   ( num > 31 ? num > 63 ? GET_INSTNAME(inst)->EMR3 : GET_INSTNAME(inst)->EMR2  : GET_INSTNAME(inst)->EMR1 )
#define GET_PR(inst,num)    ( num > 31 ? num > 63 ? GET_INSTNAME(inst)->PR3  : GET_INSTNAME(inst)->PR2   : GET_INSTNAME(inst)->PR1  )

#define GET_FTSR(i)  ( i > 31 ? i > 63 ? EXTI->FTSR3 : EXTI->FTSR2 : EXTI->FTSR1 )
#define GET_RTSR(i)  ( i > 31 ? i > 63 ? EXTI->RTSR3 : EXTI->RTSR2 : EXTI->RTSR1 )

#elif defined(STM32H723xx) || defined(STM32H733xx) || defined(STM32H725xx) || defined(STM32H735xx) || defined(STM32H730xx)
const char * const exti_line_name[]= { 
/* insert pattern ( max length is 12 ) 
  "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", "LPUART1 wkup", */
  "PVD/AVD",      "RTC alarms",   "RTC tamper  ", "RTC wkup tmr", "COMP1 output", "COMP2 output", "I2C1 wkup",    "I2C2 wkup",    
  "I2C3 wkup",    "I2C4 wkup",    "USART1 wkup",  "USART2 wkup",  "USART3 wkup",  "USART3 wkup",  "UART4 wkup",   "UART5 wkup",   
  "UART7 wkup",   "UART8 wkup",   "LPUART1 rx",   "LPUART1 tx",   "SPI1 wkup",    "SPI2 wkup",    "SPI3 wkup",    "SPI4 wkup",
  "SPI5 wkup",    "SPI6 wkup",    "MDIO wkup",    "USB1 wkup",    "resvd",        "resvd",        "resvd",        "LPTIM1 wkup",    
  "LPTIM2 wkup",  "LPTIM2 outp",  "LPTIM3 wkup",  "LPTIM3 outp",  "LPTIM4 wkup",  "LPTIM5 wkup",  "SWPMI wkup",   "WKUP0 pin", 
  "WKUP1 pin",    "resvd",        "WKUP3 pin",    "resvd",        "WKUP5 pin",    "RCC Interr.",  "I2C4 Event",   "I2C4 Error",     
  "LPUART1 wkup", "SPI6 Interr.", "BDMA CH0 Int", "BDMA CH1 Int", "BDMA CH2 Int", "BDMA CH3 Int", "BDMA CH4 Int", "BDMA CH5 Int", 
  "BDMA CH6 Int", "BDMA CH7 Int", "DMAMUX2 Int",  "ADC3 Int",     "SAI4 Int",     "HSEM0",        "resvd",        "resvd",
  "resvd",        "resvd",        "resvd",        "resvd",        "resvd",        "HDMICEC wkup", "ETHER wkup",   "HSECSS wkup",
  "TEMP",         "UART9 wkup",   "USART10 wkup", "I2C5 wkup",
};
const char * const exti_domain_name[]= { "EXTI   " };
#define EXTI_MAXNUM             91
#define EXTI_MAXDOMAIN          1

#define EXTI1_IS_CONFIGURABLE   0b00000000001111111111111111111111
#define EXTI2_IS_CONFIGURABLE   0b00000000000010100000000000000000 
#define EXTI3_IS_CONFIGURABLE   0b00000000011000000000000000000000 
#define EXTI_IS_GPIO(i)         ( i < 16 )
#define EXTI_IS_CFGABLE2(i)     ( i > 63 ? EXTI3_IS_CONFIGURABLE : EXTI2_IS_CONFIGURABLE )
#define EXTI_IS_CFGABLE(i)      ( i > 31 ? EXTI_IS_CFGABLE2(i)   : EXTI1_IS_CONFIGURABLE )

#define GET_INSTNAME(inst)  EXTI
#define GET_IMR(inst,num)   ( num > 31 ? num > 63 ? GET_INSTNAME(inst)->IMR3 : GET_INSTNAME(inst)->IMR2  : GET_INSTNAME(inst)->IMR1 )
#define GET_EMR(inst,num)   ( num > 31 ? num > 63 ? GET_INSTNAME(inst)->EMR3 : GET_INSTNAME(inst)->EMR2  : GET_INSTNAME(inst)->EMR1 )
#define GET_PR(inst,num)    ( num > 31 ? num > 63 ? GET_INSTNAME(inst)->PR3  : GET_INSTNAME(inst)->PR2   : GET_INSTNAME(inst)->PR1  )

#define GET_FTSR(i)  ( i > 31 ? i > 63 ? EXTI->FTSR3 : EXTI->FTSR2 : EXTI->FTSR1 )
#define GET_RTSR(i)  ( i > 31 ? i > 63 ? EXTI->RTSR3 : EXTI->RTSR2 : EXTI->RTSR1 )

#else
    #error "No EXTI Line definitions for current MCU"
    const char * const exti_line_name[]= {};
#endif
/*
 *************************************************************
 * @brief Fill in the EXTI Line name
 * @param sel    - exti line number
 * @param retbuf - return buffer to be written to
 * @param maxlen - length of retbuf on retrun, right padded with blanks
 * @note retbuf must provide at least maxlen+1 bytes !!!
 *************************************************************
 */
static void DBG_get_exti_line_name(uint32_t sel, char *retbuf, const uint32_t maxlen)
{
  uint32_t cptr  = 0;
  const char *p;

  memset(retbuf, ' ', maxlen );
  if ( sel < 16 ) {
    /* assemble constant part plus number ( 2 digits ) */
    cptr = strlen(exti_gpio_name);
    strncpy(retbuf, exti_gpio_name,cptr );
    retbuf[cptr++] = '0' + sel/10;
    retbuf[cptr++] = '0' + sel % 10;
  } else {
    /* Copy namr from array */
    sel -= 16;
    if ( sel < sizeof(exti_line_name)/sizeof(char *) ) {
      p = exti_line_name[sel];
      strncpy(retbuf, p, strlen(p));
    }
  }

  /* append terminating \0 */
  retbuf[maxlen] = '\0';
}


static char DBG_get_exti_pinsource(uint32_t pinidx )
{
  /* one exti config register in SYSCFG->EXTICRx entry is 4 bit wide */
  const uint32_t exticr_mask = 0b1111;
  
  /* One register holds the configuration for four lines */
  uint32_t exticr_offset = pinidx >> 2;

  /* compute the amount of bits to shift mask and shift back result bits */
  uint32_t exticr_shift  = ( pinidx & 0b11 ) * 4;
  
  /* Get the relevant four bits right aligned */
  uint32_t exticr_bits = ( SYSCFG->EXTICR[exticr_offset] & ( exticr_mask << exticr_shift ) ) >> exticr_shift;
  if ( exticr_bits > 8 ) 
    return '?';
  else
    return 'A' + exticr_bits;
}



static void DBG_n_spaces(uint32_t num )
{
  while ( num ) {
    DEBUG_PUTC(' ');
    num--;
  }
}

static void DBG_dump_one_exti(uint32_t domain, uint32_t idx)
{
  char namebuf[EXTI_NAMELEN+1]; 
  uint32_t mask = 1 << ( idx & 0x1f );

  UNUSED(domain);
  DBG_get_exti_line_name(idx, namebuf, EXTI_NAMELEN);
  DEBUG_PRINTF("%s",namebuf);
  if ( ( GET_IMR(domain,idx) | GET_EMR(domain,idx) )  & mask ) {
    DEBUG_PRINTF(" %s %s", GET_IMR(domain,idx) & mask ? "I" : " ", GET_EMR(domain,idx) & mask ? "E" : " ");
    DEBUG_PRINTF(" %s", GET_PR(domain,idx) & mask ? "P" : " ");
    DEBUG_PRINTF(" %s", Exti_Has_Callback(idx) ? "Cb" : "  ");
    if ( mask & EXTI_IS_CFGABLE(idx) ) {
      DEBUG_PRINTF(" %s %s", GET_RTSR(idx) & mask ? "Rise" : "    ", GET_FTSR(idx) & mask ? "Fall" : "    " );
    } else {
        DBG_n_spaces(10);
    }

    if ( EXTI_IS_GPIO(idx) ) {
        DEBUG_PRINTF(" P%c",DBG_get_exti_pinsource(idx));
        print_dec_number ( idx, 2, true);
    } else {
        DBG_n_spaces(5);
    }
  } else {
    DEBUG_PRINTF(" -----"); DBG_n_spaces(18);

  }
}
/******************************************************************************
 * STM32L4xx : Domain  = 0
 * STM32H7xx : Domain  = 0 .. 2 ( D1, D2 and D3 domains )
 *****************************************************************************/
void DBG_dump_exti_config(uint32_t exti_domain)
{
    uint32_t i;
    
    if ( exti_domain >= EXTI_MAXDOMAIN ) return;

    DEBUG_PRINTF("%s Settings ---------------------------------------------------------\n", exti_domain_name[exti_domain]);
    int oldIndent = DBG_setIndentRel(+2);

    for ( i = 0; i <= EXTI_MAXNUM/2; i++ )
    {
      DBG_dump_one_exti(exti_domain,i); DEBUG_PRINTF("  |  " );
      if ( i+EXTI_MAXNUM/2+1 <= EXTI_MAXNUM ) DBG_dump_one_exti(exti_domain, i+EXTI_MAXNUM/2+1); 
      DEBUG_PRINTF("\n" );
    }
 
    DBG_setIndentAbs(oldIndent);

}

#endif /* EXTI_EXI__

/*
 *************************************************************
 * Functions to dump NVIC settings
 *************************************************************
 */
 #include "hardware/structs/nvic.h"

#define NVIC_NAME_LEN             15
const char * const nvic_fmt_undef= "IRQ#%d";
const char * const sys_nvic_name[]= { 
/*.             .             .             .             .             .             .             . */
  "",           "",           "NMI",        "HardFault",  "MemMgmt",    "BusFault",   "UsageFault", "", 
  "",           "",           "",           "SVCall",     "DebugMon",   "",           "PendSV",     "SysTick"
};
#define MAX_IRQNUM 26
const char * const user_nvic_name[MAX_IRQNUM]= { 
    /*.                 .                 .                 .                 .                 .                 .                 . */
      "TIMER_IRQ_0",    "TIMER_IRQ_1",    "TIMER_IRQ_2",    "TIMER_IRQ_3",    "PWM_IRQ_WRAP",   "USBCTRL_IRQ",    
      "XIP_IRQ",        "PIO0_IRQ_0",     "PIO0_IRQ_1",     "PIO1_IRQ_0",     "PIO1_IRQ_1",     "DMA_IRQ_0",
      "DMA_IRQ_1",      "IO_IRQ_BANK0",   "IO_IRQ_QSPI",    "SIO_IRQ_PROC0",  "SIO_IRQ_PROC1",  "CLOCKS_IRQ",
      "SPI0_IRQ",       "SPI1_IRQ",       "UART0_IRQ",      "UART1_IRQ",      "ADC_IRQ_FIFO",   "I2C0_IRQ",  
      "I2C1_IRQ",       "RTC_IRQ",        
    };

/*
 *************************************************************
 * @brief Fill in the NVIC IRQ name
 * @param sel    - NVIC irq number
 * @param retbuf - return buffer to be written to
 * @param maxlen - length of retbuf on retrun, right padded with blanks
 * @note retbuf must provide at least maxlen+1 bytes !!!
 *************************************************************
 */
static void DBG_get_nvic_irq_name(int32_t irqnum, char *retbuf, const uint32_t maxlen)
{
  const char *p;

  memset(retbuf, ' ', maxlen );
  if ( irqnum < 0 ) {
    p = sys_nvic_name[16+irqnum];
    /* Copy Name to return buffer */
    strncpy(retbuf, p, strlen(p) );
  } else {
      if ( irqnum < (int32_t)(sizeof(user_nvic_name)/sizeof(char *)) ) {
        p = user_nvic_name[irqnum];
        /* Copy Name to return buffer */
        strncpy(retbuf, p, strlen(p) );
      } else {
        /* write directly to return buffer */
        snprintf(retbuf, maxlen, nvic_fmt_undef, irqnum); 
        /* and overwrite the terminating \0 with blank to adjust length of string */
        retbuf[strlen(retbuf)] = ' ' ;
      }
  }
  /* append terminating \0 in any case */
  retbuf[maxlen] = '\0';
}

static uint32_t NVIC_Is_IRQ_enabled(int32_t irqnum)
{
  if ( irqnum < 0 ) return 1;
  if ( irqnum >= MAX_IRQNUM ) return 0;
  return  nvic_hw->iser & ( 1 << irqnum ) ? 1 : 0; 
}

static int32_t NVIC_Get_IRQ_prio(int32_t irqnum)
{
  /* 
   * each interrupt has assigned one byte in ipr[8]
   * only the 2 MSB define the interupt prio [0..3]
   * 0 - highest .. 3 - lowest
   */
  uint8_t shift;
  uint8_t ofs;
  const uint32_t mask = 0b11;
  if ( irqnum < 0 ) return 1;
  if ( irqnum >= MAX_IRQNUM ) return 999;

  /* Determine register offset and shift for currrent irqnum */
  ofs   = (uint8_t)(irqnum / 4);
  shift = (uint8_t)((irqnum & 0b11) * 8 + 6);
  uint32_t iprword = nvic_hw->ipr[ofs];
  
  return ( iprword & ( mask << shift )) >> shift; 
}


static void DBG_dump_one_nvic_entry(int32_t irqnum)
{
  char namebuf[NVIC_NAME_LEN+1];
  uint32_t uPriority;
  char enabled;

  enabled = NVIC_Is_IRQ_enabled(irqnum) ? 'E' : '-';

  DBG_get_nvic_irq_name( irqnum, namebuf, NVIC_NAME_LEN);
  uPriority = NVIC_Get_IRQ_prio( irqnum );
  DBG_printf_indent("%s %c %2d", namebuf, enabled, uPriority );
}


void DBG_dump_nvic_config(void)
{ 
    int oldIndent, max_ints;
    max_ints = MAX_IRQNUM;


    DEBUG_PUTS  ("NVIC Settings -------------------------------------------------------------" );
    oldIndent = DBG_setIndentRel(+2);
 
    // RHB todo -----------------------   
    #if 0
    DBG_printf_indent("CortexM Interrupts ------------------------------------------------------\n" );
    DBG_setIndentRel(+2);
    DBG_dump_one_nvic_entry(NonMaskableInt_IRQn, prigroup );                /*! Cortex-M4 Non Maskable Interrupt         */
    DEBUG_PUTC('\n');
    DBG_dump_one_nvic_entry(HardFault_IRQn, prigroup);                      /*! Cortex-M4 Hard Fault Interrupt           */
    DEBUG_PUTC('\n');
    DBG_dump_one_nvic_entry(MemoryManagement_IRQn, prigroup);               /*! Cortex-M4 Memory Management Interrupt    */
    DEBUG_PUTC('\n');
    DBG_dump_one_nvic_entry(BusFault_IRQn, prigroup);                       /*! Cortex-M4 Bus Fault Interrupt            */
    DEBUG_PUTC('\n');
    DBG_dump_one_nvic_entry(UsageFault_IRQn, prigroup);                     /*! Cortex-M4 Usage Fault Interrupt          */
    DEBUG_PUTC('\n');
    DBG_dump_one_nvic_entry(SVCall_IRQn, prigroup);                         /*! Cortex-M4 SV Call Interrupt              */
    DEBUG_PUTC('\n');
    DBG_dump_one_nvic_entry(DebugMonitor_IRQn, prigroup);                   /*! Cortex-M4 Debug Monitor Interrupt        */
    DEBUG_PUTC('\n');
    DBG_dump_one_nvic_entry(PendSV_IRQn, prigroup);                         /*! Cortex-M4 Pend SV Interrupt              */
    DEBUG_PUTC('\n');
    DBG_dump_one_nvic_entry(SysTick_IRQn, prigroup);                        /*! Cortex-M4 System Tick Interrupt          */
    DEBUG_PUTC('\n');
    DBG_setIndentRel(-2);
    /* --------------------------------- End todo */
    #endif

    DBG_printf_indent("Configurable Interrupts -------------------------------------------------\n" );
    DBG_dump_number("Max Nr of Interrupts :", max_ints );
    DBG_setIndentRel(+2);
    for ( uint32_t i = 0; i <= (uint32_t)max_ints ; i++ ) {
      if ( NVIC_Is_IRQ_enabled(i) ) {
        DBG_dump_one_nvic_entry(i); 
        DEBUG_PUTC('\n');
      }
    }
    DBG_setIndentRel(-2);
 
 
    DBG_setIndentAbs(oldIndent);
}


#endif // #if DEBUG_FEATURES > 0

/**
  * @}
  */
