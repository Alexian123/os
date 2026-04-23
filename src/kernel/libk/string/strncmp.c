#include <stddef.h>

int strncmp(const char *s1, const char *s2, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (*s1 != *s2) break;
        ++s1;
        ++s2;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}