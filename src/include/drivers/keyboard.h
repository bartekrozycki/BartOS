#pragma once

#include "interrupts_handlers.h"
#include "thread_semaphore.h"

#define KEYBOARD_IRQ_LINE 1
typedef enum SCAN_CODE_PRESSED {
    KEY_PRESSED_ESCAPE = 0x1,
    KEY_RELEASED_ESCAPE = 0x81,

    KEY_PRESSED_f1 = 0x3b,
    KEY_RELEASED_f1 = 0xbb,

    KEY_PRESSED_f2 = 0x3c,
    KEY_RELEASED_f2 = 0xbc,

    KEY_PRESSED_f3 = 0x3d,
    KEY_RELEASED_f3 = 0xbd,

    KEY_PRESSED_f4 = 0x3e,
    KEY_RELEASED_f4 = 0xbe,

    KEY_PRESSED_f5 = 0x3f,
    KEY_RELEASED_f5 = 0xbf,

    KEY_PRESSED_f6 = 0x40,
    KEY_RELEASED_f6 = 0xc0,

    KEY_PRESSED_f7 = 0x41,
    KEY_RELEASED_f7 = 0xc1,

    KEY_PRESSED_f8 = 0x42,
    KEY_RELEASED_f8 = 0xc2,

    KEY_PRESSED_f9 = 0x43,
    KEY_RELEASED_f9 = 0xc3,

    KEY_PRESSED_f10 = 0x44,
    KEY_RELEASED_f10 = 0xc4,

    KEY_PRESSED_f11 = 0x57,
    KEY_RELEASED_f11 = 0xd7,

    KEY_PRESSED_f12 = 0xb8,
    KEY_RELEASED_f12 = 0xd8,

    KEY_PRESSED_backspace = 0xe,
    KEY_RELEASED_backspace = 0x8e,

    KEY_PRESSED_left_shift = 0x2a,
    KEY_RELEASED_left_shift = 0xaa,

    KEY_PRESSED_right_shift = 0x36,
    KEY_RELEASED_right_shift = 0xb6,

    KEY_PRESSED_enter = 0x1c,
    KEY_RELEASED_enter = 0x9c,

    KEY_PRESSED_capslock = 0x1c,
    KEY_RELEASED_capslock = 0x9c,


    KEY_PRESSED_arrow_left = 0x1c,
    KEY_RELEASED_arrow_left = 0x9c,


} SCAN_CODE_PRESSED;

// Key Board Packet

typedef struct KBP_T KBP;

union flags{
    u16 flags;
    struct{
        u16 key_pressed:1;
        u16 shift_pressed:1; //
        u16 ctrl_pressed:1;
        u16 alt_pressed:1;
        u16 capslock_active:1;
        u16 num_lock_active:1;
        u16 scroll_lock_active:1;
    };
};

typedef struct kbp_queue_t {
    struct SEMAPHORE_T *sync;
    KBP *front;
    KBP *back;
    union flags status;

} kbp_queue;

struct KBP_T {
    u32 unicode_code;
    u16 key_code;
    union flags *status;
    struct KBP_T *next;
};

kbp_queue *get_keyboard_queue( void );
void init_keyboard(void);
void keyboard_interrupt(InterruptSave *is);

void key_queue_push(KBP *packet);
KBP* key_queue_pop(void);
