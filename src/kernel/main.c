#include <kernel/arch/i386/multiboot.h>
#include <kernel/arch/i386/asmcall.h>
#include <kernel/drivers/tty.h>
#include <kernel/drivers/system_timer.h>
#include <kernel/arch/i386/hal.h>
#include <kernel/arch/i386/x86_utils.h>
#include <kernel/core/utils.h>

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define KERNEL_VIRTUAL_BASE 0xC0000000

#define SANITY_CHECKS 1

uintptr_t free_memory_start;
size_t total_ram;

void ASMCALL kernel_main(multiboot_info_t *mbd, unsigned int magic) {
	// determine kernel physical range
	extern char _kernel_start[]; 
    extern char _kernel_end[];
	uintptr_t k_start = (uintptr_t)_kernel_start - KERNEL_VIRTUAL_BASE;
    uintptr_t k_end = (uintptr_t)_kernel_end - KERNEL_VIRTUAL_BASE;

#ifdef SANITY_CHECKS
	// make sure the magic number matches for memory mapping
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        panic("invalid magic number!");
    }
	
	// check bit 6 to see if we have a valid memory map
    if (!(mbd->flags >> 6 & 0x1)) {
        panic("invalid memory map given by GRUB bootloader");
    }
#endif

	// parse memory map
	multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)mbd->mmap_addr;
    uint32_t mmap_entries = mbd->mmap_length / sizeof(multiboot_memory_map_t);
	for (uint32_t i = 0; i < mmap_entries; ++i) {
        if (mmap[i].type == MULTIBOOT_MEMORY_AVAILABLE) {
            // Find the highest available address to determine total RAM
			uint64_t addr = ((uint64_t)mmap[i].addr_high << 32) | (uint64_t)mmap[i].addr_low;
        	uint64_t len  = ((uint64_t)mmap[i].len_high << 32) | (uint64_t)mmap[i].len_low;
            uint64_t entry_end = addr + len;
            if (entry_end > total_ram) total_ram = (size_t)entry_end;
        }
    }

	// init PMM Bitmap
    //pmm_init(k_end, total_ram);	 // place the bitmap immediately after the kernel in memory

    // 4. Mark reserved areas in Bitmap
    // Loop through mmap again: if type != AVAILABLE, pmm_lock_region(addr, len)
    // ALSO: pmm_lock_region(k_start, k_end - k_start); // Protect the kernel!

	// init hardware abstraction layer
	hal_init();

	/* Unmap identity mapped memory */
	extern uint32_t initial_page_dir[];
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