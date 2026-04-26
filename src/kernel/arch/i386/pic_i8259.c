#include <kernel/pic_driver.h>
#include <kernel/port_io.h>

#include <stdbool.h>

#define PIC1_CMD_PORT       0x20
#define PIC1_DATA_PORT      0x21
#define PIC2_CMD_PORT       0xA0
#define PIC2_DATA_PORT      0xA1

// Initialization Control Word 1
// -----------------------------
//  0   ICW4    if set, the PIC expects to receive ICW4 during initialization
//  1   SGNL    if set, only 1 PIC in the system; if unset, the PIC is cascaded with slave PICs
//              and ICW3 must be sent to controller
//  2   ADI     call address interval, set: 4, not set: 8; ignored on x86, set to 0
//  3   LTIM    if set, operate in level triggered mode; if unset, operate in edge triggered mode
//  4   INIT    set to 1 to initialize PIC
//  5-7         ignored on x86, set to 0
enum pic_icw1
{
    PIC_ICW1_ICW4               = 0x01,
    PIC_ICW1_SINGLE             = 0x02,
    PIC_ICW1_INTERVAL4          = 0x04,
    PIC_ICW1_LEVEL              = 0x08,
    PIC_ICW1_INIT               = 0x10
};


// Initialization Control Word 4
// -----------------------------
//  0   uPM     if set, PIC is in 80x86 mode; if cleared, in MCS-80/85 mode
//  1   AEOI    if set, on last interrupt acknowledge pulse, controller automatically performs 
//              end of interrupt operation
//  2   M/S     only use if BUF is set; if set, selects buffer master; otherwise, selects buffer slave
//  3   BUF     if set, controller operates in buffered mode
//  4   SFNM    specially fully nested mode; used in systems with large number of cascaded controllers
//  5-7         reserved, set to 0
enum pic_icw4
{
    PIC_ICW4_8086               = 0x01,
    PIC_ICW4_AUTO_EOI           = 0x02,
    PIC_ICW4_BUFFER_MASTER      = 0x04,
    PIC_ICW4_BUFFER_SLAVE       = 0x00,
    PIC_ICW4_BUFFERED           = 0x08,
    PIC_ICW4_SFNM               = 0x10
};

enum pic_cmd
{
    PIC_CMD_EOI                 = 0x20,
    PIC_CMD_READ_IRR            = 0x0A,
    PIC_CMD_READ_ISR            = 0x0B
};

static uint16_t pic_mask = 0xFFFF;
static bool pic_auto_eoi = false;

static uint16_t pic_get_mask() {
    return (uint16_t)inb(PIC1_DATA_PORT) | (((uint16_t)inb(PIC2_DATA_PORT)) << 8);
}

static void pic_set_mask(uint16_t mask) {
    pic_mask = mask;
    outb(PIC1_DATA_PORT, pic_mask & 0xFF);
    iowait();
    outb(PIC2_DATA_PORT, pic_mask >> 8);
    iowait();
}

static void pic_remap(uint8_t offset_pic1, uint8_t offset_pic2, bool auto_eoi) {
    // send ICW1
    outb(PIC1_CMD_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INIT);
    iowait();
    outb(PIC2_CMD_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INIT);
    iowait();

    // send ICW2 - offsets
    outb(PIC1_DATA_PORT, offset_pic1);
    iowait();
    outb(PIC2_DATA_PORT, offset_pic2);
    iowait();

    // send ICW3
    outb(PIC1_DATA_PORT, 0x4);     // PIC1 has a slave connected to pin IRQ2
    iowait();
    outb(PIC2_DATA_PORT, 0x2);     // PIC2 is connected to PIC1 on pin IRQ2 (cascade id)
    iowait();

    // send ICW4
    uint8_t icw4 = PIC_ICW4_8086;
    if (auto_eoi) {
        pic_auto_eoi = true;
        icw4 |= PIC_ICW4_AUTO_EOI;
    }
    outb(PIC1_DATA_PORT, icw4);
    iowait();
    outb(PIC2_DATA_PORT, icw4);
    iowait();

    // mask all
    pic_set_mask(0xFFFF);
}

static bool pic_is_auto_eoi(void) {
    return pic_auto_eoi;
}

static void pic_mask_irq(int irq) {
    pic_set_mask(pic_mask | (1 << irq));
}

static void pic_unmask_irq(int irq) {
    pic_set_mask(pic_mask & ~(1 << irq));
}

static void pic_disable() {
    pic_set_mask(0xFFFF);
}

static void pic_send_eoi(int irq) {
    if (irq >= 8) {
        outb(PIC2_CMD_PORT, PIC_CMD_EOI);
    }
    outb(PIC1_CMD_PORT, PIC_CMD_EOI);
}

static uint16_t pic_read_irr(void) {
    outb(PIC1_CMD_PORT, PIC_CMD_READ_IRR);
    outb(PIC2_CMD_PORT, PIC_CMD_READ_IRR);
    return (((uint16_t)inb(PIC2_CMD_PORT)) << 8) | ((uint16_t)inb(PIC1_CMD_PORT));
}

static uint16_t pic_read_isr(void) {
    outb(PIC1_CMD_PORT, PIC_CMD_READ_ISR);
    outb(PIC2_CMD_PORT, PIC_CMD_READ_ISR);
    return (((uint16_t)inb(PIC2_CMD_PORT)) << 8) | ((uint16_t)inb(PIC1_CMD_PORT));
}

static bool pic_probe(void) {
    pic_disable();
    pic_set_mask(0x1337);
    return pic_get_mask() == 0x1337;
}

static const pic_driver_t driver = {
    .name = "i8259 PIC",
    .probe = &pic_probe,
    .remap = &pic_remap,
    .is_auto_eoi = &pic_is_auto_eoi,
    .disable = &pic_disable,
    .send_eoi = &pic_send_eoi,
    .mask = &pic_mask_irq,
    .unmask = &pic_unmask_irq
};

const pic_driver_t *get_pic_i8258_driver(void) {
    return &driver;
}