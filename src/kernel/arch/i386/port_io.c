#include <kernel/arch/i386/port_io.h>

void iowait(void) {
    outb(UNUSED_PORT, 0);
}