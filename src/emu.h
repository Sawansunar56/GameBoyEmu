#pragma once

#include "base_core.h"

typedef struct {
    b8 paused;
    b8 running;
    b8 die;
    u64 ticks;
} emu_context;

PURE i32 emu_run(int argc, char** argv);

emu_context *emu_get_context();

void emu_cycles(int cpu_cycles);
