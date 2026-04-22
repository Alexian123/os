#include <kernel/isr.h>
#include <kernel/asmcall.h>
#include <stdio.h>
#include <stddef.h>

#define ISR_COUNT 256
#define EXCEPTION_COUNT 32

static const char* const exceptions[EXCEPTION_COUNT] = {
    "Divide by zero error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception ",
    "",
    "",
    "",
    "",
    "",
    "",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    ""
};

static isr_handler_t handlers[ISR_COUNT];

void isr_register_handler(uint8_t n, isr_handler_t handler) {
    handlers[n] = handler;
}

/* Main dispatcher called from assembly */
void ASMCALL isr_handler_common(registers_t* regs) {
    if (handlers[regs->int_no] != NULL) {
        handlers[regs->int_no](regs);
    } else if (regs->int_no < EXCEPTION_COUNT) {
        printf("Unhandled exception: %s (0x%x)\n", exceptions[regs->int_no], regs->int_no);
        hlt();
    } else {
        printf("Interrupt 0x%x not handled!\n", regs->int_no);
    }
}