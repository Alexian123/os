#ifndef __ASM_H
#define __ASM_H

#define ASMCALL __attribute__((cdecl))

static inline void cli(void) {
    __asm__ volatile ("cli" ::: "memory");
}

static inline void sti(void) {
    __asm__ volatile ("sti" ::: "memory");
}

static inline void hlt(void) {
    __asm__ volatile ("hlt");
}

#endif