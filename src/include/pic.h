#pragma once

// I/O port     Usage
// 0x40         Channel 0 data port (read/write)
// 0x41         Channel 1 data port (read/write)
// 0x42         Channel 2 data port (read/write)
// 0x43         Mode/Command register (write only, a read is ignored)

#define PIC_CHANNEL_0        0x40
#define PIC_CHANNEL_1        0x41
#define PIC_CHANNEL_2        0x42
#define PIC_COMMAND_REGISTER 0x43

#define PIC_MODE0            0x0
#define PIC_MODE1            0x1
#define PIC_MODE2            0x2
#define PIC_MODE3            0x3
#define PIC_MODE4            0x4
#define PIC_MODE5            0x5

#define PIC_ACCESS_LATCH     0x0
#define PIC_ACCESS_LOW       0x1
#define PIC_ACCESS_HIGH      0x2
#define PIC_ACCESS_LOHI      0x3

#define PIC_CHANNEL_0        0x0
#define PIC_CHANNEL_1        0x1
#define PIC_CHANNEL_2        0x2
#define PIC_CHANNEL_RBC      0x3

