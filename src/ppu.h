#pragma once

#include "base_core.h"

struct oam_entry {
  u8 y;
  u8 x;
  u8 tile;
  u8 flags;

  unsigned f_cgb_pn: 3;
  unsigned f_cgb_vram_bank : 1;
  unsigned f_pn: 1;
  unsigned f_x_flip : 1;
  unsigned f_y_flip : 1;
  unsigned f_bgp : 1;
};

struct ppu_context {
  oam_entry oam_ram[40];
  u8 vram[0x2000];
};

void ppu_init();
void ppu_tick();

void ppu_oam_write(u16 address, u8 value);
u8 ppu_oam_read(u16 address);

void ppu_vram_write(u16 address, u8 value);
u8 ppu_vram_read(u16 address);

