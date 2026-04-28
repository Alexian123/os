#include <kernel/arch/i386/isr.h>
#include <kernel/drivers/pit_driver.h>
#include <kernel/drivers/pic_driver.h>
#include <kernel/arch/i386/asmcall.h>

#define TIMER_IRQ 0

static volatile uint32_t system_timer_fractions = 0;
static volatile uint32_t system_timer_ms = 0;
static uint32_t irq0_fractions = 0;
static uint32_t irq0_ms = 0;
static uint32_t irq0_frequency = 0;  
static uint16_t timer_reload_value = 0;

/* --- IRQ handler --- */
static void system_timer_irq(registers_t *r) { 
    // Carry over from fractions to milliseconds
    if (system_timer_fractions < irq0_fractions) { 
        // A 32-bit overflow occurred
        system_timer_ms += 1;
    }
    
    system_timer_ms += irq0_ms;
}

/* --- Init --- */
void system_timer_init(uint32_t frequency) {
    uint32_t reload = 0;

    if (frequency <= 18) {
        reload = 0x10000; // Max reload (effectively 65536)
    } else if (frequency >= 1193181) {
        reload = 1;
    } else {
        // High precision rounding: (3579545 / 3) / frequency
        // 3579545 / 3 = 1193181.666...
        reload = (1193182 + (frequency / 2)) / frequency;
    }
    
    // Ensure reload doesn't exceed 16-bit (0 is 65536 in PIT terms)
    if (reload > 0xFFFF) reload = 0; 
    timer_reload_value = (uint16_t)reload;

    // Calculate actual frequency for display
    irq0_frequency = (reload == 0) ? 18 : (1193182 / reload);

    // Fixed point 32.32 calculation
    // result = (reload * 3000 * 2^42 / 3579545) >> 10
    uint64_t full_val = (uint64_t)timer_reload_value * 0xDBB3A062ULL;
    if (timer_reload_value == 0) full_val = (uint64_t)65536 * 0xDBB3A062ULL;
    
    full_val >>= 10;
    irq0_ms = (uint32_t)(full_val >> 32);
    irq0_fractions = (uint32_t)full_val;

    const pit_driver_t *pit_drv = get_pit_i8254_driver();
    pit_drv->configure(PIT_CMD_SELECT_CH0 | PIT_CMD_ACCESS_LO_HI | PIT_CMD_OPMODE2); // Channel 0, lobyte/hibyte, Rate Gen
    pit_drv->set_reload_value(PIT_CHANNEL0, timer_reload_value);
    
    isr_register_handler(TIMER_IRQ + PIC_REMAP_OFFSET, system_timer_irq);
    get_pic_i8258_driver()->unmask(TIMER_IRQ);
}

void system_timer_sleep(uint32_t ms) {
    uint32_t end_time = system_timer_ms + ms;
    while (system_timer_ms < end_time) {
        hlt();
    }
}