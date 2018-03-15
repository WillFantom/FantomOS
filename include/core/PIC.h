#ifndef __PIC_H
#define __PIC_H

#include <core/Common.h>

#define PIC_MASTER 0x20
#define PIC_SLAVE 0xA0

#define PIC_COMMAND 0x00
#define PIC_DATA 0x01

#define PIC_EOI 0x20

//Init
bool PIC_init();

//Funcs
void PIC_eoi(uint8_t int_num);

#endif
