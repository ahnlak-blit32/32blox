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
  lives = 3;
  score = 0;

  /* All done. */
  return;
}


/*
 * get_score - exposes the current score for the current game
 */

uint16_t GameState::get_score( void )
{
  return score;
}


/*
 * update - called every tick (~10ms) to update the state of the game.
 * 
 * uint32_t - the elapsed time (in ms) since the game launched.
 */

gamestate_t GameState::update( uint32_t p_time )
{

  /* If after all that we have no more lives, it's game over. */
  if ( lives == 0 )
  {
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
  uint8_t l_brick;

  /* Clear the screen down. */
  blit::screen.clear();

  /* Draw a smooth gradient backdrop. */
  for ( uint16_t i = 0; i < blit::screen.bounds.h; i++ )
  {
    blit::screen.pen = blit::Pen( 10, 10, ( ( blit::screen.bounds.h - i ) / 2 ) );
    blit::screen.h_span( blit::Point( 0, i ), blit::screen.bounds.w );
  }

  /* Now we work through the level one brick at a time... */
  for ( uint8_t l_row = 0; l_row < BOARD_HEIGHT; l_row++ )
  {
    for ( uint8_t l_column = 0; l_column < BOARD_WIDTH; l_column++ )
    {
      /* Fetch the brick for this location. */
      l_brick = level->get_brick( l_row, l_column );

      /* Move on if it's empty. */
      if ( l_brick == 0 )
      {
        continue;
      }

      /* Then draw the appropriate brick from the spritesheet. */
      blit::screen.sprite( 
        blit::Rect( ( l_brick - 1 ) * 2, SPRITE_ROW_BRICK, 2, 1 ),
        blit::Point( l_column * 16, l_row * 8 )
      );
    }
  }

  /* All done. */
  return;
}

/* End of GameState.cpp */
