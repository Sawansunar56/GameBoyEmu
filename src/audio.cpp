#include "audio.h"
#include "base_core.h"
#include "bit_constants.h"
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
 SquareChannel *channel_1    = &ctx.channel_1;
 SquareChannel *channel_2    = &ctx.channel_2;
 WaveChannel *wave_channel   = &ctx.wave_channel;
 NoiseChannel *noise_channel = &ctx.noise_channel;

 if (BETWEEN(addr, 0xff30, 0xff3f))
 {
  // check this
  wave_channel->wave_ram[(addr & 0xf)] = value;
 }

 switch (addr)
 {
 case 0xff10: {
  channel_1->sweep_raw = value;

  channel_1->sweep_pace      = (value >> 4) & (u8)BIT_CONST::NBIT_3;
  channel_1->sweep_direction = (value & BIT_CONST::BIT_4) != 0;
  channel_1->sweep_step      = value & BIT_CONST::NBIT_3;
 }
 break;
 case 0xff11: {
  channel_1->length_raw = value;

  channel_1->length_wave_duty    = (value >> 6) & BIT_CONST::NBIT_2;
  channel_1->length_intial_timer = (value & BIT_CONST::NBIT_6);
 }
 break;
 case 0xff12: {
  channel_1->volume_raw = value;

  channel_1->initial_volume    = (value >> 4) & BIT_CONST::NBIT_4;
  channel_1->env_dir           = (value & BIT_CONST::BIT_4) != 0;
  channel_1->volume_sweep_pace = (value & BIT_CONST::NBIT_3);
 }
 break;
 case 0xff13: {
  channel_1->freq_low_raw = value;

  channel_1->frequency = static_cast<u16>(value);
 }
 break;
 case 0xff14: {
  channel_1->freq_high_raw = value;

  channel_1->frequency =
      ((value & BIT_CONST::NBIT_3) << 7) | channel_1->frequency;
  channel_1->length_enable = (value & BIT_CONST::BIT_7) != 0;
  channel_1->freq_trigger  = (value & BIT_CONST::BIT_8) != 0;
 }
 break;
 case 0xff16: {
  channel_2->length_raw = value;

  channel_2->length_wave_duty    = (value >> 6) & BIT_CONST::NBIT_2;
  channel_2->length_intial_timer = (value & BIT_CONST::NBIT_6);
 }
 break;
 case 0xff17: {
  channel_2->volume_raw = value;

  channel_2->initial_volume    = (value >> 4) & BIT_CONST::NBIT_4;
  channel_2->env_dir           = (value & BIT_CONST::BIT_4) != 0;
  channel_2->volume_sweep_pace = (value & BIT_CONST::NBIT_3);
 }
 break;
 case 0xff18: {
  channel_2->freq_low_raw = value;

  channel_2->frequency = static_cast<u16>(value);
 }
 break;
 case 0xff19: {
  channel_2->freq_high_raw = value;

  channel_2->frequency =
      ((value & BIT_CONST::NBIT_3) << 7) | channel_1->frequency;
  channel_2->length_enable = (value & BIT_CONST::BIT_7) != 0;
  channel_2->freq_trigger  = (value & BIT_CONST::BIT_8) != 0;
 }
 break;
 case 0xff1A: {
  wave_channel->dac_raw = value;

  wave_channel->dac_enable = (value >> 7);
 }
 break;
 case 0xff1B: {
  wave_channel->length_timer = value;
 }
 break;
 case 0xff1C: {
  wave_channel->output_level_raw = value;

  wave_channel->output_level = (value >> 5) & BIT_CONST::NBIT_2;
 }
 break;
 case 0xff1d: {
  wave_channel->freq_low_raw = value;

  wave_channel->frequency = static_cast<u16>(value);
 }
 break;
 case 0xff1e: {
  wave_channel->freq_high_raw = value;

  wave_channel->frequency =
      ((value & BIT_CONST::NBIT_3) << 7) | wave_channel->frequency;
  wave_channel->length_enable = (value & BIT_CONST::BIT_7) != 0;
  wave_channel->freq_trigger  = (value & BIT_CONST::BIT_8) != 0;
 }
 break;
 case 0xff20: {
  noise_channel->length_timer = value;
 }
 break;
 case 0xff21: {
  noise_channel->volume_raw = value;

  noise_channel->initial_volume    = (value >> 4) & BIT_CONST::NBIT_4;
  noise_channel->env_dir           = (value & BIT_CONST::BIT_4) != 0;
  noise_channel->volume_sweep_pace = (value & BIT_CONST::NBIT_3);
 }
 break;
 case 0xff22: {
  noise_channel->frequency_raw = value;

  noise_channel->clock_shift   = (value >> 4) & BIT_CONST::NBIT_4;
  noise_channel->lfsr_width    = (value & BIT_CONST::BIT_4) != 0;
  noise_channel->clock_divider = (value & BIT_CONST::NBIT_3);
 }
 break;
 case 0xff23: {
  noise_channel->control_raw = value;

  noise_channel->freq_trigger  = (value & BIT_CONST::BIT_8) != 0;
  noise_channel->length_enable = (value & BIT_CONST::BIT_7) != 0;
 }
 break;
 default:
  NO_IMPL
 }
}
void audio_tick()
{
}
