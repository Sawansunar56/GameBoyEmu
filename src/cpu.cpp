#include "cpu.h"
#include "bus.h"
#include "dbg.h"
#include "emu.h"
#include "instructions.h"
#include "interrupts.h"
#include "timer.h"
#include <stdio.h>


cpu_context ctx = {0};

#define CPU_DEBUG 0

void cpu_init()
{
 ctx.regs.pc             = 0x100;
 ctx.regs.sp             = 0xfffe;
 *((short *)&ctx.regs.a) = 0xb001;
 *((short *)&ctx.regs.b) = 0x1300;
 *((short *)&ctx.regs.d) = 0xD800;
 *((short *)&ctx.regs.h) = 0x4d01;
 ctx.ie_register         = 0;
 ctx.int_flags           = 0;
 ctx.int_master_enabled  = false;
 ctx.enabling_ime        = false;

 timer_get_context()->div = 0xABCC;
}

function void fetch_instruction()
{
 ctx.cur_opcode = bus_read(ctx.regs.pc++);
 ctx.cur_inst   = instruction_by_opcode(ctx.cur_opcode);
}

function void execute()
{
 IN_PROC proc = inst_get_processor(ctx.cur_inst->type);

 if (!proc)
 {
  NO_IMPL
 }

 proc(&ctx);
 // printf("Not executing yet\n");
}

b8 cpu_step()
{
 if (!ctx.halted)
 {
  u16 pc = ctx.regs.pc;

  fetch_instruction();
  emu_cycles(1);
  fetch_data();

#if CPU_DEBUG == 1
  char flags[16];

  sprintf(flags,
          "%c%c%c%c",
          ctx.regs.f & (1 << 7) ? 'Z' : '-',
          ctx.regs.f & (1 << 6) ? 'N' : '-',
          ctx.regs.f & (1 << 5) ? 'H' : '-',
          ctx.regs.f & (1 << 4) ? 'C' : '-');

  std::string inst = inst_to_str(&ctx);

  printf("%08llX - %04X: %-12s (%02X %02X %02X) A: %02X F: %s BC: %02X%02X DE: "
         "%02X%02X HL: "
         "%02x%02X\n",
         emu_get_context()->ticks,
         pc,
         inst.c_str(),
         ctx.cur_opcode,
         bus_read(pc + 1),
         bus_read(pc + 2),
         ctx.regs.a,
         flags,
         ctx.regs.b,
         ctx.regs.c,
         ctx.regs.d,
         ctx.regs.e,
         ctx.regs.h,
         ctx.regs.l);

#endif

  if (ctx.cur_inst == NULL)
  {
   printf("Unknown Instruction! %02X\n", ctx.cur_opcode);
   exit(-7);
  }

  dbg_update();
  dbg_print();

  execute();
 }
 else
 {
  emu_cycles(1);

  if (ctx.int_flags)
  {
   ctx.halted = false;
  }
 }

 if (ctx.int_master_enabled)
 {
  cpu_handle_interrupt(&ctx);
  ctx.enabling_ime = false;
 }

 if (ctx.enabling_ime)
 {
  ctx.int_master_enabled = true;
 }
 return true;
}

u8 cpu_get_ie_register() { return ctx.ie_register; }

void cpu_set_ie_register(u8 n) { ctx.ie_register = n; }

u8 cpu_get_int_flags() { return ctx.int_flags; }

void cpu_set_int_flags(u8 value) { ctx.int_flags = value; }

void cpu_request_interrupt(interrupt_type t) {
  ctx.int_flags |= t;
}
