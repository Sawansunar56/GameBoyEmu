#include "dbg.h"
#include "bus.h"
#include <cstdio>

glob char dbg_msg[1024] = {0};
glob int msg_size = 0;

void dbg_update()
{
  if(bus_read(0xff02) == 0x81) {
    char c = bus_read(0xff01);

    dbg_msg[msg_size++] = c;

    bus_write(0xff02, 0);
  }
}
void dbg_print()
{
  if(dbg_msg[0]) {
    printf("DBG: %s\n", dbg_msg);
  }
}
