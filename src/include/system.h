#pragma once

#include "ints.h"
#include "print.h"

#define PIC1		    0x20		/* IO base address for master PIC */
#define PIC2		    0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	    (PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	    (PIC2+1)

#define PIC_EOI		0x20

void int_wait_forever(void);
void out(u16 port, u8 data);
u8 in(u16 port);