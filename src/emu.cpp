#include <SDL.h>
#include <chrono>
#include <stdio.h>
#include "emu.h"
#include "audio.h"
#include "cart.h"
#include "cpu.h"
#include "dma.h"
#include "ppu.h"
#include "timer.h"
#include "ui.h"
#include <stop_token>
#include <thread>

glob emu_context ctx;

// comps
// : cpu
// : Address bus
// : PPU
// : Timer
// : Cartridge

emu_context *emu_get_context() { return &ctx; }

void delay(u32 ms) { SDL_Delay(ms); }

void cpu_run(std::stop_token stop_token)
{
 timer_init();
 cpu_init();
 ppu_init();

 ctx.running = 1;
 ctx.paused  = 0;
 ctx.ticks   = 0;

 while (!stop_token.stop_requested())
 {
  if (ctx.paused)
  {
   // delay(10);
   std::this_thread::sleep_for(std::chrono::milliseconds(10));
   continue;
  }

  if (!cpu_step())
  {
   printf("CPU stopped\n");
   break;
  }
 }
 printf("from the worker thread\n");
}

s32 emu_run(int argc, char **argv)
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

 ui_init();

 // SDL_Thread *t1;
 // t1 = SDL_CreateThread(cpu_run, "CPU running thread", NULL);
 //
 // if (t1 == NULL)
 // {
 //  printf("SDL_Create thread failed: %s\n", SDL_GetError());
 //  SDL_Quit();
 //  return 1;
 // }
 std::jthread cpu_thread(cpu_run);

 u32 prev_frame = 0;
 while (!ctx.die)
 {
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  ui_handle_events();

  if (prev_frame != ppu_get_context()->current_frame)
  {
   ui_update();
  }

  prev_frame = ppu_get_context()->current_frame;
 }

 cpu_thread.request_stop();
 printf("from the main thread bro\n");

 SDL_Quit();
 return 0;
}

void emu_cycles(i32 cpu_cycles)
{
 for (int i = 0; i < cpu_cycles; i++)
 {
  for (int n = 0; n < 4; n++)
  {
   ctx.ticks++;
   timer_tick();
   ppu_tick();
   audio_tick();
  }

  dma_tick();
 }
}
