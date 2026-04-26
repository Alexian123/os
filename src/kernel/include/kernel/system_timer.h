#ifndef __SYSTEM_TIMER_H
#define __SYSTEM_TIMER_H

#include <kernel/pit_driver.h>

/*
    Frequency | Resolution | Overhead  | Best Use Case
    -----------------------------------------------------------------------------------
    100 Hz    | 10ms       | Very Low  | Old Linux kernels, simple task switching.
    1000 Hz   | 1ms        | Low       | Standard desktop OS / Sleep functions.
    8000 Hz+  | 0.125ms    | Medium    | High-fidelity audio or high-precision gaming
*/
void system_timer_init(uint32_t frequency);

void system_timer_sleep(uint32_t ms);

#endif