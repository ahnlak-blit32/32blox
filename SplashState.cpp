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
#include "assets_images.hpp"

#include "SplashState.hpp"


/* Functions. */

/*
 * constructor - create the contents of this State container.
 */

SplashState::SplashState( void )
{
  /* Create the surface containing our splash screen. */
  splash_surface = blit::Surface::load( a_splash_img );

  /* And compute the gradient colours for the background. */
  for ( uint16_t i = 0; i < SPLASHSTATE_GRADIENT_HEIGHT / 2; i++ )
  {
    gradient_pen[i] = gradient_pen[SPLASHSTATE_GRADIENT_HEIGHT - i - 1]
                    = blit::Pen( 40 - i / 2, 10 + i, 30 + i / 2 );
  }

  /* The font pen will be simpler. */
  font_pen = blit::Pen( 255, 255, 0 );
  font_tween.init( blit::tween_sine, 255.0f, 100.0f, 500, -1 );
}


/*
 * init - called whenever the game engine is switching to this state
 *
 * GameStateInterface * - the state we were most recently in
 */

void SplashState::init( GameStateInterface *p_previous )
{
  /* Set the font tween running. */
  font_tween.start();

  /* All done. */
  return;
}


/*
 * update - called every tick (~10ms) to update the state of the game.
 * 
 * uint32_t - the elapsed time (in ms) since the game launched.
 */

gamestate_t SplashState::update( uint32_t p_time )
{
  /* We also need to check to see if the user has pressed the A button. */
  if ( blit::buttons.pressed & blit::Button::A )
  {
    font_tween.stop();
    return STATE_GAME;
  }

  /* In this state, we'll update the background gradient, to make it look */
  /* pretty (or at least, moving so it's obvious we haven't crashed)      */
  if ( SPLASHSTATE_GRADIENT_HEIGHT < ++gradient_offset )
  {
    gradient_offset = 0;
  }

  /* The font pen we use will pulse more subtlely. */
  font_pen.g = font_tween.value;

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
  for ( uint16_t i = 0; i < blit::screen.bounds.h; i++ )
  {
    blit::screen.pen = gradient_pen[( i + gradient_offset ) % SPLASHSTATE_GRADIENT_HEIGHT];
    blit::screen.h_span( blit::Point( 0, i ), blit::screen.bounds.w );
  }

  /* And then blit the splash screen over the top of that. */
  if ( nullptr != splash_surface )
  {
    blit::screen.blit( splash_surface, splash_surface->clip, blit::Point( 0, 0 ) );
  }

  /* Lastly, prompt the user to press a button. */
  blit::screen.pen = font_pen;
  blit::screen.text(
    "PRESS 'A' TO START",
    blit::fat_font,
    blit::Point( blit::screen.bounds.w / 2, blit::screen.bounds.h - 20 ),
    true,
    blit::TextAlign::center_center
  );

  /* All done. */
  return;
}


/* End of SplashState.cpp */
