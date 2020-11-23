#include "IRQ_handlers.h"
#include "keyboard.h"
#include "terminal.h"
#include "print.h"
#include "system.h"


u8 keymap[128] = 
    "\0"
    "\033" // Esc (27)
    "1234567890-=\b"
    "\t" // Tab
    "qwertyuiop[]"
    "\n" // Enter
    "\0" // Ctrl
    "asdfghjkl;'`"
    "\0" // Left Shift
    "\\zxcvbnm,./"
    "\0" // Right Shift
    "*"
    "\0" // Alt
    " " // Space
    "\0" // CapsLock
    "\0\0\0\0\0\0\0\0\0\0" // Function keys F1 - F10
    "\0" // NumLock
    "\0" // Scroll Lock
    "\0" // Home
    "\0" // Up
    "\0" // PgUp
    "\0" // Left
    "\0" // Right
    "\0" // End
    "\0" // Down
    "\0" // PgDn
    "\0" // Insert
    "\0" // Delete
    "\0\0\0"
    "\0" // F11
    "\0" // F12
;

void init_keyboard(void)
{
    irq_new_call(KEYBOARD_IRQ_LINE, keyboard_interrupt);
    print(SERIAL, "[Kernel] Keyboard initalized\n");
}

void keyboard_interrupt(InterruptSave *is)
{
    u8 scancode = in(0x60);

	if (!(scancode & 0x80)) // else key relasedc
		terminal_putc(keymap[scancode]);
   
}