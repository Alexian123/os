#include <kernel/core/keycodes.h>
#include <stdbool.h>

typedef struct keyboard_mods
{
    bool shift;
    bool ctrl;
    bool alt;
    bool caps;
} keyboard_mods_t;

static keyboard_mods_t mods = { 0 };

static const char shift_map[] = {
    ['1'] = '!', ['2'] = '@', ['3'] = '#', ['4'] = '$',
    ['5'] = '%', ['6'] = '^', ['7'] = '&', ['8'] = '*',
    ['9'] = '(', ['0'] = ')',
};

void keymap_update_modifiers(keycode_t key, bool pressed) {
    switch (key) {
        case KEY_LSHIFT:
        case KEY_RSHIFT:
            mods.shift = pressed;
            break;

        case KEY_CTRL:
            mods.ctrl = pressed;
            break;

        case KEY_ALT:
            mods.alt = pressed;
            break;

        case KEY_CAPSLOCK:
            if (pressed) {
                mods.caps = !mods.caps; // toggle on press
            }
            break;

        default:
            break;
    }
}

char keymap_translate(keycode_t key) {
    char c = 0;

    /* Letters */
    if (key >= KEY_A && key <= KEY_Z) {
        c = 'a' + (key - KEY_A);

        if (mods.shift ^ mods.caps) {
            c = 'A' + (key - KEY_A);
        }
        return c;
    }

    /* Numbers */
    if (key >= KEY_1 && key <= KEY_0) {
        int idx = key - KEY_1;
        const char numbers[] = "1234567890";

        c = numbers[idx];

        if (mods.shift && shift_map[(int)c]) {
            c = shift_map[(int)c];
        }
        return c;
    }

    /* Special keys */
    switch (key) {
        case KEY_SPACE: return ' ';
        case KEY_ENTER: return '\n';
        case KEY_BACKSPACE: return '\b';
        case KEY_TAB: return '\t';
        default: return 0;
    }
}