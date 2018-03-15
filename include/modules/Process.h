#ifndef __PROCESS_H
#define __PROCESS_H

#include <modules/Keyboard.h>

#include <core/Common.h>
#include <core/Memory.h>

//REQUIREMENT
#include <modules/VirtMem_Paging.h>

//Track Process IDs
uint32_t curr_PID;

//Process Contol Blocks
typedef struct ProcessHeap
{
    uint32_t top;
    uint32_t base;
} Process_heap;

typedef struct ProcessStack
{
    uint32_t base;
    uint32_t top;
} Process_stack;

typedef struct ProcessControlBlock pcb_t;
struct ProcessControlBlock
{
    uint32_t id;
    char title[32];
    Process_stack kernelStack;
    Process_stack userStack;
    Process_heap heap;
    uint8_t state;
    pcb_t *parent;
    page_dir_t *page_dir;
    keybuffer_t *keybuffer;
};

//States
#define PROCESS_STATE_CREATED   0x1
#define PROCESS_STATE_WAIT      0x2
#define PROCESS_STATE_ACTIVE    0x3
#define PROCESS_STATE_KILLED    0x4
#define PROCESS_STATE_BLOCKED   0x5

//Kernel Process
pcb_t *kprocess;
pcb_t *Process_getKProcess();

//Functions
bool Process_init();
pcb_t *Process_createUserProcess(char *file_name);
void Process_contextSwitch(Registers *regs);
void Process_fSwitch();
pcb_t *Process_createChild(char *file_name, pcb_t *parent);
void Process_blockProcess();
void Process_makeForeground();
bool Process_kill(pcb_t *process);

//TSS x86 ONLY
struct tss_entry
{
    uint32_t link : 16;
    uint32_t res0 : 16;
    uint32_t esp0 : 32; //Stacky
    uint32_t ss0 : 16; //Data Seg descriptor
    uint32_t res1 : 16;
    uint32_t esp1 : 32;
    uint32_t ss1 : 16;
    uint32_t res2 : 16;
    uint32_t esp2 : 32;
    uint32_t ss2 : 16;
    uint32_t res3 : 16;
    uint32_t cr3 : 32; //Page Dir
    uint32_t eip : 32;
    uint32_t eflags : 32;
    uint32_t eax : 32;
    uint32_t ecx : 32;
    uint32_t edx : 32;
    uint32_t ebx : 32;
    uint32_t esp : 32;
    uint32_t ebp : 32;
    uint32_t esi : 32;
    uint32_t edi : 32;
    uint32_t es : 16;
    uint32_t res4 : 16;
    uint32_t cs : 16;
    uint32_t res5 : 16;
    uint32_t ss : 16;
    uint32_t res6 : 16;
    uint32_t ds : 16;
    uint32_t res7 : 16;
    uint32_t fs : 16;
    uint32_t res8 : 16;
    uint32_t gs : 16;
    uint32_t res9 : 16;
    uint32_t ldtr : 16;
    uint32_t res10 : 16;
    uint32_t res11 : 16;
    uint32_t iopb : 16;
} __attribute__((packed));

struct tss_entry tss_e;

void Process_setTSS(uint32_t ss0, uint32_t esp0);

#endif
