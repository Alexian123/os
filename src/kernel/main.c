#include <kernel/vga_io.h>
#include <kernel/asmcall.h>
#include <kernel/idt.h>
#include <kernel/pic_8259.h>
#include <kernel/isr.h>
#include <stdio.h>

static volatile uint32_t ticks = 0;

isr_handler_t timer_irq_handler(registers_t *regs) {
	(void)regs;
	printf("%d ", ++ticks);
}

void kernel_main(void) {
	idt_init();
	debug_printf("[DEBUG] IDT initialized.\n");

	const pic_driver_t *pic_driver = get_pic_driver();
	pic_driver->remap(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);
	debug_printf("[DEBUG] PIC remapped.\n");

	isr_register_handler(32, timer_irq_handler);

	pic_driver->unmask(0);
	sti();

	vga_clearscreen();
	
	printf("Hello, Kernel!\n");

	for (;;) {
		hlt();
	}
}