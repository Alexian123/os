#ifndef __PORT_IO_H
#define __PORT_IO_H

#include <stdint.h>
#include <kernel/asmcall.h>

extern void ASMCALL outb(uint16_t port, uint8_t data);
extern uint8_t ASMCALL inb(uint16_t port);

#endif