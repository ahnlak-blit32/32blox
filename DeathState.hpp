/*
 * DeathState.hpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The Death state is reached once the game is over; we check to see if there
 * is a new hi score to record and, if so, get the player initials.
 */

#ifndef   _DEATHSTATE_HPP_
#define   _DEATHSTATE_HPP_

#include "AssetFactory.hpp"
#include "HighScore.hpp"

class DeathState : public GameStateInterface
{
private:
  AssetFactory   &assets = AssetFactory::get_instance();
  char            name[7];
  uint16_t        score;
  uint8_t         cursor;  
  HighScore      *high_score;
  blit::Pen       font_pen;
  blit::Tween     font_tween;

public:
                  DeathState( void );
  void            init( GameStateInterface * );
  void            fini( GameStateInterface * );
  gamestate_t     update( uint32_t );
  void            render( uint32_t );
};

#endif /* _DEATHSTATE_HPP_ */

/* End of DeathState.hpp */
