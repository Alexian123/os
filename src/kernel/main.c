#include <kernel/vga_io.h>
#include <kernel/asmcall.h>
#include <kernel/idt.h>
#include <kernel/pic_8259.h>
#include <kernel/isr.h>
#include <kernel/keyboard_driver.h>
#include <kernel/input_event_queue.h>
#include <kernel/tty.h>

#include <stdio.h>

void kernel_main(void) {
	idt_init();
	debug_printf("[DEBUG] IDT initialized.\n");

	// init PIC
	const pic_driver_t *pic_driver = get_pic_driver();
	pic_driver->remap(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);
	debug_printf("[DEBUG] %s remapped.\n", pic_driver->name);

	// init input event queue
	input_queue_init();

	// init keyboard
	const keyboard_driver_t *kbd = get_keyboard_driver();
	kbd->init();
	debug_printf("[DEBUG] %s driver initialized.\n", kbd->name);

	// Enable IRQ's
	pic_driver->unmask(1);
	sti();

	vga_clearscreen();
	//printf("Hello, Kernel!\n");

	tty_init();

	tty_write("Hello, Kernel!\n");

	for (;;) {
		tty_update();

		if (tty_line_available()) {
			const char *line = tty_get_line();

			tty_write("You typed: ");
			tty_write(line);
			tty_write("\n> ");
		}

		hlt();
	}
}