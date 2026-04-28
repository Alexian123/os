#include <stddef.h>
#include <stdint.h>

void *memset(void *s, int c, size_t n) {
    if (!s) return NULL;
    uint8_t *bytes = (uint8_t*)s;
    uint8_t val = (uint8_t)c;
    for (size_t i = 0; i < n; ++i) {
        bytes[i] = val;
    }
    return s;
}