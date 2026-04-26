#ifndef __PIT_DRIVER_H
#define __PIT_DRIVER_H

#include <stdint.h>

typedef enum pit_channel
{
    PIT_CHANNEL0,
    PIT_CHANNEL1,
    PIT_CHANNEL2
} pit_channel_t;

/*
    Bits         Usage
    7 and 6      Select channel :
                    0 0 = Channel 0
                    0 1 = Channel 1
                    1 0 = Channel 2
                    1 1 = Read-back command (8254 only)
    5 and 4      Access mode :
                    0 0 = Latch count value command
                    0 1 = Access mode: lobyte only
                    1 0 = Access mode: hibyte only
                    1 1 = Access mode: lobyte/hibyte
    3 to 1       Operating mode :
                    0 0 0 = Mode 0 (interrupt on terminal count)
                    0 0 1 = Mode 1 (hardware re-triggerable one-shot)
                    0 1 0 = Mode 2 (rate generator)
                    0 1 1 = Mode 3 (square wave generator)
                    1 0 0 = Mode 4 (software triggered strobe)
                    1 0 1 = Mode 5 (hardware triggered strobe)
                    1 1 0 = Mode 2 (rate generator, same as 010b)
                    1 1 1 = Mode 3 (square wave generator, same as 011b)
    0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
*/
enum pit_cmd
{
    PIT_CMD_BINARY_MODE                     = 0x00,
    PIT_CMD_BCD_MODE                        = 0x01,

    PIT_CMD_OPMODE0                         = 0x00,     // interrupt on terminal count
    PIT_CMD_OPMODE1                         = 0x02,     // hardware re-triggerable one-shot
    PIT_CMD_OPMODE2                         = 0x04,     // rate generator
    PIT_CMD_OPMODE3                         = 0x06,     // square wave generator
    PIT_CMD_OPMODE4                         = 0x08,     // software triggered strobe
    PIT_CMD_OPMODE5                         = 0x0A,     // hardware triggered strobe
    PIT_CMD_OPMODE2_1                       = 0x0C,     // rate generator, same as mode 2
    PIT_CMD_OPMODE3_1                       = 0x0E,     // quare wave generator, same as mode 3

    PIT_CMD_LATCH_COUNT_VAL                 = 0x00,
    PIT_CMD_ACCESS_LO_ONLY                  = 0x10,
    PIT_CMD_ACCESS_HI_ONLY                  = 0x20,
    PIT_CMD_ACCESS_LO_HI                    = 0x30,

    PIT_CMD_SELECT_CH0                      = 0x00,
    PIT_CMD_SELECT_CH1                      = 0x40,
    PIT_CMD_SELECT_CH2                      = 0x80,
    PIT_CMD_SELECT_READ_BACK                = 0xC0,

    PIT_READ_BACK_CMD_NO_LATCH_COUNT        = 0x20,
    PIT_READ_BACK_CMD_NO_LATCH_STATUS       = 0x10,
    PIT_READ_BACK_CMD_CH2                   = 0x08,
    PIT_READ_BACK_CMD_CH1                   = 0x04,
    PIT_READ_BACK_CMD_CH0                   = 0x02
};

typedef struct pit_driver
{
    const char *name;
    uint16_t (*read_reload_value)(pit_channel_t channel);
    void (*set_reload_value)(pit_channel_t channel, uint16_t reload_value);
    void (*configure)(uint8_t cmd_flags);
} pit_driver_t;

const pit_driver_t *get_pit_i8254_driver();

#endif