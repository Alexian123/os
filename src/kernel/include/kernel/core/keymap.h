#ifndef __KEYMAP_H
#define __KEYMAP_H 1

#include <stdbool.h>

#include <kernel/core/keycodes.h>

void keymap_update_modifiers(keycode_t key, bool pressed);
char keymap_translate(keycode_t key);

#endif