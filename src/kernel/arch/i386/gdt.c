#include <stdint.h>
#include <kernel/arch/i386/asmcall.h>

struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

struct gdt_descriptor
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

/* Access byte flags */
enum gdt_access 
{
    GDT_ACCESS_ACCESSED        = 1 << 0,

    GDT_ACCESS_RW              = 1 << 1, // readable (code) / writable (data)
    GDT_ACCESS_DC              = 1 << 2, // direction (data) / conforming (code)
    GDT_ACCESS_EXECUTABLE      = 1 << 3, // 1 = code, 0 = data

    GDT_ACCESS_DESCRIPTOR_TYPE = 1 << 4, // 1 = code/data, 0 = system

    GDT_ACCESS_RING0           = 0 << 5,
    GDT_ACCESS_RING1           = 1 << 5,
    GDT_ACCESS_RING2           = 2 << 5,
    GDT_ACCESS_RING3           = 3 << 5,

    GDT_ACCESS_PRESENT         = 1 << 7
};

/* Granularity byte flags */
enum gdt_granularity 
{
    GDT_GRAN_LIMIT_HIGH_MASK = 0x0F, // lower 4 bits

    GDT_GRAN_AVAILABLE       = 1 << 4,
    GDT_GRAN_32BIT           = 1 << 6, // D/B bit
    GDT_GRAN_4K              = 1 << 7  // granularity
};

#define GDT_CODE_PL0 ( \
    GDT_ACCESS_PRESENT | \
    GDT_ACCESS_DESCRIPTOR_TYPE | \
    GDT_ACCESS_EXECUTABLE | \
    GDT_ACCESS_RW | \
    GDT_ACCESS_RING0 )

#define GDT_DATA_PL0 ( \
    GDT_ACCESS_PRESENT | \
    GDT_ACCESS_DESCRIPTOR_TYPE | \
    GDT_ACCESS_RW | \
    GDT_ACCESS_RING0 )

#define GDT_GRAN_DEFAULT ( \
    GDT_GRAN_4K | \
    GDT_GRAN_32BIT )

static struct gdt_entry gdt[3];
static struct gdt_descriptor gdt_desc;

extern void ASMCALL gdt_load(uint32_t gdt_desc_ptr);

static void gdt_set_gate(int i, uint32_t base, uint32_t limit,
                         uint8_t access, uint8_t gran) {
    gdt[i].base_low    = base & 0xFFFF;
    gdt[i].base_middle = (base >> 16) & 0xFF;
    gdt[i].base_high   = (base >> 24) & 0xFF;

    gdt[i].limit_low   = limit & 0xFFFF;
    gdt[i].granularity = (limit >> 16) & 0x0F;
    gdt[i].granularity |= gran & 0xF0;
    gdt[i].access      = access;
}

void gdt_init(void) {
    gdt_desc.limit = sizeof(gdt) - 1;
    gdt_desc.base  = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);                                    // null
    gdt_set_gate(1, 0, 0xFFFFFFFF, GDT_CODE_PL0, GDT_GRAN_DEFAULT); // code
    gdt_set_gate(2, 0, 0xFFFFFFFF, GDT_DATA_PL0, GDT_GRAN_DEFAULT); // data

    gdt_load((uint32_t)&gdt_desc);
}