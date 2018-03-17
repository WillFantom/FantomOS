
#include <modules/virtmem_paging/VirtMem_Paging.h>

#include <core/debug/Common.h>
#include <core/interfaces/Memory.h>
#include <core/boot/Kernel.h>
#include <core/interfaces/Interrupts.h>
#include <core/debug/Debug.h>

/*  PageFault Handler
    -   Detects the address and error code of the pagefault.
        Replaces the default handler found in FantomOSs deafult IDT
*/
void VirtMem_handlePageFault(Registers *regs)
{
    uint32_t address = VirtMem_pagefaultAddr();
    uint32_t error = regs->err_code;

    kprintf("\n\n PAGEFAULT \n - Error -> %x\n - Address -> %x\n", error, address);
    Kernel_panic(NULL);
}

/*  Disable 4MB pages
    -   By default, FantomOS boots with a pageDir with 4MB pages (loader.s)
        This removes that allowing for 4KB pages, reducing fragmentation
        (wasted space)
*/
void VirtMem_disablePSE()
{
    uint32_t cr4, cr0;
    __asm__ volatile("mov %%cr4, %0" : "=r"(cr4));
    cr4 = cr4 & 0xffffffef;
    __asm__ volatile("mov %0, %%cr4" :: "r"(cr4));
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 = cr0 | 0x80000000;
    __asm__ volatile("mov %0, %%cr0" :: "r"(cr0));
    PageSizeExtension_enabled = false;
}

/*  Invalidate Translation Lookaside Buffer Entry
    -   Makes sure the OS checks the page entry for the memory address next
        time it is desired. Should be done after unmapping a page.
*/
void VirtMem_invalidateTLBE(uint32_t vaddress)
{
    __asm__ ("invlpg (%0)" :: "r" (vaddress) : "memory" );
}

/*  Map to Current
    -   Maps a page to the current DIR
        This allows for dir manipulation without disabling paging in the
        process
        Returns the tempoary vaddr
*/
uint32_t *VirtMem_mapToCurrent(uint32_t paddr, uint8_t page)
{
    if(PageSizeExtension_enabled)
    {
        if(paddr > 0x400000)
            Kernel_panic("Disable PSE and switch to new KDIR before using paging!");
        return paddr + KBASE_VADDRESS;
    }

    //This temp page will sit at the bottom of the kernel memory
    // This allows us to assume there is already a dir_entry here and it
    // has all the right config

    //RIP our in memory MB header
    uint32_t vaddr = TEMP_VADDR;
    page_table_t *pt = (page_table_t *)( (uint32_t)0xFFC00000 +  (0x4 * ((vaddr) >> 12)));
    vaddr = TEMP_VADDR + (page * PAGE_SIZE);
    page_table_entry_t *pte = &pt->entry[ vaddr >> 12 & 0x03FF];

    VirtMem_initPTE(pte, paddr, READ_WRITE, KERNEL_MODE);

    VirtMem_invalidateTLBE(vaddr);
    return vaddr;
}

/*  Init a Dir Entry
    -   Takes the properties of the dir e and sets them
        As stated in OSDev Wiki, these functions can be really helpfull for readablity in later funcitons
*/
void VirtMem_initPDE(page_dir_entry_t *dir_e, page_table_t *table, uint8_t readWrite, uint8_t priv)
{
    //Always clear as a basic level of security!!!
    memset(dir_e, 0, sizeof(page_dir_entry_t));
    dir_e->page_address = ((uint32_t)table) >> 12;
    dir_e->present = 0x1;
    dir_e->read_write_flag = readWrite;
    dir_e->mode_flag = priv;
}

