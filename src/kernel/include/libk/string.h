#ifndef __STRING_H
#define __STRING_H 1

#include <stddef.h>

size_t strlen(const char *str);
int strncmp(const char *s1, const char *s2, size_t n);

void *memcpy(void *dest, const void *src, size_t n);

#endif