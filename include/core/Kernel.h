#ifndef __KERNEL_H
#define __KERNEL_H

#include <core/Common.h>
#include <core/multiboot.h>

//System Information
typedef struct SystemInfo
{
    char        OSname[48];
    char        OSversion[8];
    bool        serial_enabled;
    bool        video_enabled;
    bool        interrupts_enabled;
    bool        gdt_enabled;
    bool        physicalMemMgmt_enabled;
    bool        virtualMemMgmt_enabled;
    bool        heapMemMgmt_enabled;
    bool        scheduler_enabled;
    bool        userspace_enabled;
} SystemInfo_t;

SystemInfo_t sysInfo;
void printSystemInfo();

//Modules
#define MAX_KERNEL_MODULES 96
uint8_t modCount;
bool (*KMod_init[MAX_KERNEL_MODULES])();

//Multiboot
#define MAX_MB_MODS 5
multiboot_info_t *mb;
uint32_t mb_mods[MAX_MB_MODS];
uint32_t kernel_end;

//Base Fucntions
void kmain(multiboot_info_t *mb_info, uint32_t k_end, uint32_t magic);
void Kernel_halt();
void Kernel_panic();

#endif
