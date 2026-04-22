#ifndef __PIC_DRIVER_H
#define __PIC_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct pic_driver
{
    const char *name;
    bool (*probe)();
    void (*remap)(uint8_t offset_pic1, uint8_t offset_pic2, bool auto_eoi);
    void (*disable)();
    void (*send_eoi)(int irq);
    void (*mask)(int irq);
    void (*unmask)(int irq);
} pic_driver_t;

#endif