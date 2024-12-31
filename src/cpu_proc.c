#include "cpu.h"
#include "emu.h"

function void proc_none(cpu_context *ctx)
{
  printf("INVALID INSTRUCTION\n");
  exit(-7);
}

function void proc_ld(cpu_context *ctx) {}

function b8 check_cond(cpu_context *ctx)
{
  b8 z = CPU_FLAG_Z;
  b8 c = CPU_FLAG_C;

  switch (ctx->cur_inst->cond)
  {
  case CT_NONE:
    return true;
  case CT_C:
    return c;
  case CT_NC:
    return !c;
  case CT_Z:
    return z;
  case CT_NZ:
    return !z;
  }

  return false;
}

function void proc_jp(cpu_context *ctx)
{
  if (check_cond(ctx))
  {
    ctx->regs.pc = ctx->fetched_data;
    emu_cycles(1);
  }
}

function void proc_nop(cpu_context *ctx) 
{
}

glob IN_PROC processors[] = {
    [IN_NONE] = proc_none,
    [IN_NOP]  = proc_nop,
    [IN_LD]   = proc_ld,
    [IN_JP]   = proc_jp,
};

IN_PROC inst_get_processor(in_type type) { return processors[type]; }
