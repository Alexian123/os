#include <kernel/multiboot.h>
#include <kernel/asmcall.h>
#include <kernel/tty.h>
#include <kernel/system_timer.h>
#include <kernel/hal.h>
#include <kernel/util.h>

#include <stdint.h>
#include <stdio.h>

extern uint32_t initial_page_dir[];

void ASMCALL kernel_main(multiboot_info_t *mbd, unsigned int magic) {
	// init hardware abstraction layer
	hal_init();

	/* Make sure the magic number matches for memory mapping*/
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        panic("invalid magic number!");
    }
	
	/* Check bit 6 to see if we have a valid memory map */
    if (!(mbd->flags >> 6 & 0x1)) {
        panic("invalid memory map given by GRUB bootloader");
    }

    /* Loop through the memory map and display the values */
    for (int i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);
        printf("Start Addr: 0x%x%x | Length: 0x%x%x | Size: 0x%x | Type: %d\n",
            mmmt->addr_high, mmmt->addr_low, mmmt->len_high, mmmt->len_low, mmmt->size, mmmt->type);
        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
            /* 
             * Do something with this memory block!
             * BE WARNED that some of memory shown as availiable is actually 
             * actively being used by the kernel! You'll need to take that
             * into account before writing to memory!
             */
        }
    }

	/* Unmap identity mapped memory */
	initial_page_dir[0] = 0;	// clear first entry (0MB - 4MB)
	flush_tlb();

	// for (;;) {
	// 	for (int i = 0; i < 5; ++i) {
	// 		system_timer_sleep(1000);
	// 		printf(".");
	// 	}
	// 	printf(" 5 seconds passed. \n");
	// }

	/* Shell */
	tty_write("Hello, Kernel!\n> ");
	for (;;) {	// this loop should never end
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