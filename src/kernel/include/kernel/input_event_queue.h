#ifndef __INPUT_EVENT_QUEUE_H
#define __INPUT_EVENT_QUEUE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum input_event_type
{
    INPUT_EVENT_NONE = 0,
    INPUT_EVENT_KEY,
    INPUT_EVENT_MOUSE_MOVE,
    INPUT_EVENT_MOUSE_BUTTON
} input_event_type_t;

typedef struct input_event
{
    input_event_type_t type;
    union 
    {
        struct 
        {
            int keycode;
            uint8_t scancode;
            bool pressed;
        } key;

        struct 
        {
            int dx;
            int dy;
        } mouse_move;

        struct 
        {
            uint8_t button;
            bool pressed;
        } mouse_button;
    };
} input_event_t;

/* Core API */
void input_queue_init(void);
bool input_queue_push_event(const input_event_t *ev);
bool input_queue_pop_event(input_event_t *ev);

#endif 