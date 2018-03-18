
//ME
#include <modules/drivers/Keyboard.h>

//REQUIREMENTS
#include <modules/heapmem_sbrk/HeapMem_SBRK.h>
#include <modules/processes/Scheduler_RR.h>
#include <modules/processes/Process.h>

//CORE
#include <core/debug/Common.h>
#include <core/drivers/PS2.h>
#include <core/boot/Kernel.h>
#include <core/debug/Debug.h>
#include <core/interfaces/Interrupts.h>
#include <core/interfaces/Memory.h>

pcb_t *blocking = NULL;

bool Keyboard_init()
{
    /*keyboard_info.caps = false;
    keyboard_info.shift = false;*/

    /*PS2_send(PS2_DATA, 0xF0);
    PS2_send(PS2_DATA, 1);*/
    kbBuffer = Keyboard_getNewBuffer();

    //blockedProcesses = calloc(1, sizeof(uint32_t) * MAX_KEYBLOCKED);
    //blockedCount = 0;

    Interrupts_addIRQ(1, &kb_handler);

    return true;
}

keybuffer_t *Keyboard_getNewBuffer()
{
    if(kbBuffer != 0)
        return kbBuffer;

    keybuffer_t *kbBufferN = calloc(1, sizeof(keybuffer_t));

    kbBufferN->s = calloc(1, (KEYBUFFER_MAX * sizeof(char)));
    kbBufferN->e = kbBufferN->s + KEYBUFFER_MAX;
    kbBufferN->r = kbBufferN->s;
    kbBufferN->w = kbBufferN->s;

    //kprintf("Write node head -> %x\n", kbBuffer->w);

    kbBufferN->curr_elements = 0;
}

char Keyboard_bufferRead()
{
    //keybuffer_t *kbBuffer = Scheduler_getCurrent()->keybuffer;


    if(kbBuffer->curr_elements == 0)
        return NULL;

    //kprintf("Key Read\n");

    char ret = *kbBuffer->r;
    kbBuffer->r++;

    kbBuffer->curr_elements--;

    if(kbBuffer->r == kbBuffer->e)
        kbBuffer->r = kbBuffer->s;

    return ret;
}

void Keyboard_bufferWrite(char key)
{
    //kprintf("Key Added\n");
    //keybuffer_t *kbBuffer = Scheduler_getCurrent()->keybuffer;

    *kbBuffer->w = key;
    kbBuffer->w++;
    if(kbBuffer->w == kbBuffer->e)
        kbBuffer->w = kbBuffer->s;

    kbBuffer->curr_elements++;
}

void kb_handler(Registers *regs)
{
    //kprintf("Keyboard Handler Called\n");
    uint8_t code = PS2_rec(PS2_DATA) - 128;

    if(code == 0xAA)
    {
        keyboard_info.shift = true;
    }
    else if(code == 0x2A)
    {
        keyboard_info.shift = false;
    }
    else if(!((code) & 0x80))
    {
        if(keyboard_info.shift == true)
            Keyboard_bufferWrite(layout_us_shift[code]);
        else
            Keyboard_bufferWrite(layout_us[code]);
        //kprintf("Key -> %x\n", code);
        //if(blocking != NULL)
        if(blocking != NULL)
        {
            if(Scheduler_getCurrent() == blocking) {
                blocking->state = PROCESS_STATE_WAIT;
                blocking = NULL;
            }
        }
    }
}

char Keyboard_getInput()
{
    char key = Keyboard_bufferRead();
    while(key == NULL)
    {
        Process_blockProcess();
        blocking = Scheduler_getCurrent();
        //kprintf("Now Blocking Process -> %d\n",((pcb_t *)(blockedProcesses[blockedCount]))->id);
        key = Keyboard_bufferRead();
    }
    return key;

}
