#include "ints.h"
#include "terminal.h"
#include "misc.h"
#include "lib.h"

u32     terminal_pos_row, 
        terminal_pos_col;
u8      terminal_color;

u16*    terminal_mem;


void terminal_cursor_set(unsigned char row, unsigned char col)
{
    unsigned short pos = row * VGA_WIDTH + col;

    out(VGA_COMMAND_PORT, VGA_CURSOR_HIGH);
    out(VGA_DATA_PORT, pos >> 8);
    out(VGA_COMMAND_PORT, VGA_CURSOR_LOW);
    out(VGA_DATA_PORT, pos);
}
void terminal_cursor_enable(uint8_t cursor_start, uint8_t cursor_end)
{
	out(0x3D4, 0x0A);
	out(0x3D5, (in(0x3D5) & 0xC0) | cursor_start);
 
	out(0x3D4, 0x0B);
	out(0x3D5, (in(0x3D5) & 0xE0) | cursor_end);
}

void terminal_initialize(void)
{
    terminal_pos_row = 0;
    terminal_pos_col = 0;
    terminal_color   = TERMINAL_COLOR_DOS_DEFAULT;
    terminal_mem     = TERMINAL_VIDEO_MEMORY_ADDRESS;

    terminal_clear();
    
    terminal_cursor_enable(13, 15);

}
void terminal_clear(void)
{
    for (u32 row = 0; row < VGA_HEIGHT; row++)
        for (u32 col = 0; col < VGA_WIDTH; col++)
            *(terminal_mem + (row * VGA_WIDTH) + col) = vga_entry(' ', terminal_color);
}
void terminal_scroll(void)
{
    for (u32 i = VGA_WIDTH; i < VGA_WIDTH * VGA_HEIGHT; i++)
        *(terminal_mem + i) = *(terminal_mem + i + VGA_WIDTH);

    u8 empty = vga_entry(' ', terminal_color);
    for (u32 i = VGA_WIDTH *( VGA_HEIGHT - 1); i < VGA_WIDTH *VGA_HEIGHT; i++)
        *(terminal_mem + i) = empty;

    terminal_pos_col = 0;
    terminal_pos_row--;
}
void terminal_setcolor(unsigned char color)
{
    terminal_color = color;
    for (u32 row = 0; row < VGA_HEIGHT; row++)
        for (u32 col = 0; col < VGA_WIDTH; col++)
            *(terminal_mem + (row * VGA_WIDTH) + col) = vga_entry(*(terminal_mem + (row * VGA_WIDTH) + col), terminal_color);
}
void entry_at(u8 col, u8 row, u8 color, char c)
{
    *(terminal_mem + (row * VGA_WIDTH) + col) = vga_entry(c, color);
    if (col >= VGA_WIDTH)
        terminal_cursor_set(row + 1, 0);
    else
        terminal_cursor_set(row, col+1);
}
void terminal_putchar(char c)
{
    if (c == '\n')
    {
        terminal_pos_col = 0;
        if (++terminal_pos_row >= VGA_HEIGHT)
            terminal_scroll();
        terminal_cursor_set(terminal_pos_row, 0);
        return;
    }
    entry_at(terminal_pos_col, terminal_pos_row, terminal_color, c);

    if (++terminal_pos_col >= VGA_WIDTH)
    {
        terminal_pos_col = 0;
        if (++terminal_pos_row >= VGA_HEIGHT)
        {
            terminal_scroll();
        }
    }
}
void terminal_write(const char* data, u32 size)
{
    if (!data) return;
    
    for (u32 i = 0; i < size; i++)
    {
        terminal_putchar(*(data + i));
    }
    
}
int terminal_writestring(const char* data)
{
    int i = 0;
    while (*(data+i))
        terminal_putchar(*(data+(i++)));
        
    return i;
}
int terminal_write_base(i32 d, u32 base)
{
    char i_dec[32];
    char *x = itoa(d, i_dec, base);
    terminal_writestring(x);

    return d; // TODO    
}