#ifndef __KEYBOARD_H
#define __KEYBOARD_H

//REQUIREMENTS
#include <modules/heapmem_sbrk/HeapMem_SBRK.h>

//CORE
#include <core/debug/Common.h>
#include <core/interfaces/Memory.h>

//Buffer
#define KEYBUFFER_MAX 128
#define MAX_KEYBLOCKED 48
typedef struct KeyBuffer
{
    char*  s;
    char*  e;
    char*  r;
    char*  w;
    uint16_t curr_elements;
} keybuffer_t;
keybuffer_t *Keyboard_getNewBuffer();
uint32_t *blockedProcesses;
uint8_t blockedCount;
keybuffer_t *kbBuffer;

//CAPS / SHIFT
struct Keyboard_Info
{
    bool caps;
    bool shift;
};
struct Keyboard_Info keyboard_info;

bool Keyboard_init();
void kb_handler(Registers *regs);
char Keyboard_getInput();


//CODES
static char layout_us[128] =
{
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0,
    0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static  char layout_us_shift[128] =
{
    0, 27, '!', '"', 0, '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R',
    'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K',  'L', ':',
    '@', '~',  0, '~', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,/**/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

#endif
