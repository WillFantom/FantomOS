/*  - PIC.c
    AUTHOR: Will Fantom
    DESC:   Programmable Interrupt Controller
            Used in FantomOS for hadnling the End Of Interrupt Routine
*/

#include <core/PIC.h>
#include <core/Common.h>

bool PIC_init()
{

    //Init Start
    outb(PIC_MASTER + PIC_COMMAND, 0x11);
    outb(PIC_SLAVE + PIC_COMMAND, 0x11);

    //Vector Offset
    outb(PIC_MASTER + PIC_DATA, 0x20);
    outb(PIC_SLAVE + PIC_DATA, 0x28);

    //Inform Master of Slave
    outb(PIC_MASTER + PIC_DATA, 0x04);

    //Cascade Identity
    outb(PIC_SLAVE + PIC_DATA, 0x02);

    //Mode (8086)
    outb(PIC_MASTER + PIC_DATA, 0x01);
    outb(PIC_SLAVE + PIC_DATA, 0x01);

    //No Mask
    outb(PIC_MASTER + PIC_DATA, 0x00);
    outb(PIC_SLAVE + PIC_DATA, 0x00);

    return true;

}

void PIC_eoi(uint8_t int_num)
{
    if(int_num > 39)
        outb(PIC_SLAVE + PIC_COMMAND, PIC_EOI);

    outb(PIC_MASTER + PIC_COMMAND, PIC_EOI);
}
