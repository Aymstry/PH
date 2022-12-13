#ifndef __funcines_swi__
#define __funcines_swi__

#include <LPC210x.H> 
#include <inttypes.h>

uint32_t __swi(2) read_IRQ_bit (void);
uint32_t __swi(3) read_FIQ_bit (void);
void __swi(0xFF) enable_irq (void);
void __swi(0xFE) disable_irq (void);
void __swi(0xFD) enable_irq_fiq (void);
void __swi(0xFC) disable_irq_fiq (void);

#endif