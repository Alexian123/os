#include <kernel/vga_io.h>
#include <kernel/asmcall.h>
#include <kernel/idt.h>
#include <kernel/pic_8259.h>
#include <kernel/isr.h>
#include <kernel/keyboard_driver.h>
#include <kernel/input_event_queue.h>
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

	for (;;) {
		input_event_t ev;
		if (input_queue_pop_event(&ev)) {
			switch (ev.type) {
				case INPUT_EVENT_KEY:
					if (ev.key.pressed && ev.key.ascii) {
						putchar(ev.key.ascii);
					}
					break;

				default:
					break;
			}
		}

		hlt();
	}
}