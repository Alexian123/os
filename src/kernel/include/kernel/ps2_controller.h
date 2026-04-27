#ifndef __PS2_CONTROLLER_H
#define __PS2_CONTROLLER_H 1

#include <stdint.h>

#include <stdbool.h>
typedef struct ps2_controller_driver
{
    const char *name;

    void (*init)(void);

    bool (*read)(uint8_t *data);
    void (*write)(uint8_t data);

    bool (*read_status)(uint8_t *status);

    void (*enable_port1)(void);
    void (*disable_port1)(void);

} ps2_controller_driver_t;

const ps2_controller_driver_t *get_ps2_controller(void);

#endif