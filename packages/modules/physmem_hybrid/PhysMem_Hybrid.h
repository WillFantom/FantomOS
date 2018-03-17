#ifndef __PHYSMEMHYBRID_H
#define __PHYSMEMHYBRID_H

#include <core/debug/Common.h>
#include <core/data_structs/Bitmap.h>
#include <core/data_structs/Stack.h>

//Memory info
typedef struct PhysMemInfoH
{
    uint32_t phys_kend;
    uint64_t phys_total;
    uint32_t phys_blocks;
    uint32_t phys_mbfree;
} PhysMemInfoH_t;

PhysMemInfoH_t mem_info_h;

void printMemInfoHybrid();

//MAP
#define MAX_PHYS_STACK 0x10000
bitmap_t *phys_bitmap;
stack_t *phys_stack;

//Funcs
PhysMem_Hybrid_init();
void *PhysMem_Hybrid_allocate();
void PhysMem_Hybrid_free(void *block_address);

#endif
