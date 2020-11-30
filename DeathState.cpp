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

#include <string.h>


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
  /* Set our name to a default. */
  strcpy( name, "AAAAAA" );

  /* And we'll need access to the high score table. */
  high_score = new HighScore();

  /* The font pen will be simpler. */
  font_pen = blit::Pen( 255, 255, 0 );
  font_tween.init( blit::tween_sine, 255.0f, 100.0f, 500, -1 );  
}


/*
 * init - called whenever the game engine is switching to this state
 *
 * GameStateInterface * - the state we were most recently in
 */

void DeathState::init( GameStateInterface *p_previous )
{
  /* Set the font tween running. */
  font_tween.start();

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
  if ( high_score->rank( score ) == MAX_SCORES )
  {
    score = 0;
  }

  /* Set the cursor to the beginning of the name. */
  cursor = 0;

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
    font_tween.stop();
    return STATE_HISCORE;
  }

  /* The font pen we use will pulse more subtlely. */
  font_pen.g = font_tween.value;

  /* Left and right simply move the cursor. */
  if ( ( blit::buttons.pressed & blit::Button::DPAD_LEFT ) && ( cursor > 0 ) )
  {
    cursor--;
  }
  if ( ( blit::buttons.pressed & blit::Button::DPAD_RIGHT ) && ( cursor < 5 ) )
  {
    cursor++;
  }
  if ( ( blit::buttons.pressed & blit::Button::DPAD_UP ) && ( name[cursor] < 'Z' ) )
  {
    name[cursor]++;
  }
  if ( ( blit::buttons.pressed & blit::Button::DPAD_DOWN ) && ( name[cursor] > 'A' ) )
  {
    name[cursor]--;
  }

  /* If the user presses the save button, then we save their score and move on. */
  if ( blit::buttons.pressed & blit::Button::B )
  {
    printf( "%s has scored %d\n", name, score );
    high_score->save( score, name );
    font_tween.stop();
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
  char l_buffer[16];

  /* Clear the screen down. */
  blit::screen.clear();

  /* Draw a smooth gradient backdrop. */
  for ( uint16_t i = 0; i < blit::screen.bounds.h; i++ )
  {
    blit::screen.pen = blit::Pen( ( ( blit::screen.bounds.h - i ) / 2 ), 10, 10 );
    blit::screen.h_span( blit::Point( 0, i ), blit::screen.bounds.w );
  }

  /* Show what the score was. */
  blit::screen.pen = blit::Pen( 255, 255, 255 );
  snprintf( l_buffer, 12, "%05d", score );
  blit::screen.text(
    l_buffer,
    blit::minimal_font,
    blit::Point( blit::screen.bounds.w / 2, 30 ),
    true,
    blit::TextAlign::center_center
  );

  /* And the name, ready for them to change if they want to... */
  snprintf( l_buffer, 12, "%c %c %c %c %c %c", 
            name[0], name[1], name[2], name[3], name[4], name[5] );
  blit::screen.text(
    l_buffer,
    blit::minimal_font,
    blit::Point( blit::screen.bounds.w / 2, 50 ),
    true,
    blit::TextAlign::center_center
  );

  /* The rest is all in a nicely tweened pen. */
  blit::screen.pen = font_pen;

  /* Draw a box around the current letter being edited. */
  blit::screen.h_span( blit::Point( 53 + cursor * 9, 43 ), 8 );
  blit::screen.h_span( blit::Point( 53 + cursor * 9, 56 ), 8 );
  blit::screen.v_span( blit::Point( 53 + cursor * 9, 43 ), 14 );
  blit::screen.v_span( blit::Point( 61 + cursor * 9, 43 ), 14 );

  /* The static messaging next - congrats, and how to enter your name... */
  blit::screen.text(
    "NEW HIGH SCORE!",
    blit::fat_font,
    blit::Point( blit::screen.bounds.w / 2, 10 ),
    true,
    blit::TextAlign::center_center
  );
  blit::screen.text(
    "LEFT/RIGHT TO SELECT",
    blit::fat_font,
    blit::Point( blit::screen.bounds.w / 2, blit::screen.bounds.h - 40 ),
    true,
    blit::TextAlign::center_center
  );
  blit::screen.text(
    "UP/DOWN TO CHANGE",
    blit::fat_font,
    blit::Point( blit::screen.bounds.w / 2, blit::screen.bounds.h - 30 ),
    true,
    blit::TextAlign::center_center
  );

  /* Lastly, prompt the user to press a button. */
  blit::screen.text(
    "PRESS 'B' TO SAVE",
    blit::fat_font,
    blit::Point( blit::screen.bounds.w / 2, blit::screen.bounds.h - 10 ),
    true,
    blit::TextAlign::center_center
  );


  /* All done. */
  return;
}


/* End of DeathState.cpp */
