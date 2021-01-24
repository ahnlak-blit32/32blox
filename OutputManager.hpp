/*
 * OutputManager.hpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The OutputManager is a singleton class to manage outputs; implemented as a
 * singleton because it's interacted with from everywhere and I'm trying to hide
 * the fact that this is, basically, a global :-)
 */

#ifndef   _OUTPUTMANAGER_HPP_
#define   _OUTPUTMANAGER_HPP_

typedef struct
{
  bool                  sound_enabled;
  bool                  music_enabled;
  bool                  haptic_enabled;  
} output_flags_t;

#define CHANNEL_PICKUP  6
#define CHANNEL_BOUNCE  7


class OutputManager
{
private:
                        OutputManager( void );
  output_flags_t        flags;
  blit::Tween           haptic_tween;
public:
  static OutputManager &get_instance( void );
  bool                  sound_enabled( void );
  bool                  music_enabled( void );
  bool                  haptic_enabled( void );
  void                  enable_sound( bool );
  void                  enable_music( bool );
  void                  enable_haptic( bool );
  void                  update( uint32_t );
  void                  trigger_haptic( float, uint32_t );
  void                  play_effect_bounce( uint16_t );
  void                  play_effect_pickup( void );
};


#endif /* _OUTPUTMANAGER_HPP_ */

/* End of OutputManager.hpp */
