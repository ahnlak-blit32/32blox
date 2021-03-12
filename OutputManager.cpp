/*
 * OutputManager.cpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The OutputManager is a singleton class to manage outputs; the options as to
 * which output channels are enabled are saved, so that we always remember if
 * we should be playing music, sounds or haptics.
 */

/* System headers. */

/* Local headers. */

#include "32blit.hpp"
#include "32blox.hpp"

#include "OutputManager.hpp"
#include "assets_audio.hpp"


/* Functions. */


/*
 * constructor - Initialises all the asset objects
 */

OutputManager::OutputManager( void )
{
  /* See if we've saved any defaults. */
  if ( !blit::read_save( flags, SAVE_SLOT_OUTPUT ) )
  {
    /* Then set some defaults, which is "all enabled" */
    flags.sound_enabled = true;
    flags.music_enabled = true;
    flags.haptic_enabled = false;
  }

  /* Set up the sound channels. */
  blit::channels[CHANNEL_LEVEL].waveforms  = blit::Waveform::TRIANGLE | blit::Waveform::SINE | blit::Waveform::SQUARE;
  blit::channels[CHANNEL_LEVEL].frequency  = 3500;
  blit::channels[CHANNEL_LEVEL].volume     = 0xffff;
  blit::channels[CHANNEL_LEVEL].attack_ms  = 32;
  blit::channels[CHANNEL_LEVEL].decay_ms   = 512;
  blit::channels[CHANNEL_LEVEL].sustain    = 256;
  blit::channels[CHANNEL_LEVEL].release_ms = 128;

  blit::channels[CHANNEL_FALLING].waveforms  = blit::Waveform::SINE;
  blit::channels[CHANNEL_FALLING].frequency  = 1000;
  blit::channels[CHANNEL_FALLING].volume     = 0x3fff;
  blit::channels[CHANNEL_FALLING].attack_ms  = 4;
  blit::channels[CHANNEL_FALLING].decay_ms   = 32;
  blit::channels[CHANNEL_FALLING].sustain    = 0;
  blit::channels[CHANNEL_FALLING].release_ms = 32;

  blit::channels[CHANNEL_PICKUP].waveforms  = blit::Waveform::TRIANGLE;
  blit::channels[CHANNEL_PICKUP].frequency  = 1400;
  blit::channels[CHANNEL_PICKUP].volume     = 0xffff;
  blit::channels[CHANNEL_PICKUP].attack_ms  = 8;
  blit::channels[CHANNEL_PICKUP].decay_ms   = 128;
  blit::channels[CHANNEL_PICKUP].sustain    = 0;
  blit::channels[CHANNEL_PICKUP].release_ms = 64;

  blit::channels[CHANNEL_BOUNCE].waveforms  = blit::Waveform::SAW | blit::Waveform::NOISE;
  blit::channels[CHANNEL_BOUNCE].volume     = 0x7fff;
  blit::channels[CHANNEL_BOUNCE].attack_ms  = 8;
  blit::channels[CHANNEL_BOUNCE].decay_ms   = 32;
  blit::channels[CHANNEL_BOUNCE].sustain    = 0;
  blit::channels[CHANNEL_BOUNCE].release_ms = 16;

  /* All done. */
  return;
}


/*
 * get_instance - fetches the singleton instance of the OutputManager
 */

OutputManager &OutputManager::get_instance( void )
{
  static OutputManager myself;
  return myself;
}


/*
 * x_enabled - functions to access the various flags.
 */

bool OutputManager::sound_enabled( void )
{
  return flags.sound_enabled;
}
bool OutputManager::music_enabled( void )
{
  return flags.music_enabled;
}
bool OutputManager::haptic_enabled( void )
{
  return flags.haptic_enabled;
}


/*
 * enable_x - functions to set the flag to the provided boolean value.
 *
 * These functions also save the config out once the change is made.
 */

