#include <kernel/isr.h>
#include <kernel/asmcall.h>
#include <kernel/pic_driver.h>
#include <stdio.h>
#include <stddef.h>

#define ISR_COUNT           256
#define EXCEPTION_COUNT     32

#define IRQ_START_INDEX     PIC_REMAP_OFFSET
#define IRQ_END_INDEX       PIC_REMAP_OFFSET + 16

static const char *const exceptions[EXCEPTION_COUNT] = {
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
        // call registered handler
        handlers[regs->int_no](regs);
        
        // check if it is an IRQ
        if (regs->int_no >= IRQ_START_INDEX && regs->int_no <= IRQ_END_INDEX) {
            // send End Of Interrupt if not auto
            pic_driver_t *pic_drv = get_pic_i8258_driver();
            if (!pic_drv->is_auto_eoi()) {
                get_pic_i8258_driver()->send_eoi(regs->int_no - IRQ_START_INDEX);
            }
        }
    } else if (regs->int_no < EXCEPTION_COUNT) {
        printf("Unhandled exception: %s (0x%x), code %d\n", exceptions[regs->int_no], regs->int_no, regs->err_code);
        hlt();
    } else {
        printf("Interrupt 0x%x not handled!\n", regs->int_no);
    }
}