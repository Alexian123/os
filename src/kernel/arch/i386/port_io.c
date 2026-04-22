#include <kernel/port_io.h>

void iowait(void) {
    outb(UNUSED_PORT, 0);
}