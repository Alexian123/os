#include <kernel/arch/i386/port_io.h>

#define PORT_E9 0xE9

void debug_putchar(char c) {
    outb(PORT_E9, (uint8_t)c);
}