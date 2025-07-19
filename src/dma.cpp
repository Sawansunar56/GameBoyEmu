#include "dma.h"
#include "ppu.h"
#include "bus.h"
#include <chrono>
#include <cstdio>
#include <thread>

struct dma_context
{
 b8 active;
 u8 byte;
 u8 value;
 u8 start_delay;
};

glob dma_context ctx;

void dma_start(u8 start)
{
 ctx.active      = true;
 ctx.byte        = 0;
 ctx.start_delay = 2;
 ctx.value       = start;
}
void dma_tick()
{
 if (!ctx.active)
 {
  return;
 }

 if (ctx.start_delay)
 {
  ctx.start_delay--;
  return;
 }

 ppu_oam_write(ctx.byte, bus_read((ctx.value * 0x100) + ctx.byte));

 ctx.byte++;
 ctx.active = ctx.byte < 0xA0;

 if(!ctx.active) {
   printf("DMA DONE!\n");
   std::this_thread::sleep_for(std::chrono::milliseconds(2000));
 }
}

b8 dma_transferring() { return ctx.active; }
