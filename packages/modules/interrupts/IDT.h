#ifndef __IDT_H
#define __IDT_H

#include <core/debug/Common.h>
#include <core/interfaces/Memory.h>
#include <core/interfaces/Interrupts.h>

#define IRQ0 32

#define IDT_ACCESS 0x8E

struct IDT_entry
{
    unsigned short l_off;
    unsigned short sel;
    unsigned char justZero;
    unsigned char flags;
    unsigned short u_off;
} __attribute__((packed));

struct IDT_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct IDT_ptr idt_p;
struct IDT_entry idt[IDT_ENTRIES];
//void (*handlers[IDT_ENTRIES])(Registers *regs);

//Init
bool IDT_init();

//Handler Adder
void IDT_addISR(uint8_t isr, void* addr);
void IDT_addIRQ(uint8_t irq, void* addr);

//ASM setup
extern void idt_setup(unsigned long *idt);

//TODO : find a way to shorten this
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr128();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif
