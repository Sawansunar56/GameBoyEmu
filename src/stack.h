#pragma once

#include "base_core.h"

void stack_push(u8 data);
void stack_push16(u16 data);

PURE u8 stack_pop();
PURE u16 stack_pop16();
