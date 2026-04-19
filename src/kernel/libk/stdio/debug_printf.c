#include <stdio.h>

int debug_printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int bytes = vdprintf(FD_DEBUG, fmt, ap);
    va_end(ap);
    return bytes;
}