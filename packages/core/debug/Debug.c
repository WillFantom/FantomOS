/*  - Debug.c
    AUTHOR: Will Fantom
    DESC:   Allows for printing in a simple manner to the serail port or vga
            if the dirver is added as a module.
            Also provides other functions that will be handy in the creation
            of modules.
*/

#include <core/debug/Debug.h>
#include <core/debug/Common.h>
#include <core/boot/Kernel.h>
#include <stdarg.h>
#include <core/drivers/Serial.h>

#if USE_VGA
    #include <modules/drivers/VGA.h>
#endif

// Put char to whereever can take it
void Debug_putChar(char c)
{
    if(sysInfo.serial_enabled == true)
        Serial_putChar(c, 1);

    #if USE_VGA
        if(sysInfo.video_enabled == true && USE_VGA)
        {
            VGA_putChar(c);
        }
    #endif

}

// Put string to whereever can take it
void Debug_putStr(char *s)
{
    if(sysInfo.serial_enabled == true)
        Serial_putStr(s, 1);

    #if USE_VGA
        if(sysInfo.video_enabled == true && USE_VGA)
            VGA_putStr(s);
    #endif

}

//Convert a number to its string equiv
// many examples of this function can be found online
char *iota(char *buffer, uint32_t value, uint8_t base)
{
    static char *values = "0123456789ABCDEF";
    unsigned char bufferpos = 0;

    if(value == 0 || (base < 1 || base > 16))
    {   //Handle 0 or invalid base
        buffer[bufferpos] = '0';
        buffer[++bufferpos] = '\0';
        return buffer;
    }

    if(value < 0 && base == 10)
        value = -value;

    while(value > 0)
    {   //Change to base value

        buffer[bufferpos++] = values[value % base];
        value = (value / base);
    }

    if(value < 0 && base == 10)
        buffer[bufferpos++] = "-";

    char temp = 'N';
    for(char c = 0 ; c < bufferpos/2 ; c++)
    {   //Reverse the string
        temp = buffer[c];
        buffer[c] = buffer[bufferpos - 1 - c];
        buffer[bufferpos - 1 - c] = temp;
    }

    buffer[bufferpos] = '\0';
    return buffer;
}

// Offer a simple print fucntion for both display and com
void kprintf(const char *message, ...)
{
    va_list args;
    va_start(args, message);

    char *buffer[48];

    while(*message != '\0')
    {
        if(*message == '%')
        {
            switch(*(++message))
            {
                case 'X' :
                case 'x' :
                    Debug_putStr(iota(buffer, va_arg(args, int), 16));
                    memset(buffer, 0, sizeof(char) * 16);
                    break;
                case 'D' :
                case 'd' :
                    Debug_putStr(iota(buffer, va_arg(args, int), 10));
                    memset(buffer, 0, sizeof(char) * 16);
                    break;
                case 'C' :
                case 'c' :
                    Debug_putChar(va_arg(args, char));
                    break;
                case 'S' :
                case 's' :
                    Debug_putStr(va_arg(args, char*));
                    break;
            }
            message++;
        }
        else
        {
            Debug_putChar(*message++);
        }

    }

    va_end(args);
}
