#include <core/PIT.h>
#include <core/Common.h>

bool PIT_init()
{
    uint16_t div = IN_HZ / OUT_HZ;

    outb(PIT_PORT_CTRL, 0x36);
    outb(PIT_PORT_0, div);
    outb(PIT_PORT_0, div >> 8);

    return true;
}
