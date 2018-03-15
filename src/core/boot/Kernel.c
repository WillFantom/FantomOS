/*  - Kernel.c
    AUTHOR: Will Fantom
    DESC:   Loads in all the Kernel modules. Called from loader.s linked to
            the function kmain(...).
    note:   If developing a module for the kernel, this is where it should be
            booted.
            This is mapped to the address specified in KBASE_VADDRESS via
            paging. Therefore, any memory managment sould be done with paging
            in mind, or disable paging via the CR0 reg to use direct memory
            addressing.
            The PSE bit is also set, so any paging will have to use 4MB pages
            unless the PSE bit is disabled.
            The common include should remain the same for the data types to
            be defined.
            If creating a Video Driver that allows for printing text to the
            screen, a function should be made called print. This should act like
            printf allowing for extra args. One such type should be %s
            so that strings can be printed. If this is done, set Video_enabled to
            true allowing for better debug statements within the kernel.
            DO NOT remove or edit the Drivers.c/h or Serial.c/h as serial port
            debugging is a requirement of FantomOS.

            Further info on module creation can be found in the GitHub ReadMe
*/

#include <core/Kernel.h>
#include <core/multiboot.h>
#include <core/Common.h>

#include <core/Debug.h>
#include <core/Memory.h>
#include <core/Interrupts.h>

#include <core/Serial.h>
#include <core/PS2.h>
#include <core/PIC.h>
#include <core/PIT.h>

#include <modules/VGA.h>
#include <modules/GDT.h>
#include <modules/IDT.h>
#include <modules/PhysMem_Bitmap.h>
#include <modules/PhysMem_Hybrid.h>
#include <modules/VirtMem_Paging.h>
#include <modules/HeapMem_SBRK.h>

#include <modules/VFS.h>
#include <modules/RamDisk.h>

#include <modules/Keyboard.h>

#include <modules/Scheduler_RR.h>
#include <modules/Process.h>

#include <modules/User.h>

void Kernel_halt()
{
    while(1)
        halt_cpu();
    while(1);
}

bool Kernel_initModule(char *name, bool (*function)())
{
    kprintf("Loading Module -> %s\n", name);

    return (function)();
}

void Kernel_panic(char *message)
{

    if(message != NULL)
        kprintf("Message -> %s\n", message);

    kprintf("\n -- Kernel Panic -- \n");
    Kernel_halt();
}

bool Kernel_validMagic(uint32_t magic)
{
    if(magic == MULTIBOOT_BOOTLOADER_MAGIC)
        return true;
    return false;
}

void Kernel_getMBMods(multiboot_info_t *mb_info)
{
    //uint32_t mods[mb_info->mods_count];
    //mb_mods = &mods;
    for(uint32_t mod = 0 ; mod < mb_info->mods_count ; mod++)
        mb_mods[mod] = (uint32_t)((multiboot_module_t *)(mb_info->mods_addr + KBASE_VADDRESS))[mod].mod_start;
}

void Kernel_addKenrelMods()
{
    modCount = 0;
    KMod_init[modCount++] = &Serial_init;
    KMod_init[modCount++] = &VGA_init;

    KMod_init[modCount++] = &GDT_init;
    KMod_init[modCount++] = &IDT_init;

    KMod_init[modCount++] = &PIC_init;
    KMod_init[modCount++] = &PIT_init;
    KMod_init[modCount++] = &PS2_init;

    //KMod_init[modCount++] = &PhysMem_Bitmap_init;
    KMod_init[modCount++] = &PhysMem_Hybrid_init;
    KMod_init[modCount++] = &VirtMem_Paging_init;
    KMod_init[modCount++] = &HeapMem_SBRK_init;

    KMod_init[modCount++] = &VFS_init;
    KMod_init[modCount++] = &RamDisk_init;

    KMod_init[modCount++] = &Keyboard_init;

    KMod_init[modCount++] = &Scheduler_RR_init;
    KMod_init[modCount++] = &Process_init;

    KMod_init[modCount++] = &User_switchTo;
}

bool Kernel_initMods()
{
    interrupts_dis();
    for(uint8_t m = 0 ; m < modCount ; m++)
    {
        if(!KMod_init[m]())
            return false;
    }
    interrupts_enb();
    return true;
}

void printSystemInfo()
{
    kprintf("\n#################\n");
    kprintf("## SYSTEM INFO ##\n");
    kprintf("# OS Name -> %s\n", sysInfo.OSname);
    kprintf("# OS Version -> %s\n", sysInfo.OSversion);
    kprintf("# Serail OK? -> %d\n", sysInfo.serial_enabled);
    kprintf("# Video OK? -> %d\n", sysInfo.video_enabled);
    kprintf("# Ints OK? -> %d\n", sysInfo.interrupts_enabled);
    kprintf("# GDT OK? -> %d\n", sysInfo.gdt_enabled);
    kprintf("# Phys Mem OK? -> %d\n", sysInfo.physicalMemMgmt_enabled);
    kprintf("# Virt Mem OK? -> %d\n", sysInfo.virtualMemMgmt_enabled);
    kprintf("# Heap Mem OK? -> %d\n", sysInfo.heapMemMgmt_enabled);
    kprintf("# Scheduler OK? -> %d\n", sysInfo.scheduler_enabled);
    kprintf("# Scheduler OK? -> %d\n", sysInfo.scheduler_enabled);
    kprintf("# Userspace OK? -> %d\n", sysInfo.userspace_enabled);
    kprintf("#################\n\n");
}

void kmain(multiboot_info_t *mb_info, uint32_t k_end, uint32_t magic)
{

    mb = mb_info;
    kernel_end = k_end;
    /*  - MB Magic Check
        Check to see if we got the correct magic number in the bootlaoder
        (grub)
    */
    if(!Kernel_validMagic(magic))
        Kernel_panic(NULL);


    /*  - Boot Modules
        Modules such as the ramdisk are loaded into memory just after the
        kenrel. The start address of these mods are needed to be able to load
        them in later.
    */
    Kernel_getMBMods(mb_info);

    /*  - Add Kernel Modules
        Add in the kernel modules in order that you desire them to be loaded
        in (array index). Each modules *must* have init fuction that returns
        true (1) or false (0). This is the function that'll be called by the
        initModules function.''
        Think about the dependacies of each module to determine the ordering.
    */
    Kernel_addKenrelMods();

    /*  - Init Kernel Modules
        This is where all previous modules are loaded in. They should not
        require params! The may access information that can be found in this
        file such as the mb_info or the kernel end physical address.
        Returns FAIL and panics if any 1 module fails.
        If a USERSPACE module is used that loads a userspace app binary, it may
        be the case that the end of this is never reached. Therfore, it is best
        to load that module last.
    */
    if(!Kernel_initMods())
        Kernel_panic("1 or more modules failed to load");

    kprintf("Kernel End Reached\n");
    //printSystemInfo();
    Kernel_halt();
}
