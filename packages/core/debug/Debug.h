#ifndef __DEBUG_H
#define __DEBUG_H

#include <core/boot/Kernel.h>
#include <core/debug/Common.h>
#include <stdarg.h>

#define USE_VGA true

#define KASSERT(X) if(!(X)) {Kernel_panic("Condition Failed Assertion");}

void kprintf(const char *message, ...);

#endif
