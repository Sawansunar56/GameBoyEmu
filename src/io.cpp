#include "io.h"
#include "base_core.h"
#include "cpu.h"
#include "dma.h"
#include "timer.h"
#include <cstdio>

glob char serial_data[2];

u8 ly = 0;
u8 io_read(u16 address)
{
 if (address == 0xff01)
 {
  return serial_data[0];
 }

 if (address == 0xff02)
 {
  return serial_data[1];
 }

 if (BETWEEN(address, 0xff04, 0xff07))
 {
  return timer_read(address);
 }

 if (address == 0xff0f)
 {
  return cpu_get_int_flags();
 }

 if(address == 0xff44) {
   return ly++;
 }


 printf("UNSUPPORTED bus_read(%04X)\n", address);
 return 0;
}

void io_write(u16 address, u8 value)
{
 if (address == 0xff01)
 {
  serial_data[0] = value;
  return;
 }

 if (address == 0xff02)
 {
  serial_data[1] = value;
  return;
 }

 if (BETWEEN(address, 0xff04, 0xff07))
 {
  timer_write(address, value);
 }

 if (address == 0xff0f)
 {
  cpu_set_int_flags(value);
 }

 if (address == 0xff46)
 {
  dma_start(value);
  printf("DMA start\n");
 }

 printf("UNSUPPORTED bus_write(%04X)\n", address);
}
