/*
 * DeathState.cpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The Death state is reached once the game is over; we check to see if there
 * is a new hi score to record and, if so, get the player initials.
 */

/* System headers. */

/* Local headers. */

#include "32blit.hpp"
#include "32blox.hpp"
#include "assets_images.hpp"

#include "DeathState.hpp"
#include "GameState.hpp"
#include "HighScore.hpp"


/* Functions. */

/*
 * constructor - create the contents of this State container.
 */

DeathState::DeathState( void )
{
  /* Load up the game spritesheet. */
  //sprites = blit::SpriteSheet::load( a_game_sprites_img );

  /* And we'll need access to the high score table. */
  high_score = new HighScore();
}


/*
 * init - called whenever the game engine is switching to this state
 *
 * GameStateInterface * - the state we were most recently in
 */

void DeathState::init( GameStateInterface *p_previous )
{
  /* The previous state *should* have been a GameState. */
  GameState *l_game = dynamic_cast<GameState *>( p_previous );
  if ( nullptr == l_game )
  {
    /* Then we have no idea what state we're in, and have to move on. */
    score = 0;
    return;
  }
  score = l_game->get_score();

  /* If the ranking is zero, that means there's no point in recording it. */
  if ( 0 == high_score->rank( score ) )
  {
    score = 0;
  }

  /* All done. */
  return;
}


/*
 * update - called every tick (~10ms) to update the state of the game.
 * 
 * uint32_t - the elapsed time (in ms) since the game launched.
 */

gamestate_t DeathState::update( uint32_t p_time )
{
  /* If our score doesn't even rank, then we move on. */
  if ( 0 == score )
  {
    return STATE_HISCORE;
  }

  /* All done, remain in our current state */
  return STATE_DEATH;
}


/*
 * render - called every frame (~20ms) to render the screen.
 * 
 * uint32_t - the elapsed time (in ms) since the game launched.
 */

void DeathState::render( uint32_t p_time )
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


/* End of DeathState.cpp */
