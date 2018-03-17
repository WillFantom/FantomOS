/*  - Faults.c
    AUTHOR: Will Fantom
    DESC:   handles any fault, providing an error message
*/

#include <modules/interrupts/Faults.h>
#include <core/debug/Debug.h>
#include <core/interfaces/Memory.h>

static char *fault_msg[] = {
    "Division by Zero", //0
    "Debug Fault", //1
    "Non-Maskable Interrupt Exception", //2
    "Breakpoint Exception", //3
    "Into Detected Overflow", //4
    "Out of Bounds", //5
    "Invalid Opcode", //6
    "No Coprocessor Exception", //7
    "Double Fault", //8
    "Coprocessor Segment Overrun Exception", //9
    "Bad TSS", //10
    "Seg not Present", //11
    "Stack Fault", //12
    "General Protection Fault", //13
    "Page Fault", //14
    "Unknown Interrupt", //15
    "Coprocessor Fault", //16
    "Alignment Check Exception", //17
    "Machine Check Exception", //18
    "Reserved", //19
    "Reserved", //20
    "Reserved", //21
    "Reserved", //22
    "Reserved", //23
    "Reserved", //24
    "Reserved", //25
    "Reserved", //26
    "Reserved", //27
    "Reserved", //28
    "Reserved", //29
    "Reserved", //30
    "Reserved" //31
};

void Faults_handle(Registers *regs)
{
    kprintf("\nFault Handled || int_num: %d || err_code: %d\n", regs->int_num, regs->err_code);
    Kernel_panic(fault_msg[regs->int_num]);
}
