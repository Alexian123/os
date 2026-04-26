#include <kernel/asmcall.h>
#include <kernel/tty.h>
#include <kernel/system_timer.h>
#include <kernel/hal.h>

#include <stdio.h>

void kernel_main(void) {
	hal_init();

	// for (;;) {
	// 	for (int i = 0; i < 5; ++i) {
	// 		system_timer_sleep(1000);
	// 		printf(".");
	// 	}
	// 	printf(" 5 seconds passed. \n");
	// }

	tty_write("Hello, Kernel!\n> ");
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