/*  Init a Table Entry
    -   Takes the properties of the table e and sets them
        As stated in OSDev Wiki, these functions can be really helpfull for readablity in later funcitons
*/
void VirtMem_initPTE(page_table_entry_t *table_e, uint32_t address, uint8_t readWrite, uint8_t priv)
{
    //Always clear as a basic level of security!!!
    memset(table_e, 0, sizeof(page_table_entry_t));
    table_e->page_address = ((uint32_t)address) >> 12;
    table_e->present = 0x1;
    table_e->read_write_flag = readWrite;
    table_e->mode_flag = priv;
}

/*  get Mapping
    -   given a virtual address, what is its physical address in a given dir
*/
uint32_t VirtMem_virt2Phys(page_dir_t *dir, uint32_t vaddr)
{
    //Map in the correct DIR temp
    page_dir_t *dir_v = VirtMem_mapToCurrent(dir, 0);
    page_dir_entry_t *dir_e = &dir_v->entry[vaddr >> 22];

    //Is it mapped
    if(!dir_e->present)
        return 0;

    //Map the table
    page_table_t *table_v = VirtMem_mapToCurrent((dir_e->page_address << 12), 0);
    page_table_entry_t *table_e = &table_v->entry[vaddr >> 12 & 0x03FF];

    //Is it mapped
    if(!table_e->present)
        return 0;

    return (uint32_t)table_e->page_address << 12;
}

/*  Create Dir
    -   Makes a dir at a given paddress.
        Fractal maps it into its last entry. Allows for temp Mapping and not
        disabling paging each time.
*/
page_dir_t *VirtMem_createDir(uint32_t paddr)
{
    //Get Physical Space
    if(paddr == 0)
        paddr = (uint32_t)PhysMem_allocate();

    //Virtual Dir (Temp)
    page_dir_t *virtual_dir = VirtMem_mapToCurrent(paddr, 0);

    //Clear the dir
    memset(virtual_dir, 0, sizeof(page_dir_t));

    //Fractal Mapping (Recursive)
    virtual_dir->entry[1023].present = 0x1;
    virtual_dir->entry[1023].read_write_flag = READ_WRITE;
    virtual_dir->entry[1023].mode_flag = KERNEL_MODE;
    virtual_dir->entry[1023].page_address = paddr >> 12;

    return (page_dir_t *)paddr;
}

/*  Free Dir
    -   Dumps the dir of a typical higher half process
*/
void VirtMem_freeDir(page_dir_t * dir)
{
    /*page_dir_t *vdir = VirtMem_mapToCurrent(dir, 2);

    //User Code -> Kernel Base
    for(uint32_t m = 0 ; m < KBASE_VADDRESS ; m++)
    {
        if(vdir->entry[m].present == 0x1)
        {
            page_table_t *vtab = VirtMem_mapToCurrent((vdir->entry[m].page_address) << 12, 4);
            for(uint32_t te = 0 ; te < 1024 ; te++)
            {
                if(vtab->entry[te].present == 0x1)
                {
                    uint32_t phys = (vtab->entry[te].page_address) << 12;
                    if(phys > 0)
                        PhysMem_free(phys);
                }
            }

        }
    }*/
}

/*  Map Page
    -   Maps a page to a given dir
        If processing, could just pass the process
*/
uint32_t VirtMem_mapPage(page_dir_t *dir, uint32_t vaddr, uint32_t paddr, uint8_t readWrite, uint8_t priv)
{
    if(paddr == 0)
        paddr = PhysMem_allocate();

    //Temp map the dir
    page_dir_t *dir_v = VirtMem_mapToCurrent(dir, 0);
    page_dir_entry_t *dir_e = &dir_v->entry[vaddr >> 22];

    //Does dir entry exist in memory?
    if(!dir_e->present)
    {
        page_table_t *pt = PhysMem_allocate();
        VirtMem_initPDE(dir_e, pt, readWrite, priv);
        pt = VirtMem_mapToCurrent(pt, 4);
        memset(pt, 0, sizeof(page_table_entry_t));
    }

    //Temp Map Table
    page_table_t *table_v = VirtMem_mapToCurrent((dir_e->page_address << 12), 0);
    page_table_entry_t *table_e = &table_v->entry[vaddr >> 12 & 0x03FF];

    //kprintf("MP: DIR->%x VA->%x, PA->%x\n", dir, vaddr, paddr);

    //Dont check if anything is here already, if there is, override it
    VirtMem_initPTE(table_e, paddr, readWrite, priv);
    VirtMem_invalidateTLBE(vaddr);

    return paddr;
}

