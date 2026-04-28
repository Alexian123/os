#ifndef __PIC_DRIVER_H
#define __PIC_DRIVER_H 1

#include <stdint.h>
#include <stdbool.h>

#define PIC_REMAP_OFFSET 32

typedef struct pic_driver
{
    const char *name;
    bool (*probe)(void);
    void (*remap)(uint8_t offset_pic1, uint8_t offset_pic2, bool auto_eoi);
    bool (*is_auto_eoi)(void);
    void (*disable)(void);
    void (*send_eoi)(int irq);
    void (*mask)(int irq);
    void (*unmask)(int irq);
    uint16_t (*read_irr)(void);
    uint16_t (*read_isr)(void);
} pic_driver_t;

const pic_driver_t *get_pic_i8258_driver(void);

#endif