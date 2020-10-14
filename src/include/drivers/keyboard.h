#pragma once

#include "IRQ_handlers.h"

#define KEYBOARD_IRQ_LINE 1

void keyboard_init(void);
void keyboard_interrupt(InterruptSave *is);
