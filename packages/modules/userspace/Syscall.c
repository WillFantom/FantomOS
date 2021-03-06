//ME
#include <modules/userspace/Syscall.h>

//REQUIREMENTS
#include <modules/processes/Process.h>
#include <modules/processes/Scheduler_RR.h>
#include <modules/drivers/VGA.h>
#include <modules/vfs/VFS.h>
#include <modules/heapmem_sbrk/HeapMem_SBRK.h>
#include <modules/drivers/Keyboard.h>
#include <modules/drivers/RTC.h>

//CORE
#include <core/interfaces/Memory.h>
#include <core/interfaces/Interrupts.h>

//All the syscalls
void *Syscall_map[SYSCALL_COUNT] = {

    //Processes
    &Process_createChild,
    &Scheduler_getCurrent,

    //Memory
    &HeapMem_SBRK,

    //File System
    &VFS_open,
    &VFS_read,
    &VFS_fileInfo,
    &VFS_getFileAtIndex,

    //Screen Functions
    &VGA_putChar,
    &VGA_putStr,
    &VGA_cls,
    &VGA_colourError,
    &VGA_standardColour,

    //Keyboard Functions
    &Keyboard_getInput,

    //Time
    &RTC_printTime,

    //Testing (may not be stable)
    &Process_makeForeground,
    &Process_kill,
};

void Syscall_handle(Registers *regs)
{

    if(regs->eax >= SYSCALL_COUNT)
        return;

    unsigned int ret = 0;

    __asm__ volatile (" \
     push %1; \
     push %2; \
     push %3; \
     push %4; \
     push %5; \
     call *%6; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     " : "=a" (ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (Syscall_map[regs->eax]));

    regs->eax = ret;
}
