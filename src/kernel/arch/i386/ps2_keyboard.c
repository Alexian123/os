#include <kernel/keyboard_driver.h>
#include <kernel/ps2_controller.h>
#include <kernel/isr.h>
#include <kernel/input_event_queue.h>
#include <kernel/keycodes.h>

#define KBD_IRQ 33

static const ps2_controller_driver_t *ps2;

/* --- Parser state --- */
static int extended = 0;
static int e1_bytes = 0;

/* --- Translate Set 1 scancodes → keycodes --- */
static keycode_t translate_set1(uint8_t sc, int ext) {
    if (!ext) {
        switch (sc) {
            /* Letters */
            case 0x1E: return KEY_A;
            case 0x30: return KEY_B;
            case 0x2E: return KEY_C;
            case 0x20: return KEY_D;
            case 0x12: return KEY_E;
            case 0x21: return KEY_F;
            case 0x22: return KEY_G;
            case 0x23: return KEY_H;
            case 0x17: return KEY_I;
            case 0x24: return KEY_J;
            case 0x25: return KEY_K;
            case 0x26: return KEY_L;
            case 0x32: return KEY_M;
            case 0x31: return KEY_N;
            case 0x18: return KEY_O;
            case 0x19: return KEY_P;
            case 0x10: return KEY_Q;
            case 0x13: return KEY_R;
            case 0x1F: return KEY_S;
            case 0x14: return KEY_T;
            case 0x16: return KEY_U;
            case 0x2F: return KEY_V;
            case 0x11: return KEY_W;
            case 0x2D: return KEY_X;
            case 0x15: return KEY_Y;
            case 0x2C: return KEY_Z;

            /* Numbers */
            case 0x02: return KEY_1;
            case 0x03: return KEY_2;
            case 0x04: return KEY_3;
            case 0x05: return KEY_4;
            case 0x06: return KEY_5;
            case 0x07: return KEY_6;
            case 0x08: return KEY_7;
            case 0x09: return KEY_8;
            case 0x0A: return KEY_9;
            case 0x0B: return KEY_0;

            /* Control */
            case 0x1C: return KEY_ENTER;
            case 0x0E: return KEY_BACKSPACE;
            case 0x39: return KEY_SPACE;
            case 0x0F: return KEY_TAB;
            case 0x01: return KEY_ESC;

            case 0x2A: return KEY_LSHIFT;
            case 0x36: return KEY_RSHIFT;
            case 0x1D: return KEY_CTRL;
            case 0x38: return KEY_ALT;

            case 0x3A: return KEY_CAPSLOCK;
        }
    } else {
        switch (sc) {
            case 0x48: return KEY_UP;
            case 0x50: return KEY_DOWN;
            case 0x4B: return KEY_LEFT;
            case 0x4D: return KEY_RIGHT;

            case 0x1D: return KEY_CTRL;  // Right Ctrl
            case 0x38: return KEY_ALT;   // Right Alt (AltGr)
        }
    }

    return KEY_NONE;
}

/* --- IRQ handler --- */
static void keyboard_irq(registers_t *r) {
    (void)r;

    uint8_t sc;
    if (!ps2->read(&sc))
        return;

    /* Handle Pause key (E1 sequence: ignore safely) */
    if (sc == 0xE1) {
        e1_bytes = 5;
        return;
    }

    if (e1_bytes > 0) {
        e1_bytes--;
        return;
    }

    /* Extended prefix */
    if (sc == 0xE0) {
        extended = 1;
        return;
    }

    /* Break / make */
    int is_break = sc & 0x80;
    uint8_t code = sc & 0x7F;
    keycode_t key = translate_set1(code, extended);
    extended = 0;
    int pressed = !is_break;

    if (key == KEY_NONE)
        return;

    input_event_t ev = {0};

    ev.type = INPUT_EVENT_KEY;
    ev.key.keycode = key;
    ev.key.scancode = code;
    ev.key.pressed = pressed;

    input_queue_push_event(&ev);
}

/* --- Init --- */
static void keyboard_init(void) {
    ps2 = get_ps2_controller();
    ps2->init();

    isr_register_handler(KBD_IRQ, keyboard_irq);
}

/* --- Driver struct --- */
static const keyboard_driver_t driver = {
    .name = "PS/2 Keyboard",
    .init = keyboard_init
};

const keyboard_driver_t *get_keyboard_driver(void) {
    return &driver;
}