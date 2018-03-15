
//ME
#include <modules/Scheduler_RR.h>

//REQUIREMENTS
#include <modules/Process.h>
#include <modules/LinkedList.h>

//CORE
#include <core/Common.h>
#include <core/Kernel.h>
#include <core/Memory.h>
#include <core/Interrupts.h>
#include <core/Debug.h>

bool Scheduler_RR_init()
{
    Scheduler_running = false;
    process_list = LinkedList_create();
    currentProcess = NULL;
    tick = 0;

    Interrupts_addIRQ(0, &PIT_handler);

    return true;
}

void PIT_handler(Registers *regs)
{
    if(Scheduler_running)
        tick++;

    if(tick % 2 == 0 && Scheduler_running)
    {
        //kprintf("Context Switch %d\n", tick/3);
        Process_contextSwitch(regs);
    }
    else if(tick % 2 != 0 && Scheduler_running)
    {
        __asm__ volatile("mov %0, %%DR0" : : "a" (0));
    }
}

pcb_t *Scheduler_getCurrent()
{
    if(!Scheduler_running)
        return NULL;

    return currentProcess;
}

void Scheduler_addProcess(pcb_t *process)
{
    KASSERT(process->state == PROCESS_STATE_CREATED);

    if(process_list->items == 0)
        currentProcess = process;

    process->state = PROCESS_STATE_WAIT;
    LinkedList_addNode(process_list, process);
    kprintf("added process! %d\n", process->id);
}

void Scheduler_removeProcess(pcb_t *process)
{
    KASSERT(process->id != 0);
    process->state = PROCESS_STATE_KILLED;
    LinkedList_removeNode(process_list, LinkedList_getNodeByData(process_list, process));
}

pcb_t *Scheduler_nextProcess()
{
    KASSERT(process_list->items > 0);
    pcb_t *nextProcess = LinkedList_removeNode(process_list, LinkedList_getNodeByIndex(process_list, 0));
    LinkedList_addNode(process_list, nextProcess);
    return nextProcess;
}
