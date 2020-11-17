/*
 * GameState.cpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * This state contains the core gameplay logic; by definition, it is by far
 * the largest and most complex state.
 */

/* System headers. */

/* Local headers. */

#include "32blit.hpp"
#include "32blox.hpp"
#include "assets_images.hpp"

#include "GameState.hpp"
#include "Level.hpp"


/* Functions. */

/*
 * constructor - create the contents of this State container.
 */

GameState::GameState( void )
{
  /* Load up the game spritesheet. */
  sprites = blit::SpriteSheet::load( a_game_sprites_img );
}


/*
 * init - called whenever the game engine is switching to this state
 *
 * GameStateInterface * - the state we were most recently in
 */

void GameState::init( GameStateInterface *p_previous )
{
  /* Load the first level. */
  level = new Level( 1 );

  /* Reset the lives count and score. */

  /* All done. */
  return;
}


/*
 * get_score - exposes the current score for the current game
 */

uint16_t GameState::get_score( void )
{
  return 1; // score;
}


/*
 * update - called every tick (~10ms) to update the state of the game.
 * 
 * uint32_t - the elapsed time (in ms) since the game launched.
 */

gamestate_t GameState::update( uint32_t p_time )
{

if ( p_time % 1000 == 0 ) {
printf( "%d bricks left..\n", level->get_brick_count() );
return STATE_DEATH;
}

  /* All done, remain in our current state */
  return STATE_GAME;
}


/*
 * render - called every frame (~20ms) to render the screen.
 * 
 * uint32_t - the elapsed time (in ms) since the game launched.
 */

void GameState::render( uint32_t p_time )
{
  /* Clear the screen down. */
  blit::screen.clear();

  /* Draw a smooth gradient backdrop. */
  for ( uint16_t i = 0; i < blit::screen.bounds.h; i++ )
  {
    blit::screen.pen = blit::Pen( 10, 10, ( ( blit::screen.bounds.h - i ) / 2 ) );
    blit::screen.h_span( blit::Point( 0, i ), blit::screen.bounds.w );
  }

  /* All done. */
  return;
}

/* End of GameState.cpp */
