/*
 * HiscoreState.hpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The Hiscore state displays the high score table to the user, at the end
 * of the current game.
 */

#ifndef   _HISCORESTATE_HPP_
#define   _HISCORESTATE_HPP_

#include "AssetFactory.hpp"
#include "HighScore.hpp"

#define HISCORESTATE_GRADIENT_HEIGHT 160

class HiscoreState : public GameStateInterface
{
private:
  AssetFactory   &assets = AssetFactory::get_instance();
  HighScore      *high_score;
  blit::Pen       font_pen;
  blit::Tween     font_tween;
  blit::Pen       gradient_pen[HISCORESTATE_GRADIENT_HEIGHT];
  uint8_t         gradient_offset;

public:
                  HiscoreState( void );
  void            init( GameStateInterface * );
  gamestate_t     update( uint32_t );
  void            render( uint32_t );
};

#endif /* _HISCORESTATE_HPP_ */

/* End of HiscoreState.hpp */
