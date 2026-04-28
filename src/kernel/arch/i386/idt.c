#include <kernel/arch/i386/idt.h>
#include <kernel/arch/i386/asmcall.h>

#define IDT_ENTRIES 256

struct idt_entry 
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type_attr;
    uint16_t offset_high;
} __attribute__((packed));

struct idt_descriptor 
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_descriptor idt_desc;

extern void ASMCALL idt_load(uint32_t idt_desc_ptr);

/* CPU exceptions (0–31) */
#define ISR_EXCEPTION_LIST \
    X(0)  X(1)  X(2)  X(3)  X(4)  X(5)  X(6)  X(7) \
    X(8)  X(9)  X(10) X(11) X(12) X(13) X(14) X(15) \
    X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) \
    X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31)

/* PIC IRQ's (32-47) */
#define IRQ_LIST \
    X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) \
    X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47)

/* Declare ISR stubs */
#define X(n) extern void ASMCALL isr##n(void);
ISR_EXCEPTION_LIST \
IRQ_LIST
#undef X

void idt_init(void) {
    idt_desc.limit = sizeof(idt) - 1;
    idt_desc.base  = (uint32_t)&idt;

    /* Clear table */
    for (int i = 0; i < IDT_ENTRIES; ++i) {
        idt_set_gate(i, 0, 0, 0);
    }

    /* Enable CPU Exceptions and IRQ's */
#define X(n) idt_set_gate(n, (uint32_t)isr##n, 0x08, IDT_INT_GATE_PL0);
    ISR_EXCEPTION_LIST \
    IRQ_LIST
#undef X

    idt_load((uint32_t)&idt_desc);
}

void idt_set_gate(uint8_t n, uint32_t handler, uint16_t selector, uint8_t flags){
    idt[n].offset_low  = handler & 0xFFFF;
    idt[n].selector    = selector;
    idt[n].zero        = 0;
    idt[n].type_attr   = flags;
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}