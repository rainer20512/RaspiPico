/**
  ******************************************************************************
  * @file    system/dma_irq.h
  * @author  Rainer
  * @brief   functions and data structures to handle more than on DMA request 
  *          per IRQ. ( PICO SDK limits the DMA-Requests per IRQ to 1 )
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DMA_IRQ_H_
#define __DMA_IRQ_H_

#include "config/config.h"
#include "hardware/irq.h"

#define MAX_DMA_CHANNELS      16

#if   RP2040_M0_0
  // Tell the DMA to raise IRQ line 0 when done
  #define dma_channel_irq_enable(chan, ena)     dma_channel_set_irq0_enabled(chan, ena)
  #define dma_channel_acknowledge_irq(chan)     dma_channel_acknowledge_irq0 (chan)
  #define dma_channel_get_irq_status(chan)      dma_channel_get_irq0_status(chan)

#elif RP2040_M0_1 || defined(CORE1_SIM)
  // Tell the DMA to raise IRQ line 1 when done
  #define dma_channel_irq_enable(chan, ena)     dma_channel_set_irq1_enabled(chan, ena)
  #define dma_channel_acknowledge_irq(chan)     dma_channel_acknowledge_irq1 (chan)
  #define dma_channel_get_irq_status(chan)      dma_channel_get_irq1_status(chan)
#endif

bool dma_channel_assigned       (uint channel);
void dma_channel_set_handler    (uint channel, irq_handler_t handler);


#ifdef __cplusplus
 extern "C" {
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DMA_IRQ_H_ */
