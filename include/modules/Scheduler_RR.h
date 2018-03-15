#ifndef __SCHEDULERRR_H
#define __SCHEDULERRR_H

#include <core/Common.h>

//REQUIREMENTS
#include <modules/LinkedList.h>
#include <modules/Process.h>

//Is Active
bool Scheduler_running;

//PIT handling
uint32_t tick;

//Current
pcb_t *currentProcess;

//Process List
LinkedList *process_list;

//Functions
bool Scheduler_RR_init();
pcb_t *Scheduler_getCurrent();
void Scheduler_addProcess(pcb_t *process);
void Scheduler_removeProcess(pcb_t *process);
pcb_t *Scheduler_nextProcess();
void PIT_handler(Registers *regs);

#endif
