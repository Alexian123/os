#include <kernel/tty.h>
#include <kernel/input_event_queue.h>
#include <kernel/vga_io.h>
#include <kernel/keymap.h>
#include <string.h>

#define TTY_BUFFER_SIZE 256

static char line_buffer[TTY_BUFFER_SIZE];
static int  line_len = 0;

static char ready_line[TTY_BUFFER_SIZE];
static bool line_ready = false;

void tty_init(void) {
    line_len = 0;
    line_ready = false;
    vga_clearscreen();
}

void tty_update(void) {
    input_event_t ev;

    while (input_queue_pop_event(&ev)) {
        if (ev.type != INPUT_EVENT_KEY)
            continue;

        keymap_update_modifiers(ev.key.keycode, ev.key.pressed);

        if (!ev.key.pressed)
            continue;

        char c = keymap_translate(ev.key.keycode);

        if (!c)
            continue;

        switch (c) {
        case '\n':
            vga_putchar('\n');

            /* finalize line */
            line_buffer[line_len] = '\0';
            
            memcpy(ready_line, line_buffer, line_len + 1);

            line_len = 0;
            line_ready = true;
            break;

        case '\b':
            if (line_len > 0) {
                line_len--;
                vga_putchar('\b'); vga_putchar(' '); vga_putchar('\b');
            }
            break;

        default:
            if (line_len < TTY_BUFFER_SIZE - 1) {
                line_buffer[line_len++] = c;
                vga_putchar(c);
            }
            break;
        }
    }
}

bool tty_line_available(void) {
    return line_ready;
}

const char *tty_get_line(void) {
    line_ready = false;
    return ready_line;
}

void tty_write(const char *str) {
    while (*str) {
        vga_putchar(*str++);
    }
}