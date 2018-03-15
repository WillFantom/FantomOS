
//ME
#include <modules/Process.h>

//REQUIREMENTS
#include <modules/VFS.h>
#include <modules/LinkedList.h>
#include <modules/GDT.h>
#include <modules/HeapMem_SBRK.h>
#include <modules/VirtMem_Paging.h>
#include <modules/Scheduler_RR.h>
#include <modules/Keyboard.h>

//CORE
#include <core/Common.h>
#include <core/Kernel.h>
#include <core/Memory.h>
#include <core/Debug.h>

/*  Create Kernel Process
    -   This is the main kernel process and will get the dir created in the
        paging setup.
*/
void Process_createKernelProcess()
{
    //Get the space
    kprocess = dlcalloc(1, sizeof(pcb_t));

    kprocess->id = curr_PID++;
    kprocess->kernelStack.base = (uint32_t)calloc(1, PAGE_SIZE);
    kprocess->kernelStack.top = kprocess->kernelStack.base + PAGE_SIZE - sizeof(Registers);
    kprocess->userStack.base = NULL;
    kprocess->userStack.top = NULL;
    kprocess->heap.base = NULL;
    kprocess->heap.top = NULL;
    kprocess->page_dir = VirtMem_getKDIR();
    kprocess->parent = NULL;
    kprocess->keybuffer = Keyboard_getNewBuffer();

    Registers *regs = (uint32_t)calloc(1, sizeof(Registers));

    regs->cs = SEG_KCODE;
    regs->ds = SEG_KDATA;
    regs->es = SEG_KDATA;
    regs->fs = SEG_KDATA;
    regs->gs = SEG_KDATA;

    //Enb interrupts for kprocess
    regs->flags = 0x0202;
    regs->esp0 = 0;
    regs->ss0 = 0;

    regs->eip = (uint32_t)&Kernel_halt; //Just an endless while loop... could be a sys monitor in kernel space???
    regs->int_num = 32; //IRQ0 (timer)

    memcpy(kprocess->kernelStack.top, regs, sizeof(Registers));
    free(regs);

    kprocess->state = PROCESS_STATE_CREATED;
}

pcb_t *Process_getKProcess()
{
    return kprocess;
}

void Process_setTSS(uint32_t ss0, uint32_t esp0)
{
    tss_e.ss0 = ss0;
    tss_e.esp0 = esp0;
}

pcb_t *Process_createUserProcess(char *file_name)
{
    //Get Process Space
    pcb_t *process = (uint32_t)calloc(1, sizeof(pcb_t));

    //Create Process Directory
    process->page_dir = VirtMem_createDir(0);
    VirtMem_cpyDir(VirtMem_getKDIR(), process->page_dir, false);

    //Get Process From FS
    VFS_node_t *file = VFS_open(file_name, 'r');
    KASSERT(file != NULL);

    //Move the Code
    for(uint32_t s = 0 ; s < (file->file_size / PAGE_SIZE) + 1 ; s++)
    {
        uint32_t p = PhysMem_allocate();
        VirtMem_mapPage(process->page_dir, USER_CODE_BASE + (s*PAGE_SIZE), p, READ_WRITE, USER_MODE);
        VFS_read(file,  VirtMem_mapToCurrent(p, 8), PAGE_SIZE, s*PAGE_SIZE);
    }

    //Unique ID
    process->id = curr_PID++;

    //Not always a parent desired (e.g. proc 1)
    process->parent = NULL;

    //Per Process Key Buffer (CURRENTLY UNUSED -> TODO)
    process->keybuffer = Keyboard_getNewBuffer();

    //Stack
    process->kernelStack.base = (uint32_t)calloc(1, PAGE_SIZE);
    process->kernelStack.top = process->kernelStack.base + PAGE_SIZE;
    process->userStack.base = USER_STACK_BASE;
    process->userStack.top = process->userStack.base + USER_STACK_SIZE - sizeof(Registers);
    uint32_t stack_paddress = PhysMem_allocate();
    VirtMem_mapPage(process->page_dir, USER_STACK_BASE, stack_paddress, READ_WRITE, USER_MODE);

    //Heap
    process->heap.base = USER_HEAP_BASE;
    process->heap.top = USER_HEAP_BASE;

    //Stack Cont
    Registers *regs = (uint32_t)calloc(1, sizeof(Registers));
    memset(regs, 0, sizeof(Registers));
    regs->cs = SEG_UCODE | 3;
    regs->ds = SEG_UDATA | 3;
    regs->es = SEG_UDATA | 3;
    regs->fs = SEG_UDATA | 3;
    regs->gs = SEG_UDATA | 3;

    regs->flags = 0x0202;
    regs->eip = USER_CODE_BASE;
    regs->int_num = 32; //IRQ0 (timer)

    regs->esp0 = process->userStack.top;
    regs->ss0 = SEG_UDATA | 3;

    //Copy the Registers to stack
    if(process->id == 1)
    {
        VirtMem_mapPage(VirtMem_getKDIR(), USER_STACK_BASE, stack_paddress, READ_WRITE, USER_MODE);
        memcpy(process->userStack.top, regs, sizeof(Registers));
    }
    else
    {
        VirtMem_mapToCurrent(stack_paddress, 0);
        memcpy(TEMP_VADDR + USER_STACK_SIZE - sizeof(Registers), regs, sizeof(Registers));
    }

    //Free The Heap Space
    free(regs);

    //Add to Scheduler
    process->state = PROCESS_STATE_CREATED;
    Scheduler_addProcess(process);

    return process;

}

