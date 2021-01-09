#include "interrupts_handlers.h"
#include "keyboard.h"
#include "vga.h"
#include "print.h"
#include "kernel_heap_malloc.h"
#include "io_bus.h"
#include "threads.h"
#include "kernel_panic.h"

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
    print(SERIAL, "info Keyboard initalized\n\n");
}

void keyboard_interrupt(InterruptSave *is)
{
    u8 scancode = in(0x60);

    // scanf lock_mutex & getc lock_mutex == 2
    if (current_focus_tcb && current_focus_tcb->kbp_queue->sync->current_count < 1)
        return;

    KBP *packet = (KBP *) malloc(sizeof(KBP));
    packet->key_code = (u16) scancode;

    key_queue_push(packet);
}