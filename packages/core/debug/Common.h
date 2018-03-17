#ifndef __COMMON_H
#define __COMMON_H

//TYPES
#define NULL ((void *)0)
#define bool int
#define true 1
#define false 0
typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef signed int ptrdiff_t;
typedef unsigned int size_t;

//SIZE
#define GIGABYTE 1073741824

//ASM Functions
extern void outb(unsigned short port, char data);
extern char inb(unsigned short port);
extern void interrupts_dis();
extern void interrupts_enb();
extern void halt_cpu();

//BIT
#define BIT_MASK(b) (1 << (b))
#define CLEARBIT(v, b) ((v) &= ~BIT_MASK(b))
#define SET_BIT(v, b) ((v) |= (BIT_MASK(b)))
#define GET_BIT_VALUE(v, b) (((v) & BIT_MASK(b)) ? 1 : 0)

//Functions
void empty();
void memcpy(void *dest, const void *src, unsigned int len);
void memset(void* dest, unsigned char val, unsigned int len);

#endif
