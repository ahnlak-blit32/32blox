/*
 * SplashState.hpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The SplashState is the entry to the game, prompting the player to start
 * the game by pressing the button.
 */

#ifndef   _SPLASHSTATE_HPP_
#define   _SPLASHSTATE_HPP_

#include "AssetFactory.hpp"

#define SPLASHSTATE_GRADIENT_HEIGHT 160

class SplashState : public GameStateInterface
{
private:
  AssetFactory   &assets = AssetFactory::get_instance();
  blit::Pen       font_pen;
  blit::Tween     font_tween;
  blit::Tween     logo_tween_x;
  blit::Tween     logo_tween_y;
  blit::Pen       gradient_pen[SPLASHSTATE_GRADIENT_HEIGHT];
  uint8_t         gradient_offset;

public:
                  SplashState( void );
  void            init( GameStateInterface * );
  gamestate_t     update( uint32_t );
  void            render( uint32_t );
};

#endif /* _SPLASHSTATE_HPP_ */

/* End of SplashState.hpp */
