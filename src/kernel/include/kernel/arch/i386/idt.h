#ifndef __IDT_H
#define __IDT_H 1

#include <stdint.h>

/* Gate type + flags (type_attr byte) */
enum idt_flags 
{
    IDT_FLAG_GATE_TASK      = 0x5, // unused here
    IDT_FLAG_GATE_16_INT    = 0x6,
    IDT_FLAG_GATE_16_TRAP   = 0x7,
    IDT_FLAG_GATE_32_INT    = 0xE,
    IDT_FLAG_GATE_32_TRAP   = 0xF,

    IDT_FLAG_RING0          = 0 << 5,
    IDT_FLAG_RING1          = 1 << 5,
    IDT_FLAG_RING2          = 2 << 5,
    IDT_FLAG_RING3          = 3 << 5,

    IDT_FLAG_PRESENT        = 1 << 7
};

#define IDT_INT_GATE_PL0 (IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32_INT)
#define IDT_TRAP_GATE_PL0 (IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32_TRAP)

void idt_init(void);
void idt_set_gate(uint8_t n, uint32_t handler, uint16_t selector, uint8_t flags);

#endif