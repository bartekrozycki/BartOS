#pragma once
#include "IRQ_handlers.h"

#define PROGRAMMABLE_INTERRUPT_TIMER_LINE 0

// I/O port     Usage
// 0x40         Channel 0 data port (read/write)
// 0x41         Channel 1 data port (read/write)
// 0x42         Channel 2 data port (read/write)
// 0x43         Mode/Command register (write only, a read is ignored)

#define PIT_CHANNEL_0        0x40
#define PIT_CHANNEL_1        0x41
#define PIT_CHANNEL_2        0x42
#define PIT_COMMAND_REGISTER 0x43

#define PIT_MODE0            0x0
#define PIT_MODE1            0x1
#define PIT_MODE2            0x2
#define PIT_MODE3            0x3
#define PIT_MODE4            0x4
#define PIT_MODE5            0x5

#define PIT_ACCESS_LATCH     0x0
#define PIT_ACCESS_LOW       0x1
#define PIT_ACCESS_HIGH      0x2
#define PIT_ACCESS_LOHI      0x3

void init_pit(void);
void pit_interrupt(InterruptSave *is);
