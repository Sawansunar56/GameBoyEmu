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
 // u8 flags;

 // NOTE: SOWN this seems to be a weird error. if you use only unsigned, it caused
 // the size of the struct to be expanded up to 8bytes which shouldn't happen and 
 // should be only 4 bytes.  probably because 
 u8 f_cgb_pn : 3;
 u8 f_cgb_vram_bank : 1;
 u8 f_pn : 1;
 u8 f_x_flip : 1;
 u8 f_y_flip : 1;
 u8 f_bgp : 1;
};

struct oam_line_entry {
  oam_entry entry;
  oam_line_entry* next;
};

struct ppu_context
{
 oam_entry oam_ram[40];
 u8 vram[0x2000];

 u8 line_sprite_count; // 0 to 10 sprites
 oam_line_entry *line_sprites; // linked list of current sprites on line.
 oam_line_entry line_entry_array[10]; // memory to use for list.

 u8 fetched_entry_count;
 oam_entry fetched_entries[3]; // entires fetched during pipeline

 u8 window_line;
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
