#pragma once

#include "base_core.h"

void dma_start(u8 start);
void dma_tick();

PURE b8 dma_transferring();
