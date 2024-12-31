#pragma once

#include "base_core.h"
#include "instructions.h"

typedef struct {
  u8 a;
  u8 f;
  u8 b;
  u8 c;
  u8 d;
  u8 e;
  u8 h;
  u8 l;
  u16 pc;
  u16 sp;
} cpu_registers;

typedef struct {
  cpu_registers regs;

  u16 fetched_data;
  u16 mem_dest;
  b8 dest_is_mem;
  u8 cur_opcode;
  instruction *cur_inst;

  b8 halted;
  b8 stepping;
} cpu_context;

void cpu_init();
b8 cpu_step();
