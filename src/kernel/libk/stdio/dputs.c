#include <stdio.h>

int dputs(const char *s, int fd) {
    int count = 0;
    while (*s) {
        dputc(*s, fd);
        ++s;
        ++count;
    }
    return count;
}