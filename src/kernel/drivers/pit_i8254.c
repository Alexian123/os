#include <kernel/drivers/pit_driver.h>
#include <kernel/arch/i386/port_io.h>
#include <kernel/arch/i386/asmcall.h>
#include <kernel/arch/i386/isr.h>
#include <kernel/drivers/pic_driver.h>

#define PIT_IRQ 0

#define PIT_CMD_PORT        0x43
#define PIT_CH0_DATA_PORT   0x40
#define PIT_CH1_DATA_PORT   0x41
#define PIT_CH2_DATA_PORT   0x42

static uint16_t pit_read_reload_value(pit_channel_t channel) {
	uint16_t reload_value = 0;
    uint8_t data_port = 0, selected_channel = 0;

    switch (channel) {
    case PIT_CHANNEL0:
        data_port = PIT_CH0_DATA_PORT;
        selected_channel = PIT_CMD_SELECT_CH0;
        break;
    
    case PIT_CHANNEL1:
        data_port = PIT_CH1_DATA_PORT;
        selected_channel = PIT_CMD_SELECT_CH1;
        break;

    case PIT_CHANNEL2:
        data_port = PIT_CH2_DATA_PORT;
        selected_channel = PIT_CMD_SELECT_CH2;
        break;

    default:
        return 0;
    } 

    // Disable interrupts
	cli();
	
	// channel in bits 6 and 7, remaining bits clear
	outb(PIT_CMD_PORT, selected_channel);
	
	reload_value = inb(data_port);                         // Low byte
	reload_value |= (((uint16_t)inb(data_port)) << 8);     // High byte
	
	return reload_value;
}

static void pit_set_reload_valuet(pit_channel_t channel, uint16_t reload_value) {
    uint8_t data_port = 0;

    switch (channel) {
    case PIT_CHANNEL0:
        data_port = PIT_CH0_DATA_PORT;
        break;
    
    case PIT_CHANNEL1:
        data_port = PIT_CH1_DATA_PORT;
        break;

    case PIT_CHANNEL2:
        data_port = PIT_CH2_DATA_PORT;
        break;

    default:
        return;
    } 

    // Disable interrupts
	cli();
	
	// Set low byte
	outb(data_port,  (uint8_t)(reload_value & 0xFF));		        // Low byte
	outb(data_port, (uint8_t)((reload_value >> 8) & 0xFF));	    // High byte
}

static void pit_configure(uint8_t cmd_flags) {
    // Disabled interrupts (just in case)
    // Note: It's better to save/restore interrupt state than just CLI
    cli();
    outb(PIT_CMD_PORT, cmd_flags);
}

static const pit_driver_t driver = {
    .name = "i8254 PIT",
    .read_reload_value = &pit_read_reload_value,
    .set_reload_value = &pit_set_reload_valuet,
    .configure = &pit_configure
};

const pit_driver_t *get_pit_i8254_driver() {
    return &driver;
}