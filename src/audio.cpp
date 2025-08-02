#include "audio.h"
#include "base_core.h"
#include "bit_constants.h"
#include "gameboy_constants.h"
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

f32 audio_mix_channels()
{
 f32 Result = 0.f;

 return Result;
}

f32 get_square_output(SquareChannel *ch)
{
 if (!ch->freq_trigger)
  return 0.0f;

 // 8-step waveform duty patterns (only 4 used)
 static const u8 duty_table[4][8] = {
     {0, 0, 0, 0, 0, 0, 0, 1}, // 12.5%
     {1, 0, 0, 0, 0, 0, 0, 1}, // 25%
     {1, 0, 0, 0, 0, 1, 1, 1}, // 50%
     {0, 1, 1, 1, 1, 1, 1, 0}  // 75%
 };

 int duty  = ch->length_wave_duty >> 6; // top 2 bits
 int index = ch->length_wave_duty & 7;

 u8 waveform = duty_table[duty][index];
 float amp   = waveform ? ch->initial_volume / 15.0f : 0.0f;

 return amp;
}

f32 get_wave_output(WaveChannel *ch)
{
 if (!ch->freq_trigger || !ch->dac_enable)
  return 0.0f;

 u8 sample = get_wave_channel_sample(ch, ch->wave_pos);
 return sample / 15.0f;
}

float get_noise_output(NoiseChannel *ch)
{
 if (!ch->freq_trigger)
  return 0.0f;

 extern u16 lfsr;
 float amp = (~lfsr & 1) ? ch->initial_volume / 15.0f : 0.0f;
 return amp;
}

void audio_tick()
{
 persist s32 frame_seq_counter = 0;
 persist s32 sample_counter    = 0;

 frame_seq_counter += 1;
 sample_counter += 1;

 constexpr s32 frame_seq_tick = (Gameboy::CLOCK / 512);
 constexpr s32 sample_tick    = (Gameboy::CLOCK / Gameboy::HD_SAMPLING_RATE);
 if (frame_seq_counter >= frame_seq_tick)
 {
  frame_seq_counter -= frame_seq_tick;
  frame_sequencer_tick();
 }

 if (sample_counter >= sample_tick)
 {
  sample_counter -= sample_tick;
  f32 sample = audio_mix_channels();
 }

 square_channel_tick(&ctx.channel_1);
 square_channel_tick(&ctx.channel_2);
 wave_channel_tick();
 noise_channel_tick();
}

void frame_sequencer_tick()
{
 square_channel_length_tick(&ctx.channel_1);
 square_channel_length_tick(&ctx.channel_2);
 wave_channel_length_tick();
 noise_channel_length_tick();

 square_channel_envelope_tick(&ctx.channel_1);
 square_channel_envelope_tick(&ctx.channel_2);
 noise_channel_envelope_tick();

 square_channel_sweep_tick();
}

void square_channel_sweep_tick()
{
 SquareChannel *ch = &ctx.channel_1;

 persist u8 sweep_timer = 0;

 if (ch->sweep_pace == 0 && ch->sweep_step == 0)
  return;

 sweep_timer--;

 if (sweep_timer == 0)
 {
  sweep_timer = ch->sweep_pace == 0 ? 8 : ch->sweep_pace;

  u16 delta = ch->frequency >> ch->sweep_step;
  u16 new_freq =
      ch->sweep_direction ? ch->frequency - delta : ch->frequency + delta;

  if (new_freq > 2047)
  {
   ch->freq_trigger = 0;
  }
  else
  {
   ch->frequency = new_freq;

   ch->freq_low_raw  = new_freq & 0xff;
   ch->freq_high_raw = (ch->freq_high_raw & 0xF8) | ((new_freq >> 8) & 0x07);
  }
 }
}

void square_channel_tick(SquareChannel *ch)
{
 if (!ch->freq_trigger)
  return;

 ch->ticks--;
 if (ch->ticks == 0)
 {
  ch->ticks = (2048 - ch->frequency) * 4;

  ch->length_wave_duty = (ch->length_wave_duty + 1) % 8;
 }
}

constexpr u8 noise_divisor_lut[] = {8, 16, 32, 48, 64, 80, 96, 112};

void noise_channel_tick()
{
 NoiseChannel *ch = &ctx.noise_channel;

 persist u16 lfsr        = 0x7fff;
 persist u16 noise_timer = 0;

 if (!ch->freq_trigger)
  return;

 noise_timer--;

 if (noise_timer == 0)
 {
  noise_timer = noise_divisor_lut[ch->clock_divider] << ch->clock_shift;

  u8 xor_result = ((lfsr & 1) ^ (lfsr >> 1) & 1);
  lfsr >>= 1;
  lfsr |= xor_result << 14;

  if (ch->lfsr_width)
  {
   lfsr &= ~(1 << 6);
   lfsr |= xor_result << 5;
  }
 }
}

void wave_channel_tick()
{
 WaveChannel *ch = &ctx.wave_channel;
 if (!ch->freq_trigger || !ch->dac_enable)
  return;

 ch->wave_timer--;

 if (ch->wave_timer == 0)
 {
  ch->wave_timer = (2048 - ch->frequency) * 2;
  ch->wave_pos   = (ch->wave_pos + 1) % 32;
 }
}

void square_channel_envelope_tick(SquareChannel *ch)
{
 if (ch->volume_sweep_pace == 0)
  return;

 persist u8 envelope_timer = 0;

 envelope_timer++;
 if (envelope_timer >= ch->volume_sweep_pace)
 {
  envelope_timer = 0;

  if (ch->env_dir && ch->initial_volume < 15)
   ch->initial_volume++;
  else if (!ch->env_dir && ch->initial_volume > 0)
  {
   ch->initial_volume--;
  }
 }
}

void noise_channel_envelope_tick()
{
 NoiseChannel *ch = &ctx.noise_channel;

 if (ch->volume_sweep_pace == 0)
  return;

 ch->ticks++;
 if (ch->ticks >= ch->volume_sweep_pace)
 {
  ch->ticks = 0;

  if (ch->env_dir && ch->initial_volume < 15)
   ch->initial_volume++;
  else if (!ch->env_dir && ch->initial_volume > 0)
  {
   ch->initial_volume--;
  }
 }
}

void square_channel_length_tick(SquareChannel *ch)
{
 if (ch->length_enable && ch->length_intial_timer > 0)
 {
  ch->length_intial_timer--;
  if (ch->length_intial_timer == 0)
  {
   ch->freq_trigger = 0;
  }
 }
}

void wave_channel_length_tick()
{
 WaveChannel *ch = &ctx.wave_channel;

 if (ch->length_enable && ch->length_timer > 0)
 {
  ch->length_timer--;
  if (ch->length_timer == 0)
  {
   ch->freq_trigger = 0;
  }
 }
}

void noise_channel_length_tick()
{
 NoiseChannel *ch = &ctx.noise_channel;

 if (ch->length_enable && ch->length_timer > 0)
 {
  ch->length_timer--;
  if (ch->length_timer == 0)
  {
   ch->freq_trigger = 0;
  }
 }
}
