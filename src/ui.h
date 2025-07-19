#pragma once
#include "base_core.h"

glob constexpr s32 SCREEN_WIDTH  = 1024;
glob constexpr s32 SCREEN_HEIGHT = 768;

void ui_init();
void ui_update();
void ui_handle_events();
