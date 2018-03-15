#include <syscall.h>
#include <file.h>



static int syscall(int type, int p1, int p2, int p3, int p4, int p5)
{
    int ret;
    __asm__ volatile("int $0x80" : "=a" (ret) : "0" (type), "b" ((int)p1), "c" ((int)p2),
            "d" ((int)p3), "S" ((int)p4), "D" ((int)p5));
    return ret;
}


unsigned int createChildProcess(FILE *file)
{
	unsigned int proc = syscall(SYS_CREATECHILD, (unsigned int)file->filename, (unsigned int)getCurrentProcessInfo(), 0, 0, 0);
    //switchToChild(getCurrentProcessInfo());
    return proc;
}

unsigned int getCurrentProcessInfo()
{
	return syscall(SYS_GETCURRENTPCB, 0, 0, 0, 0, 0);
}

void *sbrk(ptrdiff_t pages)
{
	return syscall(SYS_SBRK, pages, 0, 0, 0, 0);
}

unsigned int open(const char *file_name, const char mode)
{
	return syscall(SYS_FILEOPEN, file_name, mode, 0, 0, 0);
}
void read(FILE *node, char *dest, unsigned int size, unsigned int offset)
{
	syscall(SYS_FILEREAD, (unsigned int)node, dest, size, offset, 0);
}
void getFileInfo(unsigned int node, unsigned int dest)
{
	return syscall(SYS_FILESIZE, node, dest, 0, 0, 0);
}
char *getFileAtIndex(unsigned int idx)
{
	return syscall(SYS_FILEATINDEX, idx, 0, 0, 0, 0);
}

void putChar(char c)
{
	syscall(SYS_PUTCHAR, c, 0, 0, 0, 0);
}
void putStr(const char *str)
{
	syscall(SYS_PUTSTR, str, 0, 0, 0, 0);
}
void cls()
{
	syscall(SYS_CLS, 0, 0, 0, 0, 0);
}
void vgaError()
{
	syscall(SYS_VGAERROR, 0, 0, 0, 0, 0);
}
void vgaStd()
{
	syscall(SYS_VGASTD, 0, 0, 0, 0, 0);
}

char getInput()
{
	return syscall(SYS_KEYBOARDIN, 0, 0, 0, 0, 0);
}

void printTime()
{
	syscall(SYS_PRINTTIME, 0, 0, 0, 0, 0);
}

void makeForegroundProcess()
{
	syscall(SYS_MAKEFOREGROUND, 0, 0, 0, 0, 0);
    //for(int i = 0 ; i < 10000000 ; i++);
}

void killProcess(unsigned int proc)
{
    syscall(SYS_KILL, proc, 0, 0, 0, 0);
}

void suicide()
{
    syscall(SYS_KILL, getCurrentProcessInfo(), 0, 0, 0, 0);
}


void memcpy(void *dest, const void *src, unsigned int len)
{
    char *d = (char *)dest;
    const char *s = (const char *)src;
    while(len--)
    {
        *d++ =*s++;
    }
    return dest;
}

void memset(void* dest, unsigned char val, unsigned int len)
{
    unsigned char *d = (unsigned char *)dest;
    while(len > 0)
    {
        *d++ = val;
        len--;
    }
}


void empty()
{

}
