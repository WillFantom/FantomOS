

//ME
#include <modules/HeapMem_SBRK.h>

//REQUIREMENTS (NOT OPTIONAL)
#include <modules/VirtMem_Paging.h>
#include <modules/Process.h>
#include <modules/Scheduler_RR.h>
#include <modules/malloc.h> //Thanks DougLea!!

//CORE
#include <core/Common.h>
#include <core/Kernel.h>
#include <core/Memory.h>
#include <core/Debug.h>

bool HeapMem_SBRK_init()
{
    kheap_top = KERNEL_HEAP_BASE;

    malloc = &dlmalloc;
    calloc = &dlcalloc;
    free = &dlfree;

    sysInfo.heapMemMgmt_enabled = true;
    return true;
}

uint32_t HeapMem_KSBRK(ptrdiff_t pages)
{
    pages = pages / PAGE_SIZE;
    pcb_t *currentProcess = Scheduler_getCurrent();
    uint32_t addr = kheap_top;

    if(pages > 0)
    {
        if((kheap_top + (pages * PAGE_SIZE)) > (uint32_t)(KERNEL_HEAP_BASE + KERNEL_HEAP_SIZE_MAX))
            Kernel_panic("KHeap can't allocate any more!!");


        for(uint32_t p = 0; p < pages ; p++)
        {
            if(currentProcess == NULL)
                VirtMem_mapPage(VirtMem_getKDIR(), kheap_top, 0, READ_WRITE, KERNEL_MODE);
            else
                VirtMem_mapPage(currentProcess->page_dir, kheap_top, 0, READ_WRITE, KERNEL_MODE);
            memset(kheap_top, 0, PAGE_SIZE);
            kheap_top += PAGE_SIZE;
        }
        //return addr;
    }
    else if(pages < 0)
    {
        if(kheap_top - (pages * PAGE_SIZE) < KERNEL_HEAP_BASE);
            Kernel_panic("KHeap can't free any more!!");

        for(uint32_t p = 0; p < pages ; p++)
        {
            if(currentProcess == NULL)
                VirtMem_unmapPage(VirtMem_getKDIR(), kheap_top, true);
            else
                VirtMem_unmapPage(currentProcess->page_dir, kheap_top, true);
            kheap_top -= PAGE_SIZE;
        }

        //return addr;
    }
    return addr;
}


uint32_t HeapMem_SBRK(ptrdiff_t pages)
{
    pages = pages / PAGE_SIZE;
    pcb_t *currentProcess = Scheduler_getCurrent();
    uint32_t addr = currentProcess->heap.top;

    if(pages >= 0)
    {
        /*if(currentProcess->heap.top + (pages * PAGE_SIZE) > USER_HEAP_TOP);
            Kernel_panic("Heap can't allocate any more!!");*/

        for(uint32_t p = 0; p < pages ; p++)
        {
            VirtMem_mapPage(currentProcess->page_dir, currentProcess->heap.top, 0, READ_WRITE, USER_MODE);
            memset(currentProcess->heap.top, 0, PAGE_SIZE);
            currentProcess->heap.top += PAGE_SIZE;
        }
        /*kprintf("Allocated in USer Heap -> %x\n", addr);
        kprintf("New UHeap top -> %x\n", currentProcess->heap.top);*/
        return addr;
    }
    else if(pages < 0)
    {
        if(currentProcess->heap.top - (pages * PAGE_SIZE) < USER_HEAP_BASE);
            Kernel_panic("Heap can't free any more!!");

        for(uint32_t p = 0; p < pages ; p++)
        {
            VirtMem_unmapPage(currentProcess->page_dir, currentProcess->heap.top, true);
            currentProcess->heap.top -= PAGE_SIZE;
        }

        return addr;
    }

}
