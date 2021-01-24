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
  blit::channels[CHANNEL_PICKUP].waveforms  = blit::Waveform::TRIANGLE;
  blit::channels[CHANNEL_PICKUP].frequency  = 1400;
  blit::channels[CHANNEL_PICKUP].volume     = 0x7fff;
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
  flags.sound_enabled = p_flag;
  blit::write_save( flags, SAVE_SLOT_OUTPUT );
  return;
}
void OutputManager::enable_music( bool p_flag )
{
  flags.music_enabled = p_flag;
  blit::write_save( flags, SAVE_SLOT_OUTPUT );
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


/* End of OutputManager.cpp */
