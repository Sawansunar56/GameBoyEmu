#pragma once

#include "base_core.h"

struct gamepad_state
{
 b8 start;
 b8 select;
 b8 a;
 b8 b;
 b8 up;
 b8 down;
 b8 left;
 b8 right;
};

void gamepad_init();
PURE b8 gamepad_button_sel();
PURE b8 gamepad_dir_sel();
void gamepad_set_sel(u8 value);

PURE gamepad_state *gamepad_get_state();
PURE u8 gamepad_get_output();
