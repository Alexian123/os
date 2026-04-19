#ifndef __PORT_IO_H
#define __PORT_IO_H

#include <stdint.h>
#include <arch/i686/asm.h>

void ASMCALL outb(uint16_t port, uint8_t data);
void ASMCALL outw(uint16_t port, uint16_t data);
void ASMCALL outdw(uint16_t port, uint32_t data);

uint8_t ASMCALL inb(uint16_t port);
uint16_t ASMCALL inw(uint16_t port);
uint32_t ASMCALL indw(uint16_t port);

#endif