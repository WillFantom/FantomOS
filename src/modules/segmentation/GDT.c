/*  - GDT.c
    AUTHOR: Will Fantom
    DESC:   Segments the memory TODO: Addd comments in here
*/

#include <modules/GDT.h>

//REQUIREMENT FOR TSS ONLY
#include <modules/Process.h>

#include <core/Common.h>
#include <core/Memory.h>
#include <core/Debug.h>

void GDT_set(uint32_t entry, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran)
{
    gdt[entry].base_low = (base & 0xFFFF);
    gdt[entry].base_middle = (base >> 16) & 0xFF;
    gdt[entry].base_high = (base >> 24) & 0xFF;
    gdt[entry].limit_low = (limit & 0xFFFF);
    gdt[entry].granularity = ((limit >> 16) & 0x0F);
    gdt[entry].granularity |= (gran & 0xF0);
    gdt[entry].access = access;
}

void TSS_init(uint64_t base, uint64_t limit, uint8_t access, uint8_t gran)
{
    memset(&tss_e, 0, sizeof(struct tss_entry));
    GDT_set(5, base, limit, access, gran);
    tss_e.cs = 0x0b;
    tss_e.ds = 0x13;
    tss_e.es = 0x13;
    tss_e.fs = 0x13;
    tss_e.gs = 0x13;
    tss_e.ss = 0x13;
}

/*void TSS_flush()
{
    __asm__ volatile("ltr %w0" : : "r" (SEG_TSS | 3));
}*/

bool GDT_init()
{
    //Info for the CPU
    gdt_p.limit = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1;
    gdt_p.base = &gdt;

    //Set default segments for x86 ARCH DEP
    GDT_set(0, 0, 0, 0, 0); //nulls
    GDT_set(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); //ring 0 code
    GDT_set(2, 0, 0xFFFFFFFF, 0x92, 0xCF); //ring 0 data
    GDT_set(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); //ring 3 code
    GDT_set(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); //ring 3 data

    //Hardware Task Switching
    TSS_init((uint64_t)&tss_e, (uint64_t)&tss_e + sizeof(struct tss_entry), 0xE9, 0x00);
    Process_setTSS(SEG_KDATA, 0);

    //Flush
    GDT_flush((uint64_t *)&gdt_p);
    TSS_flush();

    return true;
}
