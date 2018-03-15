#ifndef __PS2_H
#define __PS2_H

#include <core/Common.h>

//Command Codes (FROM OSDEV Wiki)
#define PS2_SELFTEST 0xAA
#define PS2_SELFTEST_P 0x55
#define PS2_SELFTEST_F 0xFC
#define PS2_KEYBOARDTEST 0xAB
#define PS2_MOUSETEST 0xA9

//Toggles
#define PS2_DISABLE_KEYBOARD 0xAD
#define PS2_ENABLE_KEYBOARD 0xAE
#define PS2_DISABLE_MOUSE 0xA7
#define PS2_ENABLE_MOUSE 0xA8

//Device Config
#define PS2_CONFIG_READ 0x20
#define PS2_CONFIG_WRITE 0x60

//R/W
#define PS2_READ 0xD0
#define PS2_WRITE 0xD1

//Dump
#define PS2_DUMPRAM 0xAC

//Devices (FROM OSDEV Wiki)
#define PS2_MOUSE_A 0x00
#define PS2_MOUSE_B 0x03
#define PS2_MOUSE_C 0x04
#define PS2_KEYBOARD_A 0xAB
#define PS2_KEYBOARD_B 0x83

//PORTS
#define PS2_STATE 0x64
#define PS2_DATA 0x60
#define PS2_PORTB 0x61

//init
bool PS2_init();

//Enable Ints
void PS2_enableInt();

//funcs
void PS2_send(uint8_t port, uint8_t cmd);
uint8_t PS2_rec(uint8_t port);

#endif
