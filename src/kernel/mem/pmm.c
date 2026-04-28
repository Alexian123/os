#include <kernel/mem/pmm.h>
#include <string.h>
#include <stdint.h>

#define BITS_PER_WORD   32
#define U32_MAX         0xFFFFFFFF

static uint32_t* bitmap;
static size_t total_frames;

/* --- Helpers --- */

static inline size_t frame_index(uintptr_t addr) {
    return addr / PAGE_SIZE;
}

static inline void set_bit(size_t frame) {
    bitmap[frame / BITS_PER_WORD] |= (1 << (frame % BITS_PER_WORD));
}

static inline void clear_bit(size_t frame) {
    bitmap[frame / BITS_PER_WORD] &= ~(1 << (frame % BITS_PER_WORD));
}

void pmm_init(uintptr_t bitmap_start_phys, size_t ram_size) {
    bitmap = (uint32_t*)(bitmap_start_phys + KERNEL_VIRTUAL_BASE);
    total_frames = ram_size / PAGE_SIZE;

    size_t bitmap_bytes = (total_frames + 7) / 8;

    // mark everything USED
    memset(bitmap, 0xFF, bitmap_bytes);
}

void pmm_lock_region(uintptr_t base, size_t size) {
    uintptr_t start = base / PAGE_SIZE;
    uintptr_t end   = (base + size + PAGE_SIZE - 1) / PAGE_SIZE;

    for (uintptr_t f = start; f < end; ++f) {
        set_bit(f);
    }
}

void pmm_free_region(uintptr_t base, size_t size) {
    uintptr_t start = base / PAGE_SIZE;
    uintptr_t end   = (base + size) / PAGE_SIZE;

    for (uintptr_t f = start; f < end; ++f) {
        clear_bit(f);
    }
}

uintptr_t pmm_alloc_page(void) {
    size_t words = total_frames / BITS_PER_WORD;

    for (size_t i = 0; i < words; ++i) {
        if (bitmap[i] == U32_MAX) continue;

        for (int j = 0; j < BITS_PER_WORD; ++j) {
            uint32_t bit = 1 << j;
            
            // check if page is free
            if (!(bitmap[i] & bit)) {
                bitmap[i] |= bit;
                return (uintptr_t)((i * BITS_PER_WORD + j) * PAGE_SIZE);
            }
        }
    }

    return 0;
}