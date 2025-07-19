#pragma once

#include "base_core.h"

void dma_start(u8 start);
void dma_tick();

b8 dma_transferring();
