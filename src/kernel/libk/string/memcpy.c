#include <stddef.h>
#include <stdint.h>

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *dest_u8 = (uint8_t*)dest;
    uint8_t *src_u8 = (uint8_t*)src; 
    for (size_t i = 0; i < n; ++i) {
        dest_u8[i] = src_u8[i];
    }
    return dest;
}