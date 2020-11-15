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

#include "Level.hpp"

class GameState : public GameStateInterface
{
private:
  blit::SpriteSheet  *sprites;
  Level              *level;

  void                init( void );

public:
                      GameState( void );
  gamestate_t         update( uint32_t );
  void                render( uint32_t );
};

#endif /* _GAMESTATE_HPP_ */

/* End of GameState.hpp */
