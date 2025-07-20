#pragma once

#include "base_core.h"

glob constexpr s32 LINES_PER_FRAME = 154;
glob constexpr s32 TICKS_PER_LINE  = 456;
glob constexpr s32 YRES            = 144;
glob constexpr s32 XRES            = 160;

enum fetch_state
{
 FS_TILE,
 FS_DATA0,
 FS_DATA1,
 FS_IDLE,
 FS_PUSH
};

struct fifo_entry
{
 fifo_entry *next;
 u32 value;
};

struct fifo
{
 fifo_entry *head;
 fifo_entry *tail;
 u32 size;
};

struct pixel_fifo_context
{
 fetch_state cur_fetch_state;
 fifo pixel_fifo;
 u8 line_x;
 u8 pushed_x;
 u8 fetch_x;
 u8 bgw_fetch_data[3];
 u8 fetch_entry_data[6]; // oam data...
 u8 map_y;
 u8 map_x;
 u8 tile_y;
 u8 fifo_x;
};

struct oam_entry
{
 u8 y;
 u8 x;
 u8 tile;
 u8 flags;

 unsigned f_cgb_pn : 3;
 unsigned f_cgb_vram_bank : 1;
 unsigned f_pn : 1;
 unsigned f_x_flip : 1;
 unsigned f_y_flip : 1;
 unsigned f_bgp : 1;
};

struct ppu_context
{
 oam_entry oam_ram[40];
 u8 vram[0x2000];

 pixel_fifo_context pfc;

 u32 current_frame;
 u32 line_ticks;
 u32 *video_buffer;
};

void ppu_init();
void ppu_tick();

void ppu_oam_write(u16 address, u8 value);
PURE u8 ppu_oam_read(u16 address);

void ppu_vram_write(u16 address, u8 value);
PURE u8 ppu_vram_read(u16 address);

PURE ppu_context *ppu_get_context();
