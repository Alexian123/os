#include <stdio.h>
#include <arch/i686/vga_io.h>
#include <arch/i686/debug_e9.h>

int dputc(int c, int fd) {
    switch (fd) {
    case FD_STDERR:
    case FD_STDOUT:
        vga_putchar(c);
        return (int)((unsigned char)c);

    case FD_DEBUG:
        debug_putchar(c);
        return (int)((unsigned char)c);
        
    default:    // unsupported
        break;
    }

    return -1;
}