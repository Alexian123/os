#ifndef __PIC_8259_H
#define __PIC_8259_H

#include <kernel/pic_driver.h>

#define PIC_REMAP_OFFSET 32

const pic_driver_t *get_pic_driver();

#endif