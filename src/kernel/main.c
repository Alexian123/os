#include <arch/i686/vga_io.h>
#include <stdio.h>
#include <string.h>
#include <arch/i686/port_io.h>

void kernel_main(void) {
	vga_initialize();
	debug_printf("VGA IO initialized.\n");
	printf("Hello, Kernel!\n");
}