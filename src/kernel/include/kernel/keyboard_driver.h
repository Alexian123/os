#ifndef __KEYBOARD_DRIVER_H
#define __KEYBOARD_DRIVER_H 1

#include <stdint.h>
#include <stdbool.h>

typedef struct keyboard_driver 
{
    const char *name;
    void (*init)(void);
} keyboard_driver_t;

const keyboard_driver_t *get_keyboard_driver(void);

#endif