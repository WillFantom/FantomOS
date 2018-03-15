#ifndef __HEAPMEMSBRK_H
#define __HEAPMEMSBRK_H

#include <core/Common.h>

//Current top of kernel heap -> to be used if no processes used!
uint32_t kheap_top;

//Functions
bool HeapMem_SBRK_init();
uint32_t HeapMem_SBRK(ptrdiff_t pages);

//Maps
void *(*malloc)(uint32_t size);
void *(*calloc)(uint32_t elements, uint32_t size);
void (*free)(uint32_t addr);

#endif
