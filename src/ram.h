#pragma once

#include "base_core.h"

PURE u8 wram_read(u16 address);
void wram_write(u16 address, u8 value);

PURE u8 hram_read(u16 address);
void hram_write(u16 address, u8 value);
