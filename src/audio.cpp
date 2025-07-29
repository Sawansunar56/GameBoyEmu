#include "audio.h"
#include "base_core.h"
#include <cstdlib>

constexpr s32 sampleRate      = 44100;
constexpr s32 cpuFrequency    = 4194304;
constexpr s32 cyclesPerSample = cpuFrequency / sampleRate;

glob APU ctx = {0};

u8 audio_read(u16 addr)
{
 switch (addr)
 {
 case 0xff12:
 }
}

void audio_write(u16 addr, u8 value)
{
 SquareChannel *channel_1 = &ctx.channel_1;
 switch (addr)
 {
 case 0xff10: {
  channel_1->sweep_raw = value;

  channel_1->sweep_pace      = (value >> 4) & 0x07;
  channel_1->sweep_direction = (value & 0x08) != 0;
  channel_1->sweep_step      = value & 0x07;
 }
 break;
 case 0xff11: {
  channel_1->length_raw = value;

  channel_1->length_wave_duty = (value >> 6) & 0x03;
  channel_1->length_intial_timer = (value & 0x63);
 }
 break;
 case 0xff12:
  ctx.channel_1.volume = value;
 case 0xff13:
  ctx.channel_1.freq_low = value;
 case 0xff14:
  ctx.channel_1.freq_high_control = value;
 case 0xff16:
  ctx.channel_2.length = value;
 case 0xff17:
  ctx.channel_2.volume = value;
 case 0xff18:
  ctx.channel_2.freq_low = value;
 case 0xff19:
  ctx.channel_2.freq_high_control = value;
 case 0xff1A:
  ctx.wave_channel.dac_enable = value;
 case 0xff1B:
  ctx.wave_channel.length = value;
 case 0xff1C:
  ctx.wave_channel.output_level = value;
 default:
  NO_IMPL
 }
}
