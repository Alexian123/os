#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Hardware text mode color constants. */
typedef enum 
{
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15
} VGA_COLOR;

static inline uint8_t vga_entry_color(VGA_COLOR fg, VGA_COLOR bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000 

static size_t screen_row;
static size_t screen_column;
static uint8_t screen_color;
static uint16_t* vga_buffer = (uint16_t*)VGA_MEMORY;

void vga_clearscreen(void)  {
	screen_row = 0;
	screen_column = 0;
	screen_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			vga_buffer[index] = vga_entry(' ', screen_color);
		}
	}
}

void vga_setcolor(uint8_t color)  {
	screen_color = color;
}

void vga_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	vga_buffer[index] = vga_entry(c, color);
}

void vga_scroll_down(void) {
    if (++screen_row == VGA_HEIGHT) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
                const size_t index = y * VGA_WIDTH + x;
                const size_t index_next = (y + 1) * VGA_WIDTH + x;
                vga_buffer[index] = vga_buffer[index_next];
            }
            const size_t last_index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
            vga_buffer[last_index] = vga_entry(' ', screen_color);
        }
        --screen_row;
    }
}

void vga_putchar(char c)  {
	if (c == '\n') {
        screen_column = 0;
		vga_scroll_down();
        return;
    }
	if (c == '\r') {
		screen_column = 0;
		return;
	}
	vga_putentryat(c, screen_color, screen_column, screen_row);
	if (++screen_column == VGA_WIDTH) {
		screen_column = 0;
		if (++screen_row == VGA_HEIGHT) {
			screen_row = 0;
		}
	}
}