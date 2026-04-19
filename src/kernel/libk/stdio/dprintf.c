#include <stdio.h>

int dprintf(int fd, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int bytes = vdprintf(fd, fmt, ap);
    va_end(ap);
    return bytes;
}