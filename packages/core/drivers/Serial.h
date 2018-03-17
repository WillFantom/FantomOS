#ifndef __SERIAL_H
#define __SERIAL_H

#include <core/debug/Common.h>

#define PORT_COUNT 4

//init
bool Serial_init();

//funcs
void Serial_putChar(char c, uint8_t port);
void Serial_putStr(char *s, uint8_t port);

#endif
