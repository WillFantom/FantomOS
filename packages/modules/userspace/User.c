
//ME
#include <modules/userspace/User.h>

//REQUIREMENTS
#include <modules/userspace/Syscall.h>
#include <modules/processes/Process.h>
#include <modules/processes/Scheduler_RR.h>

//CORE
#include <core/debug/Debug.h>
#include <core/debug/Common.h>
#include <core/interfaces/Memory.h>
#include <core/interfaces/Interrupts.h>

bool User_switchTo()
{
    //char *baseApp;

    kprintf("Switching to user space\n");
    kprintf("Initial user app -> %s\n", BASE_APP);

    //Enable Syscalls
    Interrupts_addISR(SYSCALL_INTNUM , &Syscall_handle);
    //kprintf("intnum -> %d\n", SYSCALL_INTNUM);

    //Create Initial Process
    pcb_t *init = Process_createUserProcess(BASE_APP);

    //Add Kernel Process to scheduler (As Round Robin is used by defualt)
    Scheduler_addProcess(Process_getKProcess());

    //Set the Kernel Stack in TSS
    Process_setTSS(SEG_KDATA, (uint32_t) init->kernelStack.top);

    //Set the stack pointer
    __asm__ volatile("movl %0, %%esp" : : "r" (init->userStack.top));

    //Change Page Dir
    kprintf("New DIR loc -> %x\n", init->page_dir);
    VirtMem_changeDir(init->page_dir);

    //Enable Context Switching
    Scheduler_running = true;

    //Enter User Space
    kprintf("UCODE POINT (Regs) -> %x\nUCODE POINT -> %x\n",((Registers *)(init->userStack.top))->eip, USER_CODE_BASE );
    User_switch(USER_CODE_BASE);

    //Won't even reach here (unless kill init process)
    kprintf("Now in Ring 3\n");
    return true;
}
