#ifndef __PORT_IO_H
#define __PORT_IO_H 1

#include <stdint.h>
#include <kernel/arch/i386/asmcall.h>

#define UNUSED_PORT 0x80

extern void ASMCALL outb(uint16_t port, uint8_t data);
extern uint8_t ASMCALL inb(uint16_t port);

void iowait(void);

#endif