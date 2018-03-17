#ifndef __VGA_H
#define __VGA_H

#include <core/debug/Common.h>
#include <core/boot/Kernel.h>

//Static Qual
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

//Init
bool VGA_init();

//Funcs
void VGA_update();
void VGA_cls();
void VGA_scroll();
void VGA_putChar(char c);
void VGA_putStr(char *s);
void VGA_colourError(); //White on Red
void VGA_standardColour(); //White on Black

//Data
static unsigned int VGA_x = 0;
static unsigned int VGA_y = 0;
static unsigned char VGA_colour;

#endif
