#ifndef __ISR_H
#define __ISR_H 1

#include <stdint.h>

/* CPU state pushed by ISR stub */
typedef struct registers 
{
    // reverse order in which they were pushed
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

/* ISR Handler type */
typedef void (*isr_handler_t)(registers_t*);

void isr_register_handler(uint8_t n, isr_handler_t handler);

#endif