/*  UnMap Page
    -   Removes a page from its given dir
        Also frees the physical memory there! (this might not be wanted in some systems)
*/
void VirtMem_unmapPage(page_dir_t *dir, uint32_t vaddr, bool free_phys)
{
    //Temp map the dir
    page_dir_t *dir_v = VirtMem_mapToCurrent(dir, 0);
    page_dir_entry_t *dir_e = &dir_v->entry[vaddr >> 22];

    if(!dir_e->present)
        return;

    //Temp Map Table
    page_table_t *table_v = VirtMem_mapToCurrent((dir_e->page_address << 12), 0);
    page_table_entry_t *table_e = &table_v->entry[vaddr >> 12 & 0x03FF];

    if(!table_e->present)
        return;

    if(free_phys)
        PhysMem_free(table_e->page_address << 12);

    memset(table_e, 0, sizeof(page_table_entry_t));
    VirtMem_invalidateTLBE(vaddr);
}

/*  Copy Dir
    -   Copies the kernel pages of the kdir to a new dir
        TODO: copy non kernel pages if desired
*/
void VirtMem_cpyDir(page_dir_t *srcDir, page_dir_t *dstDir, bool user)
{
    //Temp map both DIRs
    page_dir_t *srcDir_v = VirtMem_mapToCurrent(srcDir, 4);
    page_dir_t *dstDir_v = VirtMem_mapToCurrent(dstDir, 0);

    ///
    page_dir_entry_t *e = &dstDir_v->entry[KBASE_VADDRESS >> 22];
    memset(e, 0, sizeof(page_dir_entry_t));
    e->page_address = srcDir_v->entry[KBASE_VADDRESS >> 22].page_address;
    e->present = 0x1;
    e->mode_flag = KERNEL_MODE;
    e->read_write_flag = READ_WRITE;

    for(uint32_t heap = (KERNEL_HEAP_BASE >> 22) ; heap < ((KERNEL_HEAP_BASE + KERNEL_HEAP_SIZE_MAX) >> 22) ; heap++)
    {
        e = &dstDir_v->entry[heap];
        memset(e, 0, sizeof(page_dir_entry_t));

        e->page_address = srcDir_v->entry[heap].page_address;
        e->present = 0x1;
        e->read_write_flag = READ_WRITE;
        e->mode_flag = KERNEL_MODE;

    }
}

/*  Get KDIR
    -   Returns the dir for the kernel process (base)
*/
page_dir_t *VirtMem_getKDIR()
{
    return kdir;
}

/*  INIT
    -   Sets up paging on FantomOS
*/
bool VirtMem_Paging_init()
{
    //As PSE is enable by deafult in loader.s
    PageSizeExtension_enabled = true;

    //Map in the PageFault Handler
    //NOTE: !! Dependancy -> IDT (inc in default mods)
    Interrupts_addISR(14, &VirtMem_handlePageFault);

    //Map in the Kernel Dir's first 4MB (kernel phys space)
    uint32_t vaddr = KBASE_VADDRESS;
    kdir = VirtMem_createDir(0);
    while(vaddr < KEND_VADDRESS)
    {
        VirtMem_mapPage(kdir, vaddr, vaddr - KBASE_VADDRESS, READ_WRITE, KERNEL_MODE);
        vaddr += PAGE_SIZE;
    }

    //Change to new DIR
    VirtMem_changeDir(kdir);

    //Disable PSE
    VirtMem_disablePSE();

    sysInfo.virtualMemMgmt_enabled = true;
    return true;

}
