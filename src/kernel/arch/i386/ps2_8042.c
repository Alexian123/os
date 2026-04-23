#include <kernel/ps2_controller.h>
#include <kernel/port_io.h>

#define PS2_DATA_PORT   0x60
#define PS2_STATUS_PORT 0x64
#define PS2_CMD_PORT    0x64

/* Status bits */
#define PS2_STATUS_OUTPUT 0x01
#define PS2_STATUS_INPUT  0x02

/* Commands */
#define PS2_CMD_ENABLE_PORT1  0xAE
#define PS2_CMD_DISABLE_PORT1 0xAD

static bool wait_input_clear(void) {
    for (int i = 0; i < 100000; ++i) {
        if (!(inb(PS2_STATUS_PORT) & PS2_STATUS_INPUT))
            return true;
    }
    return false;
}

static bool wait_output_full(void) {
    for (int i = 0; i < 100000; ++i) {
        if (inb(PS2_STATUS_PORT) & PS2_STATUS_OUTPUT)
            return true;
    }
    return false;
}

static void ps2_write(uint8_t data) {
    wait_input_clear();
    outb(PS2_DATA_PORT, data);
}

static bool ps2_read(uint8_t *data) {
    if (!wait_output_full())
        return false;

    *data = inb(PS2_DATA_PORT);
    return true;
}

static bool ps2_read_status(uint8_t *status) {
    *status = inb(PS2_STATUS_PORT);
    return true;
}

static void ps2_enable_port1(void) {
    wait_input_clear();
    outb(PS2_CMD_PORT, PS2_CMD_ENABLE_PORT1);
}

static void ps2_disable_port1(void) {
    wait_input_clear();
    outb(PS2_CMD_PORT, PS2_CMD_DISABLE_PORT1);
}

static void ps2_init(void) {
    ps2_disable_port1();
    ps2_enable_port1();
}

static const ps2_controller_driver_t driver = {
    .name = "8042 PS/2 Controller",
    .init = ps2_init,
    .read = ps2_read,
    .write = ps2_write,
    .read_status = ps2_read_status,
    .enable_port1 = ps2_enable_port1,
    .disable_port1 = ps2_disable_port1
};

const ps2_controller_driver_t *get_ps2_controller(void) {
    return &driver;
}