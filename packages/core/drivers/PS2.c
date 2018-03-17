/*  - PS2.c
    AUTHOR: Will Fantom
    DESC:   For use with PS2 devices such as keyboard and mouse.
*/

#include <core/drivers/PS2.h>
#include <core/debug/Common.h>

bool PS2_init()
{
    //Disable Devices
    /*PS2_send(PS2_STATE, PS2_DISABLE_KEYBOARD);
    PS2_send(PS2_STATE, PS2_DISABLE_MOUSE);

    //Flush Output Buffer
    PS2_rec(PS2_DATA);

    //Set Controller Config Byte
    PS2_send(PS2_STATE, PS2_CONFIG_READ);
    uint8_t config_byte = PS2_rec(PS2_DATA);
    config_byte = CLEARBIT(config_byte, 0);
    config_byte = CLEARBIT(config_byte, 1);
    config_byte = CLEARBIT(config_byte, 6);
    PS2_send(PS2_STATE, PS2_CONFIG_WRITE);
    PS2_send(PS2_DATA, config_byte);

    //Controller Self Test
    PS2_send(PS2_STATE, PS2_SELFTEST);
    if(PS2_rec(PS2_DATA) != PS2_SELFTEST_P)
        return false;

    //PORT KEY Test
    PS2_send(PS2_STATE, PS2_KEYBOARDTEST);
    if(PS2_rec(PS2_DATA) != 0x00)
        return false;

    //PORT MOUSE Test
    PS2_send(PS2_STATE, PS2_MOUSETEST);
    if(PS2_rec(PS2_DATA) != 0x00)
        return false;

    //Enable Devices
    PS2_send(PS2_STATE, PS2_ENABLE_KEYBOARD);
    PS2_send(PS2_STATE, PS2_ENABLE_MOUSE);

    PS2_send(PS2_DATA, 0xFF);
    PS2_send(PS2_DATA, 0xF4);
    PS2_send(PS2_DATA, 0xF0);
    //PS2_send(PS2_DATA, 1);

    PS2_enableInt();*/

    return true;

}

void PS2_enableInt()
{
    //ENB interrupts for both ports
    PS2_send(PS2_STATE, PS2_CONFIG_READ);
    uint8_t c = PS2_rec(PS2_DATA);
    c = c | 0b00000011;
    PS2_send(PS2_STATE, PS2_CONFIG_WRITE);
    PS2_send(PS2_DATA, c);
}

void PS2_inPoll()
{
    uint32_t c = 100000;
    while((inb(PS2_STATE) & 0x2) && c)
        c--;
}

void PS2_outPoll()
{
    uint32_t c = 100000;
    while(!(inb(PS2_STATE) & 0x1) && c)
        c--;
}

void PS2_send(uint8_t port, uint8_t cmd)
{
    PS2_inPoll();
    outb(port, cmd);
}

uint8_t PS2_rec(uint8_t port)
{
    PS2_outPoll();
    inb(port);
}
