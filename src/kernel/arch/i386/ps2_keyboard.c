#include <kernel/keyboard_driver.h>
#include <kernel/ps2_controller.h>
#include <kernel/isr.h>
#include <kernel/input_event_queue.h>

#define KBD_IRQ 33

static const ps2_controller_driver_t *ps2;

/* Basic map */
static const char keymap[128] = {
    0,27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/',
    0,'*',0,' ',0
};

static void push_event(uint8_t scancode) {
    input_event_t ev = { 0 };

    ev.type = INPUT_EVENT_KEY;
    ev.key.scancode = scancode;
    ev.key.pressed = !(scancode & 0x80);

    uint8_t code = scancode & 0x7F;
    ev.key.ascii = keymap[code];

    input_queue_push_event(&ev);
}

static void keyboard_irq(registers_t *r) {
    (void)r;
    uint8_t data;
    if (ps2->read(&data)) {
        push_event(data);
    }
}

static void keyboard_init(void) {
    ps2 = get_ps2_controller();
    ps2->init();
    isr_register_handler(KBD_IRQ, keyboard_irq);
}

static const keyboard_driver_t driver = {
    .name = "PS/2 Keyboard",
    .init = keyboard_init
};

const keyboard_driver_t *get_keyboard_driver(void) {
    return &driver;
}