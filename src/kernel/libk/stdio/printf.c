#include <stdio.h>

int printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int bytes = vdprintf(FD_STDOUT, fmt, ap);
    va_end(ap);
    return bytes;
}