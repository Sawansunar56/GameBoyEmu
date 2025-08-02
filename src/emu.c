#include "SDL.h"
#include <stdio.h>
#include "emu.h"
#include "cart.h"
#include "cpu.h"
#include "SDL_ttf.h"

glob emu_context ctx;

// comps
// : cpu
// : Address bus
// : PPU
// : Timer
// : Cartridge

emu_context *emu_get_context() { return &ctx; }

void delay(u32 ms) { SDL_Delay(ms); }

int emu_run(int argc, char **argv)
{
  if (argc < 2)
  {
    printf("Usage: emu <rom_file>\n");
    return -1;
  }

  if (!cart_load(argv[1]))
  {
    printf("Failed to load ROM file: %s\n", argv[1]);
    return -2;
  }

  printf("Cart loaded..\n");

  SDL_Init(SDL_INIT_VIDEO);
  printf("SDL INIT\n");
  TTF_Init();
  printf("TTF INIT\n");

  cpu_init();

  ctx.running = 1;
  ctx.paused  = 0;
  ctx.ticks   = 0;
  while (ctx.running)
  {
    if (ctx.paused)
    {
      delay(10);
      continue;
    }

    if (!cpu_step())
    {
      printf("CPU stopped\n");
      return -3;
    }

    ctx.ticks++;
  }
  return 0;
}

void emu_cycles(int cpu_cycles) {}
