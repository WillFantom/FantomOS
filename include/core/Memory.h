#ifndef __MEMORY_H
#define __MEMORY_H

#include <core/Common.h>
#include <core/Kernel.h>

//PAGE / FRAME Size
#define PAGE_SIZE 0x1000

//Segments
#define SEG_NULL 0
#define SEG_KCODE 0x08
#define SEG_KDATA 0x10
#define SEG_UCODE 0x18
#define SEG_UDATA 0x20
#define SEG_TSS 0x28

//Virt Memory Map - Kernel
#define KBASE_VADDRESS  0xC0000000
#define KEND_VADDRESS (KBASE_VADDRESS + 0x400000)
#define KERNEL_HEAP_BASE (KEND_VADDRESS)
#define KERNEL_HEAP_SIZE_MIN 0x00400000
#define KERNEL_HEAP_SIZE_MAX 0x00400000

//Virt Memory Map - User
#define USER_STACK_TOP (KBASE_VADDRESS - PAGE_SIZE)
#define USER_STACK_SIZE (PAGE_SIZE)
#define USER_STACK_BASE (USER_STACK_TOP - USER_STACK_SIZE)
#define USER_HEAP_BASE 0x20000000
#define USER_HEAP_TOP (USER_STACK_BASE)
#define USER_CODE_BASE 0x10000000

//FLAGS (x86)
#define READ_WRITE 0x1
#define READ_ONLY 0x0
#define KERNEL_MODE 0x0
#define USER_MODE 0x1

//x86 Specific Registers (Trap-Frame)
typedef struct registers
{
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_num, err_code;
    unsigned int eip, cs, flags, esp0, ss0;
} __attribute__((packed)) Registers;

//Optional PhysMem Mgmt Functions
//Alloc must return NULL (in Common.h) if out of memory
//Must also return a full address (page alligned)
void *(*PhysMem_allocate)(void);
void (*PhysMem_free)(void *block);

#endif