void OutputManager::enable_sound( bool p_flag )
{
  /* Set and save the flag. */
  flags.sound_enabled = p_flag;
  blit::write_save( flags, SAVE_SLOT_OUTPUT );

  /* And turn off any currently playing sounds. */
  blit::channels[CHANNEL_FALLING].off();
  blit::channels[CHANNEL_PICKUP].off();
  blit::channels[CHANNEL_BOUNCE].off();
  return;
}
void OutputManager::enable_music( bool p_flag )
{
  /* Set and save the flag. */
  flags.music_enabled = p_flag;
  blit::write_save( flags, SAVE_SLOT_OUTPUT );

  /* And either stop or start the music, as required. */
  if ( p_flag )
  {
    play_music();
  }
  else
  {
    stop_music();
  }
  return;
}
void OutputManager::enable_haptic( bool p_flag )
{
  flags.haptic_enabled = p_flag;
  blit::write_save( flags, SAVE_SLOT_OUTPUT );
  return;
}


/*
 * update - called every tick to do any output processing that is required;
 *          playing or decaying music and sound effects, and the haptic stuff.
 *
 * uint32_t - the time index from the main update() function.
 */

void OutputManager::update( uint32_t p_time )
{
  /* Update the haptic setting if the tween is active, and haptics are on. */
  if ( flags.haptic_enabled && haptic_tween.is_running() )
  {
    blit::vibration = haptic_tween.value;
  }
  else
  {
    blit::vibration = 0.0f;
  }

  /* All done. */
  return;
}


/*
 * trigger_haptic - launches a haptic buzz
 *
 * float - the strength of the buzz, from 0.0 to 1.0
 * uint32_t - the duratio nof the buzz, in milliseconds
 */

void OutputManager::trigger_haptic( float p_strength, uint32_t p_duration )
{
  /* Just set the tween running. */
  haptic_tween.init( blit::tween_linear, p_strength, 0.0f, p_duration, 1 );
  haptic_tween.start();

  /* All done. */
  return;
}


/*
 * play_effect_bounce - plays the sound effect of the ball bouncing off something
 *
 * uint16_t - the frequency to use
 */

void OutputManager::play_effect_bounce( uint16_t p_frequency )
{
  /* Only do this if sound effects are enabled. */
  if ( flags.sound_enabled )
  {
    blit::channels[CHANNEL_BOUNCE].frequency  = p_frequency;
    blit::channels[CHANNEL_BOUNCE].trigger_attack();
  }

  /* All done. */
  return;
}


/*
 * play_effect_pickup - plays the sound effect of a powerup being picked up.
 */

void OutputManager::play_effect_pickup( void )
{
  /* Only do this if sound effects are enabled. */
  if ( flags.sound_enabled )
  {
    blit::channels[CHANNEL_PICKUP].trigger_attack();
  }

  /* All done. */
  return;
}


/*
 * play_effect_falling - plays the sound of a powerup dropping from the sky
 *
 * uint8_t - the current row on the screen of the powerup, or 0 to turn off
 */

void OutputManager::play_effect_falling( uint8_t p_height )
{
  /* Only do this if sound effects are enabled. */
  if ( flags.sound_enabled )
  {
    blit::channels[CHANNEL_FALLING].frequency  = 1000 - p_height * 4;
    blit::channels[CHANNEL_FALLING].trigger_attack();
  }

  /* All done. */
  return;
}


/*
 * play_effect_level_complete - plays a beep when you clear the level.
 */

void OutputManager::play_effect_level_complete( void )
{
  /* Only do this if sound effects are enabled. */
  if ( flags.sound_enabled )
  {
    blit::channels[CHANNEL_LEVEL].trigger_attack();
  }

  /* All done. */
  return;
}


/*
 * play_music / stop_music - runs the compiled-in WAV file. These functions
 * are pretty much lifted from the ever-talented DaftFreak.
 */

void play_wav(int channel, const uint8_t *ptr, bool loop = false);
void stop_wav(int channel);

void OutputManager::play_music( void )
{
  blit::channels[CHANNEL_MUSIC].volume = 0x7fff;
  play_wav( CHANNEL_MUSIC, a_audio_music, true );
}

void OutputManager::stop_music( void )
{
  stop_wav( CHANNEL_MUSIC );
}


/* End of OutputManager.cpp */
