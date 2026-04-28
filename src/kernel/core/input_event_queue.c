#include <kernel/core/input_event_queue.h>

#define INPUT_QUEUE_SIZE 128

static input_event_t queue[INPUT_QUEUE_SIZE];
static volatile int head = 0;
static volatile int tail = 0;

void input_queue_init(void) {
    head = tail = 0;
}

bool input_queue_push_event(const input_event_t *ev) {
    int next = (head + 1) % INPUT_QUEUE_SIZE;

    if (next == tail) {
        // queue full → drop event
        return false;
    }

    queue[head] = *ev;
    head = next;
    return true;
}

bool input_queue_pop_event(input_event_t *ev) {
    if (head == tail)
        return false;

    *ev = queue[tail];
    tail = (tail + 1) % INPUT_QUEUE_SIZE;
    return true;
}