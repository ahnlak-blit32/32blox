/*
 * GameState.hpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The GameState is the core of the game, active whenever the game is actually
 * in play.
 */

#ifndef   _GAMESTATE_HPP_
#define   _GAMESTATE_HPP_

typedef enum
{
  BAT_NORMAL,
  BAT_NARROW,
  BAT_WIDE,
  BAT_MAX
} bat_type_t;

#include "Level.hpp"

class GameState : public GameStateInterface
{
private:
  blit::SpriteSheet  *sprites;
  Level              *level;
  uint8_t             lives;
  float               bat_position;
  float               bat_speed;
  bat_type_t          bat_type;
  uint8_t             bat_width[BAT_MAX] = { 16, 8, 24 };
  
  uint16_t            score;

  void                init( void );
  void                move_bat( float );

public:
                      GameState( void );
  void                init( GameStateInterface * );
  uint16_t            get_score( void );
  gamestate_t         update( uint32_t );
  void                render( uint32_t );
};

#endif /* _GAMESTATE_HPP_ */

/* End of GameState.hpp */
