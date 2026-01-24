/**
  ******************************************************************************
  * @file    system/ipc.c
  * @author  Rainer
  * @brief   functions and data structures to handle more than on DMA request 
  *          per IRQ. ( PICO SDK limits the DMA-Requests per IRQ to 1 )
  * @Note    in Multicore envoronment, Core0 uses IRQ0 only, CORE1 uses IRQ1 only
  ******************************************************************************
  */

#include "config/config.h"
#include "hardware/irq.h"
#include "hardware/dma.h"

#include "debug/debug_helper.h"

#include "system/dma_irq.h"

/*-----------------------------------------------------------------------------
 * List of user DMA handlers for every channel
 *---------------------------------------------------------------------------*/
typedef void (*User_DMA_Handler) (void);
static irq_handler_t user_dma_handlers[MAX_DMA_CHANNELS] = {0};

// ------ Forward decls -------------------------------------------------------
void DMA_Global_handler ( void );


#if   RP2040_M0_0
  #define IRQ_LINE  DMA_IRQ_0
  #define IRQ_NAME  "DMA_IRQ0"
#elif RP2040_M0_1 || defined(CORE1_SIM)
  #define IRQ_LINE  DMA_IRQ_1
  #define IRQ_NAME  "DMA_IRQ1"
#endif

/* check, whether a dma channel is assigned */
#define is_dma_channel_assigned( ch )   ( user_dma_handlers[ch] != NULL )  

/******************************************************************************
 * returns true, if dma channel "channel" is assigned
 *****************************************************************************/
bool dma_channel_assigned (uint channel)
{
  if ( channel >= MAX_DMA_CHANNELS ) return false;

  return ( user_dma_handlers[channel] != NULL );
}

/******************************************************************************
 * returns true, if NO dma channels is assigned
 *****************************************************************************/
static bool no_channel_assigned ( void )
{
  for ( uint i=0; i < MAX_DMA_CHANNELS; i++ )
    if ( is_dma_channel_assigned(i) ) return false;

  return true;
}


/******************************************************************************
 * DMA user must call this instead of SDK'S irq_set_exclusive_handler or
 * dma_channel_set_irq1_enabled
 * This handler will keep track of all DMA channels, that could claim an 
 * interrupt and call the corresponding handler 
 *****************************************************************************/
void dma_channel_set_handler (uint channel, irq_handler_t handler)
{
  /* if no handlers assigned so far, install the global DMA-IRQ-Handler */
  if ( no_channel_assigned() ) {
    irq_set_exclusive_handler(IRQ_LINE, DMA_Global_handler); 
    irq_set_enabled(IRQ_LINE, true);
  }

  /* register the user dmairq handler */
  if( user_dma_handlers[channel] != NULL ) {
    DEBUG_PRINTF("Ch %d of "IRQ_NAME"already set, will be overwritten!\n", channel);
  }
  user_dma_handlers[channel] = handler;
}

/******************************************************************************
 * The one and only handler for DMA IRQ
 * - check the Pending bits of all enabled channels
 * - if set, activate handler. Handler is responsible to reset pending bit!
 *****************************************************************************/
void DMA_Global_handler ( void )
{
  uint32_t active_irqs;
  uint32_t shift, i;
#if   RP2040_M0_0
  #define ena    (dma_hw->inte0)
  #define pend   (dma_hw->ints0)
#elif RP2040_M0_1 || defined(CORE1_SIM)
  #define ena  (dma_hw->inte1)
  #define pend   (dma_hw->ints1)
#endif

  /* get all pending and enabled DMA request bits */
  active_irqs = ena & pend;

  /* activate handlers for each active request */
  for ( i= 0, shift = 1; i < MAX_DMA_CHANNELS; i++, shift <<= 1 ) {
    if ( active_irqs & shift && user_dma_handlers[i] )
      user_dma_handlers[i]();
  }
}



    
  

