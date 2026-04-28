#ifndef __PMM_H
#define __PMM_H 1

#include <stddef.h>
#include <stdint.h>

#define KERNEL_VIRTUAL_BASE 0xC0000000
#define PAGE_SIZE 4096  // 4 KiB

void pmm_init(uintptr_t bitmap_start_phys, size_t ram_size);
void pmm_lock_region(uintptr_t base, size_t size);
void pmm_free_region(uintptr_t base, size_t size);
uintptr_t pmm_alloc_page(void);

#endif