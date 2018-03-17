#ifndef __PIT_H
#define __PIT_H

#include <core/debug/Common.h>

#define IN_HZ 1193180
#define OUT_HZ 100

#define PIT_PORT_0 0x40
#define PIT_PORT_1 0x41
#define PIT_PORT_2 0x42
#define PIT_PORT_CTRL 0x43

bool PIT_init();

#endif
