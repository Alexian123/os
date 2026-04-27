#include <kernel/gdt.h>
#include <kernel/vga_io.h>
#include <kernel/asmcall.h>
#include <kernel/idt.h>
#include <kernel/pic_driver.h>
#include <kernel/isr.h>
#include <kernel/keyboard_driver.h>
#include <kernel/input_event_queue.h>
#include <kernel/tty.h>
#include <kernel/system_timer.h>
#include <stdio.h>

static const int system_timer_frequency = 1000; // Hz

void hal_init(void) {
	// init GDT
	gdt_init();
	debug_printf("[DEBUG] HAL: GDT initialized.\n");

	// init IDT
    idt_init();
	debug_printf("[DEBUG] HAL: IDT initialized.\n");

	// init PIC
	const pic_driver_t *pic_driver = get_pic_i8258_driver();
	pic_driver->remap(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);
	debug_printf("[DEBUG] HAL: %s remapped.\n", pic_driver->name);

	// init input event queue
	input_queue_init();
	debug_printf("[DEBUG] HAL: Input event queue initialized.\n");

	// init keyboard
	const keyboard_driver_t *kbd = get_keyboard_driver();
	kbd->init();
	debug_printf("[DEBUG] HAL: %s driver initialized.\n", kbd->name);

	// init timer
	system_timer_init(system_timer_frequency);
	debug_printf("[DEBUG] HAL: System timer initialized with %dHz frequency.\n", system_timer_frequency);

	// init TTY
	tty_init();
	debug_printf("[DEBUG] HAL: TTY initialized.\n");

    // Enable IRQ's
	sti();
    debug_printf("[DEBUG] HAL: Enabled IRQ's.\n");

    debug_printf("[DEBUG] HAL: DONE.\n");
}