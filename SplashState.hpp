/*
 * SplashState.hpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * This is the main entry point of the game; it provides the init/update/render
 * functions expected by the 32blit firmware.
 */

#ifndef   _SPLASHSTATE_HPP_
#define   _SPLASHSTATE_HPP_

#define SPLASHSTATE_GRADIENT_HEIGHT 160

class SplashState : public GameStateInterface
{
private:
  blit::Surface  *splash_surface;
  blit::Pen       gradient_pen[SPLASHSTATE_GRADIENT_HEIGHT];
  uint8_t         gradient_offset;

public:
                  SplashState( void );
  gamestate_t     update( uint32_t );
  void            render( uint32_t );
};

#endif /* _SPLASHSTATE_HPP_ */

/* End of SplashState.hpp */
