#pragma once

#include "base_core.h"

struct timer_context {
  u16 div;
  u8 tima;
  u8 tma;
  u8 tac;
};

void timer_init();
void timer_tick();

void timer_write(u16 address, u8 value);
u8 timer_read(u16 address);

timer_context* timer_get_context();
