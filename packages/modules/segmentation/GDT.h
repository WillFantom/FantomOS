#ifndef __GDT_H
#define __GDT_H

#include <core/debug/Common.h>

#define GDT_ENTRIES 6

struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct gdt_ptr gdt_p;
struct gdt_entry gdt[GDT_ENTRIES];

//struct tss_entry tss_e;

//Init
bool GDT_init();

//Funcs
void GDT_set(uint32_t entry, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran);
void GDT_flush(uint64_t *GDTptr);

#endif
