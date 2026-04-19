#include <stdio.h>

int puts(const char *s) {
    return dputs(s, FD_STDOUT);
}