
//ME
#include <modules/physmem_hybrid/PhysMem_Hybrid.h>

//NO MODULE REQUIREMENTS
//(USE NO OTHER PHYSMEM ALLOCATOR)

//CORE
#include <core/debug/Common.h>
#include <core/data_structs/Bitmap.h>
#include <core/data_structs/Stack.h>
#include <core/boot/multiboot.h>
#include <core/boot/Kernel.h>
#include <core/interfaces/Memory.h>
#include <core/debug/Debug.h>

void PhysMem_Hybrid_free(void *block_address)
{
    if((uint32_t)block_address < (MAX_PHYS_STACK * PAGE_SIZE))
    {
        Stack_push(phys_stack, block_address);
    }
    else
    {
        //Convert to Index
        uint32_t index = ((uint32_t)block_address / PAGE_SIZE);
        Bitmap_clearBit(phys_bitmap, index);
    }
}

void *PhysMem_Hybrid_allocate()
{
    if(phys_stack->elements > 0)
    {
        return Stack_pop(phys_stack);
    }
    else
    {
        for(uint32_t index = 0 ; index < phys_bitmap->bytes * 8 ; index++)
        {
            if(!Bitmap_checkBit(phys_bitmap, index))
            {
                Bitmap_setBit(phys_bitmap, index);
                return (index * PAGE_SIZE);
            }
        }
        Kernel_panic("Out of Physical Memory");
        return NULL;
    }
}

bool PhysMem_Hybrid_init()
{
    //Get Kernel's Physical End Address
    if(mb->mods_count > 0)
        mem_info_h.phys_kend = (uint32_t)((multiboot_module_t *)(mb->mods_addr + KBASE_VADDRESS))[mb->mods_count - 1].mod_end;
    else
        mem_info_h.phys_kend = (uint32_t)kernel_end - KBASE_VADDRESS;

    //Get size of physical memory
    mem_info_h.phys_total = 0;
    multiboot_memory_map_t *mmape = mb->mmap_addr + KBASE_VADDRESS;
    uint32_t mmape_end = (uint32_t)mmape + mb->mmap_length;
    while((uint32_t)mmape < mmape_end)
    {
        mem_info_h.phys_total += mmape->len;
        mmape++;
    }

    //Get total memory blocks/frames/pages
    mem_info_h.phys_blocks = mem_info_h.phys_total / PAGE_SIZE;

    //Point to the BitMap
    phys_bitmap = (mem_info_h.phys_kend + KBASE_VADDRESS);

    //Point to the Stack
    phys_stack = (uint32_t)phys_bitmap + sizeof(bitmap_t);

    //Init the Map
    phys_bitmap->bytes = mem_info_h.phys_blocks / 8;
    phys_bitmap->base = (uint32_t)phys_stack + sizeof(stack_t) /*+ (MAX_PHYS_STACK * sizeof(uint32_t))*/;

    //Set all as used
    Bitmap_setAll(phys_bitmap);

    //Init the Stack
    phys_stack->base = /*(uint32_t)phys_stack + sizeof(stack_t)*/ (uint32_t)phys_bitmap->base + phys_bitmap->bytes + 1;
    phys_stack->elements = 0;
    phys_stack->max_elements = MAX_PHYS_STACK;

    //See what's free in MultiBoot Header
    mem_info_h.phys_mbfree = 0;
    multiboot_memory_map_t *mmap_entry = mb->mmap_addr + KBASE_VADDRESS;
    //uint32_t new_kend = (uint32_t)(phys_bitmap->base - KBASE_VADDRESS + phys_bitmap->bytes) & (0xFFFFF000);
    uint32_t new_kend = (uint32_t)((phys_stack->base) - KBASE_VADDRESS + (MAX_PHYS_STACK * sizeof(uint32_t)))  & (0xFFFFF000);
    while((uint32_t)mmap_entry < (mb->mmap_addr + KBASE_VADDRESS + mb->mmap_length))
    {
        if(mmap_entry->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            uint32_t paddress = mmap_entry->addr;
            for(uint32_t a = paddress ; a < mmap_entry->len ; a += PAGE_SIZE)
            {
                if(new_kend < (uint32_t)a)
                {
                    uint32_t index = ((uint32_t)a / PAGE_SIZE);
                    Bitmap_clearBit(phys_bitmap, index);
                    mem_info_h.phys_mbfree++;
                }
            }
        }
        mmap_entry++;
    }

    uint32_t start = MAX_PHYS_STACK;
    if(mem_info_h.phys_mbfree < MAX_PHYS_STACK)
        start = mem_info_h.phys_mbfree;

    for(uint32_t e = start ; e > 0 ; e--)
    {
        if(!Bitmap_checkBit(phys_bitmap, e))
        {
            Bitmap_setBit(phys_bitmap, e);
            Stack_push(phys_stack, e*PAGE_SIZE);
        }

        if(phys_stack->elements == phys_stack->max_elements)
            break;
    }

    printMemInfoHybrid();

    //
    //Set Core Memory Functions
    PhysMem_allocate = &PhysMem_Hybrid_allocate;
    PhysMem_free = &PhysMem_Hybrid_free;
    sysInfo.physicalMemMgmt_enabled = true;
    //
    //

    return true;
}

void printMemInfoHybrid()
{
    kprintf("\n------ Physical Memory Info ------\n");
    kprintf("Physical Kernel End (hex)-> %x\n", mem_info_h.phys_kend );
    kprintf("Total Physical Memory -> %dMB\n", (uint32_t)mem_info_h.phys_total / 1000000 );
    kprintf("Total Physical Blocks -> %d\n", mem_info_h.phys_blocks );
    kprintf("MB Free Blocks -> %d\n", mem_info_h.phys_mbfree );
    kprintf("----------------------------------\n\n");
}
