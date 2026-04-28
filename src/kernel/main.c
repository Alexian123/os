#include <kernel/arch/i386/multiboot.h>
#include <kernel/arch/i386/asmcall.h>
#include <kernel/drivers/tty.h>
#include <kernel/drivers/system_timer.h>
#include <kernel/arch/i386/hal.h>
#include <kernel/arch/i386/x86_utils.h>
#include <kernel/core/utils.h>
#include <kernel/mem/pmm.h>

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define SANITY_CHECKS 1

void ASMCALL kernel_main(multiboot_info_t *mbd, unsigned int magic) {
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

	// determine kernel physical range
	extern char _kernel_start[]; 
    extern char _kernel_end[];
	uintptr_t k_start = (uintptr_t)_kernel_start - KERNEL_VIRTUAL_BASE;
    uintptr_t k_end = (uintptr_t)_kernel_end - KERNEL_VIRTUAL_BASE;

	// determine total ram
	size_t total_ram = 0;
    uint32_t offset = 0;
    while (offset < mbd->mmap_length) {
        multiboot_memory_map_t* m = (multiboot_memory_map_t*)(mbd->mmap_addr + offset);
        uint64_t addr = ((uint64_t)m->addr_high << 32) | m->addr_low;
        uint64_t len  = ((uint64_t)m->len_high  << 32) | m->len_low;

		// If the START of the region is above 4GB, skip it entirely
		if (addr >= 0xFFFFFFFF) {
			continue;
		}

		// If the region STARTS below 4GB but ENDS above it, truncate it
		if (addr + len > 0xFFFFFFFF) {
			len = 0xFFFFFFFF - addr;
		}

        uint64_t end = addr + len;
        if (end > total_ram) {
            total_ram = (size_t)end;
        }
        offset += m->size + sizeof(m->size);
    }

	// init PMM (place bitmap right after kernel)
    pmm_init(k_end, total_ram);

    // mark memory regions
    offset = 0;
    while (offset < mbd->mmap_length) {
        multiboot_memory_map_t* m = (multiboot_memory_map_t*)(mbd->mmap_addr + offset);
        uint64_t addr = ((uint64_t)m->addr_high << 32) | m->addr_low;
        uint64_t len  = ((uint64_t)m->len_high  << 32) | m->len_low;

		// If the START of the region is above 4GB, skip it entirely
		if (addr >= 0xFFFFFFFF) {
			continue;
		}

		// If the region STARTS below 4GB but ENDS above it, truncate it
		if (addr + len > 0xFFFFFFFF) {
			len = 0xFFFFFFFF - addr;
		}

        if (m->type == MULTIBOOT_MEMORY_AVAILABLE) {
            pmm_free_region((uintptr_t)addr, (size_t)len);
        } else {
            pmm_lock_region((uintptr_t)addr, (size_t)len);
        }
        offset += m->size + sizeof(m->size);
    }

	// reserve kernel region
    pmm_lock_region(k_start, k_end - k_start);

    // reserve bitmap region
    size_t total_frames = total_ram / PAGE_SIZE;
    size_t bitmap_bytes = (total_frames + 7) / 8;
    pmm_lock_region(k_end, bitmap_bytes);

    // reserve low memory
    pmm_lock_region(0x0, 0x100000);

	// init hardware abstraction layer
	hal_init();

	// remove identity mapping
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