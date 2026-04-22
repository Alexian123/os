#include <kernel/vga_io.h>
#include <kernel/asmcall.h>
#include <kernel/idt.h>
#include <stdio.h>

void kernel_main(void) {
	idt_init();
	debug_printf("[DEBUG] IDT initialized.\n");

	vga_clearscreen();

	printf("Hello, Kernel!%d\n");

	for (;;) {
		hlt();
	}
}