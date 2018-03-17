/*  - IDT.c
    AUTHOR: Will Fantom
    DESC:   Sets up interrupt handling for an x86 architecture.
            Allows for adding IRQs and for passing ISRs to the fault handler.
*/

#include <modules/interrupts/IDT.h>
#include <modules/interrupts/Faults.h>

#include <core/debug/Common.h>
#include <core/interfaces/Interrupts.h>
#include <core/boot/Kernel.h>
#include <core/interfaces/Memory.h>
#include <core/drivers/PIC.h>
#include <core/debug/Debug.h>

void IDT_set(uint8_t entry, uint64_t base, uint8_t selector, uint8_t flags)
{
    idt[entry].l_off = base & 0xFFFF;
    idt[entry].u_off = (base >> 16) & 0xFFFF;
    idt[entry].justZero = 0;
    idt[entry].sel = selector;
    idt[entry].flags = flags;
}

void IDT_installISR()
{
    IDT_set(0, isr0, SEG_KCODE, IDT_ACCESS);
    IDT_set(1, isr1, SEG_KCODE, IDT_ACCESS);
    IDT_set(2, isr2, SEG_KCODE, IDT_ACCESS);
    IDT_set(3, isr3, SEG_KCODE, IDT_ACCESS);
    IDT_set(4, isr4, SEG_KCODE, IDT_ACCESS);
    IDT_set(5, isr5, SEG_KCODE, IDT_ACCESS);
    IDT_set(6, isr6, SEG_KCODE, IDT_ACCESS);
    IDT_set(7, isr7, SEG_KCODE, IDT_ACCESS);
    IDT_set(8, isr8, SEG_KCODE, IDT_ACCESS);
    IDT_set(9, isr9, SEG_KCODE, IDT_ACCESS);
    IDT_set(10, isr10, SEG_KCODE, IDT_ACCESS);
    IDT_set(11, isr11, SEG_KCODE, IDT_ACCESS);
    IDT_set(12, isr12, SEG_KCODE, IDT_ACCESS);
    IDT_set(13, isr13, SEG_KCODE, IDT_ACCESS);
    IDT_set(14, isr14, SEG_KCODE, IDT_ACCESS);
    IDT_set(15, isr15, SEG_KCODE, IDT_ACCESS);
    IDT_set(16, isr16, SEG_KCODE, IDT_ACCESS);
    IDT_set(17, isr17, SEG_KCODE, IDT_ACCESS);
    IDT_set(18, isr18, SEG_KCODE, IDT_ACCESS);
    IDT_set(19, isr19, SEG_KCODE, IDT_ACCESS);
    IDT_set(20, isr20, SEG_KCODE, IDT_ACCESS);
    IDT_set(21, isr21, SEG_KCODE, IDT_ACCESS);
    IDT_set(22, isr22, SEG_KCODE, IDT_ACCESS);
    IDT_set(23, isr23, SEG_KCODE, IDT_ACCESS);
    IDT_set(24, isr24, SEG_KCODE, IDT_ACCESS);
    IDT_set(25, isr25, SEG_KCODE, IDT_ACCESS);
    IDT_set(26, isr26, SEG_KCODE, IDT_ACCESS);
    IDT_set(27, isr27, SEG_KCODE, IDT_ACCESS);
    IDT_set(28, isr28, SEG_KCODE, IDT_ACCESS);
    IDT_set(29, isr29, SEG_KCODE, IDT_ACCESS);
    IDT_set(30, isr30, SEG_KCODE, IDT_ACCESS);
    IDT_set(31, isr31, SEG_KCODE, IDT_ACCESS);
}

void IDT_installIRQ()
{
    IDT_set(32, irq0, SEG_KCODE, IDT_ACCESS);
    IDT_set(33, irq1, SEG_KCODE, IDT_ACCESS);
    IDT_set(34, irq2, SEG_KCODE, IDT_ACCESS);
    IDT_set(35, irq3, SEG_KCODE, IDT_ACCESS);
    IDT_set(36, irq4, SEG_KCODE, IDT_ACCESS);
    IDT_set(37, irq5, SEG_KCODE, IDT_ACCESS);
    IDT_set(38, irq6, SEG_KCODE, IDT_ACCESS);
    IDT_set(39, irq7, SEG_KCODE, IDT_ACCESS);
    IDT_set(40, irq8, SEG_KCODE, IDT_ACCESS);
    IDT_set(41, irq9, SEG_KCODE, IDT_ACCESS);
    IDT_set(42, irq10, SEG_KCODE, IDT_ACCESS);
    IDT_set(43, irq11, SEG_KCODE, IDT_ACCESS);
    IDT_set(44, irq12, SEG_KCODE, IDT_ACCESS);
    IDT_set(45, irq13, SEG_KCODE, IDT_ACCESS);
    IDT_set(46, irq14, SEG_KCODE, IDT_ACCESS);
    IDT_set(47, irq15, SEG_KCODE, IDT_ACCESS);

    //SYSCALL (Access allows for USER rather then KERNEL (3 rather than 0))
    IDT_set(128, isr128, SEG_KCODE, 0xEE);
}

bool IDT_init()
{
    //Info for the CPU
    idt_p.limit = (sizeof(struct IDT_entry) * IDT_ENTRIES) - 1;
    idt_p.base = &idt;

    //Clear the memory space
    memset(&idt, 0, sizeof(struct IDT_entry)*IDT_ENTRIES);

    //Setup Handlers
    IDT_installISR();
    IDT_installIRQ();

    //Give CPU the info
    idt_setup((uint64_t *)&idt_p);

    //Tell FantomOS system
    Interrupts_addISR = &IDT_addISR;
    Interrupts_addIRQ = &IDT_addIRQ;
    sysInfo.interrupts_enabled = true;

    return true;
}

void IDT_handle(Registers *regs)
{
    if(regs->int_num < 32 && !(handlers[regs->int_num]))
        Faults_handle(regs);

    if(handlers[regs->int_num])
        handlers[regs->int_num](regs);

    PIC_eoi(regs->int_num);
}

void IDT_addISR(uint8_t isr, void* addr)
{
    //KASSERT(isr < IRQ0 && isr >= 0);
    handlers[isr] = addr;
}

void IDT_addIRQ(uint8_t irq, void* addr)
{
    KASSERT((irq + 32) >= IRQ0 && (irq + 32) < IDT_ENTRIES);
    handlers[irq + IRQ0] = addr;
}
