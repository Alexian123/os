#include <kernel/asmcall.h>
#include <kernel/vga_io.h>
#include <stdio.h>

void panic(const char *msg) {
	vga_clearscreen();
	printf("KERNEL PANINC: %s\n", msg);
	for (;;) hlt();
}