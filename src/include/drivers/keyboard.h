#pragma once

#include "IRQ_handlers.h"

#define KEYBOARD_IRQ_LINE 1

void init_keyboard(void);
void keyboard_interrupt(InterruptSave *is);
