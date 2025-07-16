#pragma once

#include "cpu.h"

typedef enum {
  IT_VBLANK  = (1 << 0),
  IT_LCD_STAT = (1 << 1),
  IT_TIMER    = (1 << 2),
  IT_SERIAL   = (1 << 3),
  IT_JOYPAD   = (1 << 4),
} interrupt_type;

void cpu_request_interrupt(interrupt_type t);
void cpu_handle_interrupt(cpu_context *ctx);

