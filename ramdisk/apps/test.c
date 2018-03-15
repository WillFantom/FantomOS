#include <syscall.h>

void main()
{
    cls();
    makeForegroundProcess();

	putStr("This is a different userspace app!\n");

    suicide();
}
