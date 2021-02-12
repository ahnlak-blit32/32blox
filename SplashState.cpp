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
  /* And compute the gradient colours for the background. */
  for ( uint16_t i = 0; i < SPLASHSTATE_GRADIENT_HEIGHT / 2; i++ )
  {
    gradient_pen[i] = gradient_pen[SPLASHSTATE_GRADIENT_HEIGHT - i - 1]
                    = blit::Pen( 40 - i / 2, 10 + i, 30 + i / 2 );
  }

  /* The font pen will be simpler. */
  font_pen = blit::Pen( 255, 255, 0 );
  font_tween.init( blit::tween_sine, 255.0f, 100.0f, 500 );

  /* Want the logo to drift a little... */
  logo_tween_x.init( blit::tween_sine, -2.0f, 2.0f, 543 );
  logo_tween_y.init( blit::tween_sine, -1.0f, 1.0f, 345 );
}


/*
 * init - called whenever the game engine is switching to this state
 *
 * GameStateInterface * - the state we were most recently in
 */

void SplashState::init( GameStateInterface *p_previous )
{
  /* Set the tweens running. */
  font_tween.start();
  logo_tween_x.start();
  logo_tween_y.start();

  /* Select the game spritesheet into the screen. */
  blit::screen.sprites = assets.spritesheet_game;

  /* All done. */
  return;
}


/*
 * fini - called whenever the game engine turns off this state.
 * 
 * GameStateInterface * - the state we were switching to
 */

void SplashState::fini( GameStateInterface *p_next )
{
  /* Turn off all our tweens. */
  font_tween.stop();
  logo_tween_x.stop();
  logo_tween_y.stop();

  /* And return. */
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

  /* Place the logo in the middle of the screen. */
  blit::Point l_pos;
  l_pos.x = (blit::screen.bounds.w - assets.surface_logo->bounds.w) / 2;
  l_pos.y = ((blit::screen.bounds.h - assets.surface_logo->bounds.h) / 2) - 20;
  l_pos.x += logo_tween_x.value;
  l_pos.y += logo_tween_y.value;
  blit::screen.blit( assets.surface_logo, assets.surface_logo->clip, l_pos );

  /* And some bricks in the corners, from the spritesheet. */
  blit::screen.sprite( 
    blit::Rect( 0, SPRITE_ROW_BRICK, 4, 2 ),
    blit::Point( 0, 0 )
  );
  blit::screen.sprite( 
    blit::Rect( 4, SPRITE_ROW_BRICK, 4, 2 ),
    blit::Point( 32, 0 )
  );
  blit::screen.sprite( 
    blit::Rect( 4, SPRITE_ROW_BRICK, 4, 2 ),
    blit::Point( blit::screen.bounds.w - 64, 0 )
  );
  blit::screen.sprite( 
    blit::Rect( 0, SPRITE_ROW_BRICK, 4, 2 ),
    blit::Point( blit::screen.bounds.w - 32, 0 )
  );
  blit::screen.sprite( 
    blit::Rect( 0, SPRITE_ROW_BRICK, 4, 2 ),
    blit::Point( 0, blit::screen.bounds.h - 16 )
  );
  blit::screen.sprite( 
    blit::Rect( 4, SPRITE_ROW_BRICK, 4, 2 ),
    blit::Point( 32, blit::screen.bounds.h - 16 )
  );
  blit::screen.sprite( 
    blit::Rect( 4, SPRITE_ROW_BRICK, 4, 2 ),
    blit::Point( blit::screen.bounds.w - 64, blit::screen.bounds.h - 16 )
  );
  blit::screen.sprite( 
    blit::Rect( 0, SPRITE_ROW_BRICK, 4, 2 ),
    blit::Point( blit::screen.bounds.w - 32, blit::screen.bounds.h - 16 )
  );

  /* Lastly, prompt the user to press a button. */
  blit::screen.pen = font_pen;
  blit::screen.text(
    STR_A_TO_START,
    assets.message_font,
    blit::Point( blit::screen.bounds.w / 2, blit::screen.bounds.h - 45 ),
    true,
    blit::TextAlign::center_center
  );

  /* All done. */
  return;
}


/* End of SplashState.cpp */
