#include <kernel/vga_io.h>
#include <kernel/asmcall.h>
#include <stdio.h>

void kernel_main(void) {
	vga_clearscreen();
	debug_printf("[DEBUG] VGA IO initialized.\n");
	printf("Hello, Kernel!\n");

	for (;;) {
		hlt();
	}
}