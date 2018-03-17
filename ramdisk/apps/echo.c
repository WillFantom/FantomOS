#include <syscall.h>
#include <stdio.h>
#include <string.h>

void main()
{
    cls();
    makeForegroundProcess();

	putStr("Keyboard Echoer\n");
    putStr("Type 'end' to go back\n");

    char inputBuffer[128];

    while(1)
    {
        char *in = gets(inputBuffer);
        if(strcmp(in, "end") == 0)
            suicide();
        putStr(in);
        putChar('\n');
    }

    suicide();
}
