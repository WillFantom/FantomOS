/*
 *  Title:  Package Manager
 *  Desc:   This detrmines which packages the kernel will load on boot
 *  Author: Will Fantom
 */
#include <core/MODULES/Manager.h>

#include <core/boot/Kernel.h>
#include <core/debug/Common.h>

//INCLUDE THE PACKAGE HERE
#include <core/drivers/Serial.h>
#include <core/drivers/PS2.h>
#include <core/drivers/PIC.h>
#include <core/drivers/PIT.h>

#include <modules/drivers/VGA.h>
#include <modules/segmentation/GDT.h>
#include <modules/interrupts/IDT.h>
#include <modules/physmem_bitmap/PhysMem_Bitmap.h>
#include <modules/physmem_hybrid/PhysMem_Hybrid.h>
#include <modules/virtmem_paging/VirtMem_Paging.h>
#include <modules/heapmem_sbrk/HeapMem_SBRK.h>

#include <modules/vfs/VFS.h>
#include <modules/ramdisk/RamDisk.h>

#include <modules/drivers/Keyboard.h>

#include <modules/processes/Scheduler_RR.h>
#include <modules/processes/Process.h>

#include <modules/userspace/User.h>

//Adds modules to array
void Manager_addModules()
{
    modCount = 0;

    //Output
    KMod_init[modCount++] = &Serial_init;
    KMod_init[modCount++] = &VGA_init;

    //Descriptor Tables
    KMod_init[modCount++] = &GDT_init;
    KMod_init[modCount++] = &IDT_init;

    //Devices
    KMod_init[modCount++] = &PIC_init;
    KMod_init[modCount++] = &PIT_init;
    KMod_init[modCount++] = &PS2_init;

    //Memory
    //KMod_init[modCount++] = &PhysMem_Bitmap_init;
    KMod_init[modCount++] = &PhysMem_Hybrid_init;
    KMod_init[modCount++] = &VirtMem_Paging_init;
    KMod_init[modCount++] = &HeapMem_SBRK_init;

    //File System
    KMod_init[modCount++] = &VFS_init;
    KMod_init[modCount++] = &RamDisk_init;

    //Input
    KMod_init[modCount++] = &Keyboard_init;

    //Processes
    KMod_init[modCount++] = &Scheduler_RR_init;
    KMod_init[modCount++] = &Process_init;

    //User Space
    KMod_init[modCount++] = &User_switchTo;
}

//Calls function pointed to in array
//If fails, returns false
bool Manager_loadModule(uint32_t modNum)
{
    if(modNum >= modCount)
        return false;

    if(!KMod_init[modNum]())
        return false;

    return true;
}
