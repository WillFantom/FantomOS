#ifndef __SYSCALL_H
#define __SYSCALL_H

#include <core/Common.h>
#include <core/Memory.h>

#define SYSCALL_INTNUM 128
#define SYSCALL_COUNT 16

void Syscall_handle(Registers *regs);

#endif
