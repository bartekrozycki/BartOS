#pragma once

#include "ints.h"

#define VGA_COMMAND_PORT                        0x3D4
#define VGA_DATA_PORT                           0x3D5
#define VGA_CURSOR_HIGH                         14
#define VGA_CURSOR_LOW                          15

#define VGA_WIDTH                               80
#define VGA_HEIGHT                              25
#define TERMINAL_VIDEO_MEMORY_ADDRESS           (u16 *) 0xF0000000

#define TERMINAL_COLOR_DOS_DEFAULT              0x07
#define TERMINAL_COLOR_BLUE_SCREEN_OF_DEATH     0x1F
#define TERMINAL_COLOR_NOSTALGIC                0x2A

enum vga_color {
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
	VGA_COLOR_WHITE = 15,
};

static inline u8 vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline u16 vga_entry(u8 asci, u8 color) {
	return (u16) asci | (u16) color << 8;
}
void entry_at(u8 x, u8 y, u8 color, char c);
void init_terminal(void);
void terminal_clear(void);
void terminal_setcolor(uint8_t color);
void terminal_putc(char c);
void terminal_write(const char* data, u32 size);
int terminal_writestring(const char* data);
int terminal_write_base(u32 d, u32 base);

void terminal_pos(size_t x, size_t y);
size_t getX();
size_t getY();
