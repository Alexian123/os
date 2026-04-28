#include <kernel/arch/i386/asmcall.h>
#include <kernel/drivers/vga_io.h>
#include <stdio.h>

void panic(const char *msg) {
	vga_clearscreen();
	printf("KERNEL PANINC: %s\n", msg);
	for (;;) hlt();
}