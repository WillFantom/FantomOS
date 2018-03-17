#ifndef __PHYSMEMBITMAP_H
#define __PHYSMEMBITMAP_H

#include <core/debug/Common.h>
#include <core/data_structs/Bitmap.h>

//Memory info
typedef struct PhysMemInfo
{
    uint32_t phys_kend;
    uint64_t phys_total;
    uint32_t phys_blocks;
    uint32_t phys_mbfree;
} PhysMemInfo_t;

PhysMemInfo_t mem_info;

void printMemInfo();

//MAP
bitmap_t *phys_map;

//Funcs
PhysMem_Bitmap_init();
void *PhysMem_Bitmap_allocate();
void PhysMem_Bitmap_free(void *block_address);

#endif
