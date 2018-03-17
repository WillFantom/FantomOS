#ifndef __SYSCALL_H
#define __SYSCALL_H

#include <core/debug/Common.h>
#include <core/interfaces/Memory.h>

#define SYSCALL_INTNUM 128
#define SYSCALL_COUNT 16

void Syscall_handle(Registers *regs);

#endif