void Process_blockProcess()
{
    Scheduler_getCurrent()->state = PROCESS_STATE_BLOCKED;
    Process_fSwitch();
}

pcb_t *Process_createChild(char *file_name, pcb_t *parent)
{
    pcb_t* child = Process_createUserProcess(file_name);
    child->parent = parent;
    return child;
}

bool Process_kill(pcb_t *process)
{
    //Process Kill
    Scheduler_removeProcess(process);
    VirtMem_freeDir(process->page_dir);

    //Free up parent if needed
    if(process->parent != NULL)
        if(process->parent->state == PROCESS_STATE_BLOCKED)
            process->parent->state = PROCESS_STATE_WAIT;

    Process_fSwitch();

}

void Process_makeForeground()
{
    if(Scheduler_getCurrent()->parent != NULL)
        if(Scheduler_getCurrent()->parent->id > 0)
            Scheduler_getCurrent()->parent->state = PROCESS_STATE_BLOCKED;
    Process_fSwitch();
}


void Process_fSwitch()
{
    interrupts_enb();
    __asm__ volatile("int %0" : : "i" (32));
}

void Process_contextSwitch(Registers *regs)
{
    //Get the Next Process
    pcb_t *process = Scheduler_getCurrent();
    pcb_t *nextProcess = Scheduler_nextProcess();

    while(nextProcess->state == PROCESS_STATE_KILLED || nextProcess->state == PROCESS_STATE_BLOCKED)
        nextProcess = Scheduler_nextProcess();

    //Is Switch Needed?
    if(process->id == nextProcess->id)
        return;

    //Save the current context
    if(process->id == kprocess->id)
    {
        process->kernelStack.top = regs;
    }
    else
    {
        process->userStack.top = regs;
    }

    //Set State of Current
    if(process->state == PROCESS_STATE_ACTIVE)
        process->state = PROCESS_STATE_WAIT;

    //Switch Context
    if(nextProcess->id != kprocess->id)
    {
        //Switching to Userspace Process
        Process_setTSS(SEG_KDATA, (uint32_t)nextProcess->kernelStack.top);
        __asm__ volatile("mov %0, %%DR0" : : "r" (nextProcess->userStack.top));
    }
    else
    {
        //Switching to Kernel Process
        __asm__ volatile("mov %0, %%DR0" : : "r" (nextProcess->kernelStack.top));
    }

    //Change Address Space and Set New State
    process->state = PROCESS_STATE_ACTIVE;
    currentProcess = nextProcess;
    VirtMem_changeDir(nextProcess->page_dir);
}

bool Process_init()
{
    Process_createKernelProcess();
    sysInfo.scheduler_enabled = true;
}
