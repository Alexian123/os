#ifndef __UTIL_H
#define __UTIL_H 1

#include <kernel/asmcall.h>

void panic(const char *msg);

extern void ASMCALL flush_tlb(void);

#endif