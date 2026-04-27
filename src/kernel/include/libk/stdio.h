#ifndef __STDIO_H
#define __STDIO_H 1

#include <stdarg.h>

#define FD_STDIN    0
#define FD_STDOUT   1
#define FD_STDERR   2
#define FD_DEBUG    3

int putchar(int c);
int puts(const char *s);

int dputc(int c, int fd);
int dputs(const char *s, int fd);

int printf(const char *format, ...);
int dprintf(int fd, const char *format, ...);
int vdprintf(int fd, const char *format, va_list ap);
int debug_printf(const char *format, ...);

#endif