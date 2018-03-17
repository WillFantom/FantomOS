/*  - VGA.c
    AUTHOR: Will Fantom
    DESC:   Allows for writing to a display via the framebuffer
            Only spoorts base VGA, no extended video modes
*/

#include <core/boot/Kernel.h>
#include <core/interfaces/Memory.h>
#include <core/debug/Common.h>
#include <core/debug/Debug.h>

#include <modules/drivers/VGA.h>

uint16_t *VGA_FRAMEBUFFER = (uint16_t *)(0xB8000 + KBASE_VADDRESS);

bool VGA_init()
{
    VGA_cls();
    VGA_standardColour();
    sysInfo.video_enabled = true;
}

void VGA_update()
{
    unsigned int location = VGA_y * VGA_WIDTH + VGA_x;

    outb(0x3D4, 14);
    outb(0x3D5, location >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, location);
}

void VGA_cls()
{
    VGA_x = 0;
    VGA_y = 0;
    VGA_standardColour();

    for(uint32_t y=0 ; y<VGA_HEIGHT ; y++)
    {
        for(uint32_t x=0 ; x<VGA_WIDTH ; x++)
        {
            unsigned int part = y * VGA_WIDTH + x;
            //VGA_FRAMEBUFFER[part] = 0;
            VGA_FRAMEBUFFER[part] = ' ' | VGA_colour << 8;
            //VGA_update();
        }
    }
}

void VGA_scroll()
{
    if(VGA_y>=VGA_HEIGHT)
    {
        for(int p=0 ; p<((VGA_HEIGHT - 1)*VGA_WIDTH) ; p++)
            VGA_FRAMEBUFFER[p] = VGA_FRAMEBUFFER[p+VGA_WIDTH];
        for(int p=((VGA_HEIGHT - 1)*VGA_WIDTH) ; p<(VGA_HEIGHT*VGA_WIDTH) ; p++)
            VGA_FRAMEBUFFER[p] = ' ' | VGA_colour << 8;
        VGA_y = VGA_HEIGHT - 1;
    }
}

void VGA_putChar(char c)
{
    unsigned int part = VGA_y*VGA_WIDTH + VGA_x;

    switch(c)
    {
    case '\r' : VGA_x=0; break;
    case '\b' : VGA_x--; break;
    case '\t' : VGA_x+=8 & ~(8-1); break;
    case '\n' : VGA_y++;
                VGA_x=0;
                VGA_scroll();
                break;
    default : VGA_FRAMEBUFFER[part] = c | VGA_colour << 8;
              VGA_x++;
              break;
    }

    if(VGA_x>=VGA_WIDTH)
    {
        VGA_x = 0;
        VGA_y++;
        VGA_scroll();
    }

    VGA_update();
}

void VGA_putStr(char *s)
{
    while(*s)
        VGA_putChar(*s++);
}

void VGA_colourError()
{
    VGA_colour = 15 | 4 << 4;
}

void VGA_standardColour()
{
    VGA_colour = 15 | 0 << 4;
}
