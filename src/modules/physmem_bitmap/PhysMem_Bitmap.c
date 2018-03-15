#include <modules/PhysMem_Bitmap.h>

#include <core/Common.h>
#include <core/Bitmap.h>
#include <core/multiboot.h>
#include <core/Kernel.h>
#include <core/Memory.h>
#include <core/Debug.h>

void PhysMem_Bitmap_free(void *block_address)
{
    //Convert to Index
    uint32_t index = ((uint32_t)block_address / PAGE_SIZE);

    Bitmap_clearBit(phys_map, index);
}

void *PhysMem_Bitmap_allocate()
{

    for(uint32_t index = 0 ; index < phys_map->bytes * 8 ; index++)
    {
        if(!Bitmap_checkBit(phys_map, index))
        {
            Bitmap_setBit(phys_map, index);
            return (index * PAGE_SIZE);
        }
    }
    Kernel_panic("Out of Physical Memory");
    return NULL;
}

bool PhysMem_Bitmap_init()
{
    //Get Kernel's Physical End Address
    if(mb->mods_count > 0)
        mem_info.phys_kend = (uint32_t)((multiboot_module_t *)(mb->mods_addr + KBASE_VADDRESS))[mb->mods_count - 1].mod_end;
    else
        mem_info.phys_kend = (uint32_t)kernel_end - KBASE_VADDRESS;

    //Get size of physical memory
    mem_info.phys_total = 0;
    multiboot_memory_map_t *mmape = mb->mmap_addr + KBASE_VADDRESS;
    uint32_t mmape_end = (uint32_t)mmape + mb->mmap_length;
    while((uint32_t)mmape < mmape_end)
    {
        mem_info.phys_total += mmape->len;
        mmape++;
    }

    //Get total memory blocks/frames/pages
    mem_info.phys_blocks = mem_info.phys_total / PAGE_SIZE;

    //Point to the BitMap
    phys_map = (mem_info.phys_kend + KBASE_VADDRESS);

    //Init the Map
    phys_map->bytes = mem_info.phys_blocks / 8;
    phys_map->base = mem_info.phys_kend + KBASE_VADDRESS + sizeof(bitmap_t);

    //Set all as used
    Bitmap_setAll(phys_map);

    //See what's free in MultiBoot Header
    mem_info.phys_mbfree = 0;
    multiboot_memory_map_t *mmap_entry = mb->mmap_addr + KBASE_VADDRESS;
    uint32_t new_kend = (uint32_t)(phys_map->base - KBASE_VADDRESS + phys_map->bytes) & (0xFFFFF000);
    while((uint32_t)mmap_entry < (mb->mmap_addr + KBASE_VADDRESS + mb->mmap_length))
    {
        if(mmap_entry->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            uint32_t paddress = mmap_entry->addr;
            for(uint32_t a = paddress ; a < mmap_entry->len ; a += PAGE_SIZE)
            {
                if(new_kend < (uint32_t)a)
                {
                    mem_info.phys_mbfree++;
                    PhysMem_Bitmap_free(a);
                }
            }
        }
        mmap_entry++;
    }

    printMemInfo();

    //
    //Set Core Memory Functions
    PhysMem_allocate = &PhysMem_Bitmap_allocate;
    PhysMem_free = &PhysMem_Bitmap_free;
    sysInfo.physicalMemMgmt_enabled = true;
    //
    //

    return true;
}

void printMemInfo()
{
    kprintf("\n------ Physical Memory Info ------\n");
    kprintf("Physical Kernel End (hex)-> %x\n", mem_info.phys_kend );
    kprintf("Total Physical Memory -> %dMB\n", (uint32_t)mem_info.phys_total / 1000000 );
    kprintf("Total Physical Blocks -> %d\n", mem_info.phys_blocks );
    kprintf("MB Free Blocks -> %d\n", mem_info.phys_mbfree );

    uint32_t freeBlocks = 0;
    for(uint32_t index = 0 ; index < phys_map->bytes * 8 ; index++)
        if(!Bitmap_checkBit(phys_map, index))
            freeBlocks++;

    kprintf("Percent Used -> %d\n", (uint32_t)(freeBlocks/mem_info.phys_blocks) );
    kprintf("----------------------------------\n\n");
}
