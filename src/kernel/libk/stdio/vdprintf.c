#include <stdio.h>

static const char HEX_CHARS[] = "0123456789abcdef";

int dprint_unsigned(int fd, unsigned int number, int radix) {
    int count = 0;
    char buffer[32];
    int pos = 0;

    // convert number to ASCII
    do {
        unsigned long long rem = number % radix;
        number /= radix;
        buffer[pos++] = HEX_CHARS[rem];
    } while (number > 0);

    // print number in reverse order
    while (--pos >= 0) {
        dputc(buffer[pos], fd);
        ++count;
    }

    return count;
}

int dprint_signed(int fd, int number, int radix) {
    int count = 0;
    if (number < 0) {
        putchar('-');
        ++count;
        count += dprint_unsigned(fd, -number, radix);
    } else {
        count += dprint_unsigned(fd, number, radix);
    }
    return count;
}

enum printf_state
{
    PRINTF_STATE_DEFAULT,
    PRINTF_STATE_FORMAT_SPEC
};

int vdprintf(int fd, const char *fmt, va_list ap) {
    int bytes = 0;
    enum printf_state state = PRINTF_STATE_DEFAULT;
    while (*fmt) {
        switch (state) {
        case PRINTF_STATE_DEFAULT:
            if (*fmt == '%') {
                state = PRINTF_STATE_FORMAT_SPEC;
            } else {
                dputc(*fmt, fd);
                ++bytes;
            }
            break;

        case PRINTF_STATE_FORMAT_SPEC:
            switch (*fmt) {
            case '%':
                dputc('%', fd);
                ++bytes;
                break;

            case 'c':
                dputc(va_arg(ap, int), fd);
                ++bytes;
                break;

            case 's':
                bytes += dputs(va_arg(ap, const char*), fd);
                break;
            
            case 'd':
                bytes += dprint_signed(fd, va_arg(ap, int), 10);
                break;

            case 'u':
                bytes += dprint_unsigned(fd, va_arg(ap, unsigned int), 10);
                break;

            case 'x':
                bytes += dprint_unsigned(fd, va_arg(ap, unsigned int), 16);
                break;

            default:    // invalid specifier
                break;
            }
            state = PRINTF_STATE_DEFAULT;
            break;

        default:
            break;
        }
        ++fmt;
    }
    return bytes;
}