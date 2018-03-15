/*  - Serial.c
    AUTHOR: Will Fantom
    DESC:   Sets up com port 1 for fata transmission.
            To be used for debugging and is required of FantomOS
*/

#include <core/Serial.h>
#include <core/Common.h>
#include <core/Debug.h>
#include <core/Kernel.h>

const unsigned short *com[4] = {0x03F8, 0x02F8, 0x03E8, 0x02E8};

bool Serial_init()
{
    uint8_t port = 1; //<-- Change the default port here
    KASSERT(port > 0 && port <= PORT_COUNT);
    outb((unsigned short)com[port] + 1, 0x00); //Disbale Interrupts
    outb((unsigned short)com[port] + 3, 0x80);
    outb((unsigned short)com[port] + 0, 0x03);
    outb((unsigned short)com[port] + 1, 0x00);
    outb((unsigned short)com[port] + 3, 0x03); //8bit
    outb((unsigned short)com[port] + 2, 0xC7);
    outb((unsigned short)com[port] + 4, 0x0B);

    sysInfo.serial_enabled = true;

    return true;
}

// Ensures the memory space is free
unsigned char Serial_transmitFree(uint8_t port)
{
    return inb(com[port-1] + 5) & 0x20;
}

// Writes a char over the com port
void Serial_putChar(char c, uint8_t port)
{
    KASSERT(port > 0 && port <= PORT_COUNT);
    while(!Serial_transmitFree(port));
    outb(com[port-1], c);
}

// Calls putChar to write until a null pointer
void Serial_putStr(char *s, uint8_t port)
{
    while(*s)
        Serial_putChar(*s++, port);
}
