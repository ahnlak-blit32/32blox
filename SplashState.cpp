/*
 * SplashState.cpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * This State class manages the Splash state of the game; here we just show
 * a nicely animated title screen and invite the user to press A to start.
 */

/* System headers. */

/* Local headers. */

#include "32blit.hpp"
#include "32blox.hpp"
#include "assets.hpp"

#include "SplashState.hpp"


/* Functions. */

/*
 * constructor - create the contents of this State container.
 */

SplashState::SplashState( void )
{
  /* Create the surface containing our splash screen. */
  splash_surface = blit::Surface::load( asset_splash );

  /* And compute the gradient colours for the background. */
  for ( uint8_t i = 0; i < SPLASHSTATE_GRADIENT_HEIGHT/2; i++ )
  {
    gradient_pen[i] = gradient_pen[SPLASHSTATE_GRADIENT_HEIGHT-i-1] = blit::Pen( 40-i/2, 10+i, 30+i/2 );
  }
}

/*
 * update - called every tick (~10ms) to update the state of the game.
 * 
 * uint32_t - the elapsed time (in ms) since the game launched.
 */

gamestate_t SplashState::update( uint32_t p_time )
{
  /* In this state, we'll update the background gradient, to make it look */
  /* pretty (or at least, moving so it's obvious we haven't crashed)      */
  if ( SPLASHSTATE_GRADIENT_HEIGHT < ++gradient_offset )
  {
    gradient_offset = 0;
  }

  /* We also need to check to see if the user has pressed the A button. */


  /* All done, remain in our current state */
  return STATE_SPLASH;
}


/*
 * render - called every frame (~20ms) to render the screen.
 * 
 * uint32_t - the elapsed time (in ms) since the game launched.
 */

void SplashState::render( uint32_t p_time )
{
  /* Clear the screen down. */
  blit::screen.clear();

  /* Draw an animated background gradient, to look pretty. */
  for ( uint8_t i = 0; i < blit::screen.bounds.h; i++ )
  {
    blit::screen.pen = gradient_pen[(i+gradient_offset)%SPLASHSTATE_GRADIENT_HEIGHT];
    blit::screen.h_span( blit::Point( 0, i ), blit::screen.bounds.w );
  }


  /* And then blit the splash screen over the top of that. */
  if ( nullptr != splash_surface )
  {
    blit::screen.blit( splash_surface, splash_surface->clip, blit::Point( 0, 0 ) );
  }

  /* All done. */
  return;
}


/* End of SplashState.cpp */
