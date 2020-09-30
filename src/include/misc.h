#pragma once

#include "ints.h"

void permahalt(void);
void int_wait_forever(void);
void out(u16 port, u8 data);
u8 in(u16 port);