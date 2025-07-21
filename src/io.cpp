#include "io.h"
#include "base_core.h"
#include "cpu.h"
#include "gamepad.h"
#include "lcd.h"
#include "timer.h"
#include <cstdio>

glob char serial_data[2];

u8 io_read(u16 address)
{
 if (address == 0xff00)
 {
  return gamepad_get_output();
 }
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

 if (BETWEEN(address, 0xff10, 0xff3f))
 {
   // ignore sound
   return 0;
 }

 if (BETWEEN(address, 0xff40, 0xff4b))
 {
  return lcd_read(address);
 }

 printf("UNSUPPORTED bus_read(%04X)\n", address);
 return 0;
}

void io_write(u16 address, u8 value)
{
 if (address == 0xff00)
 {
  gamepad_set_sel(value);
  return;
 }
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
  return;
 }

 if (address == 0xff0f)
 {
  cpu_set_int_flags(value);
  return;
 }

 if (BETWEEN(address, 0xff10, 0xff3f))
 {
   // ignore sound
   return;
 }

 if (BETWEEN(address, 0xff40, 0xff4b))
 {
  lcd_write(address, value);
  return;
 }

 printf("UNSUPPORTED bus_write(%04X)\n", address);
}
