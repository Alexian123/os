#include <stdio.h>

int putchar(int c) {
    return dputc(c, FD_STDOUT);
}