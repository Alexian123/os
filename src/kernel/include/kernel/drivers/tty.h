#ifndef __TTY_H
#define __TTY_H 1

#include <stdbool.h>

void tty_init(void);

bool tty_line_available(void);

const char *tty_get_line(void);

void tty_update(void);   // non-blocking

/* output */
void tty_write(const char *str);

#endif