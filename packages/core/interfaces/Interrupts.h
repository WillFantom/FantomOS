#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include <core/debug/Common.h>
#include <core/interfaces/Memory.h>

//Optional Interrupt Handler Adding
#define IDT_ENTRIES 256
void (*handlers[IDT_ENTRIES])(Registers *regs);

//Optional Handler Adders
void (*Interrupts_addISR)(uint8_t isr, void* addr);
void (*Interrupts_addIRQ)(uint8_t irq, void* addr);


#endif
