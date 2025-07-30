#pragma once

#include "base_core.h"
/*
 * 0xff26 - NR52: Audio Master Control
 * 0xff25 - NR51: Sound Panning
 * 0xff24 - NR50: Master Volume & VIN panning
 * 0xff10 - NR10: Channel 1 Sweep
 * 0xff11 - NR11: Channel 1 length timer & duty cycle
 * 0xff12 - NR12: Channel 1 volume & envelope
 * 0xff13 - NR13: Channel 1 period low (write-only)
 * 0xff14 - NR14: Channel 1 period high & control
 * 0xff16 - NR21: Channel 2 Sweep
 * 0xff17 - NR22: Channel 2 volume & envelope
 * 0xff18 - NR23: Channel 2 period low (write-only)
 * 0xff19 - NR24: Channel 2 period high & control
 * 0xff1A - NR30: Channel 3 DAC enable
 * 0xff1B - NR31: Channel 3 length timer (write-only)
 * 0xff1c - NR32: Channel 3 output level
 * 0xff1d - NR33: Channel 3 period low (write-only)
 * 0xff1e - NR34: Channel 3 period high & control
 * 0xff30 - 0xff3f - wave pattern ram
 * 0xff20 - NR41: Channel 4 length timer (write-only)
 * 0xff21 - NR42: Channel 4 volume & envelope
 * 0xff22 - NR43: Channel 4 frequency & randomness
 * 0xff23 - NR44: Channel 4 control
 *
 */

struct SquareChannel
{
 u8 sweep_raw;
 u8 length_raw;
 u8 volume_raw;

 u8 freq_low_raw;  // 8 bits of the 11 bit period value
 u8 freq_high_raw; //

 // sweep values
 u8 sweep_pace;
 u8 sweep_direction;
 u8 sweep_step;

 // length values
 u8 length_wave_duty;
 u8 length_intial_timer;

 // volume
 u8 initial_volume;
 u8 env_dir;
 u8 volume_sweep_pace;

 // frequency
 u16 frequency;
 u8 length_enable;
 u8 freq_trigger;

 u8 ticks;
};

struct WaveChannel
{
  u8 dac_raw;

 u8 dac_enable;
 u8 length_timer; // write only
 u8 output_level_raw;

 u8 output_level;

 u8 freq_low_raw;
 u8 freq_high_raw;

 u16 frequency;
 u8 length_enable;
 u8 freq_trigger;

 // wave pattern RAM
 u8 wave_ram[16];
};

struct NoiseChannel
{
 u8 length_timer;
 u8 volume_raw;

 u8 initial_volume;
 u8 env_dir;
 u8 volume_sweep_pace;

 u8 frequency_raw;

 u8 clock_shift;
 u8 lfsr_width;
 u8 clock_divider;

 u8 control_raw;
 u8 freq_trigger;
 u8 length_enable;
};

struct APU
{
 SquareChannel channel_1;
 SquareChannel channel_2;
 WaveChannel wave_channel;
 NoiseChannel noise_channel;

 u8 audio_master_control;
 u8 sound_panning;
 u8 master_volume;
};

PURE u8 audio_read(u16 addr);
void audio_write(u16 addr, u8 value);
void audio_tick();
