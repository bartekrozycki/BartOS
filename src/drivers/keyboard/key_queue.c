//
// Created by bartosz on 03.01.2021.
//

#include "keyboard.h"
#include "threads.h"
static inline void detectPressed(u16 key_code)
{
    switch (key_code) {
        case KEY_PRESSED_left_shift:
        case KEY_PRESSED_right_shift:
            current_focus_tcb->kbp_queue->status.shift_pressed = 1;
        case KEY_RELEASED_left_shift:
        case KEY_RELEASED_right_shift:
            current_focus_tcb->kbp_queue->status.shift_pressed = 0;
    }
}
void lock_key_queue()
{
    lock_mutex(current_focus_tcb->kbp_queue->sync);
}
void unlock_key_queue()
{
    unlock_mutex(current_focus_tcb->kbp_queue->sync);
}
void key_queue_push(KBP *packet)
{
    kbp_queue *current = current_focus_tcb->kbp_queue;

    if (!packet) return;

    detectPressed(packet->key_code);

    packet->next = NULL;
    if (current->front == NULL)
    {
        current->front = packet;
        current->back = packet;
    }
    else {
        current->back->next = packet;
        current->back = packet;
    }
    unlock_key_queue();
}

KBP* key_queue_pop(void)
{
    kbp_queue *current = current_focus_tcb->kbp_queue;

    if (current->front == NULL)
        return NULL;

    KBP * ret;

    ret  = current->back;

    if (current->back == current->front)
    {
        current->back = NULL;
        current->front = NULL;
    }
    else {
        KBP *ptr;
        for (ptr = current->front; ptr->next != current->back; ptr = ptr->next);
        current->back = ptr;
    }

    return ret;
}
