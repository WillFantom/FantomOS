#ifndef __GDT_H
#define __GDT_H

#include <core/Common.h>

#define GDT_ENTRIES 6

struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
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
