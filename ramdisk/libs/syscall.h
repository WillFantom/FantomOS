#ifndef __SYSCALL_H
#define __SYSCALL_H

#include <file.h>

typedef signed int ptrdiff_t;
#define NULL ((void *)0)
#define bool int
#define true 1
#define false 0

void empty();
void memcpy(void *dest, const void *src, unsigned int len);
void memset(void* dest, unsigned char val, unsigned int len);

//values
#define SYS_CREATECHILD 0
#define SYS_GETCURRENTPCB 1
#define SYS_SBRK 2
#define SYS_FILEOPEN 3
#define SYS_FILEREAD 4
#define SYS_FILESIZE 5
#define SYS_FILEATINDEX 6
#define SYS_PUTCHAR 7
#define SYS_PUTSTR 8
#define SYS_CLS 9
#define SYS_VGAERROR 10
#define SYS_VGASTD 11
#define SYS_KEYBOARDIN 12
#define SYS_PRINTTIME 13

#define SYS_MAKEFOREGROUND 14
#define SYS_KILL 15

//
unsigned int createChildProcess(FILE *file);
unsigned int getCurrentProcessInfo();

void *sbrk(ptrdiff_t pages);

unsigned int open(const char *file_name, const char mode);
void read(FILE *node, char *dest, unsigned int size, unsigned int offset);
void getFileInfo(unsigned int node, unsigned int dest);
char *getFileAtIndex(unsigned int idx);

void putChar(char c);
void putStr(const char *str);
void cls();
void vgaError();
void vgaStd();

char getInput();

void printTime();

void makeForegroundProcess();

void killProcess(unsigned int proc);
void suicide();

#endif
