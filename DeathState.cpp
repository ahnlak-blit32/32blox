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
  GameState *l_game = (GameState *)( p_previous );
//  GameState *l_game = dynamic_cast<GameState *>( p_previous );
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
 * fini - called whenever the game engine turns off this state.
 * 
 * GameStateInterface * - the state we were switching to
 */

void DeathState::fini( GameStateInterface *p_next )
{
  /* Turn off the tweens. */
  font_tween.stop();

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

  /* Also, for now the PicoSystem doesn't support saving, so no high score. */
  if ( TARGET_PICOSYSTEM == assets.get_platform() )
  {
    return STATE_SPLASH;
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
    high_score->save( score, name );
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
  blit::screen.pen = blit::Pen( 255, 255, 0 );
  snprintf( l_buffer, 12, "%05d", score );
  blit::screen.text(
    l_buffer,
    assets.message_font,
    blit::Point( blit::screen.bounds.w / 2, 55 ),
    true,
    blit::TextAlign::top_center
  );

  /* And the name, ready for them to change if they want to... */
  blit::screen.pen = blit::Pen( 255, 255, 255 );
  snprintf( l_buffer, 12, "%c %c %c %c %c %c", 
            name[0], name[1], name[2], name[3], name[4], name[5] );
  blit::Size l_name_sz = blit::screen.measure_text( l_buffer, assets.message_font, true );
  blit::Point l_name_box = blit::Point( 
    ( blit::screen.bounds.w - l_name_sz.w ) / 2, 
    ( blit::screen.bounds.h - l_name_sz.h ) / 2
  );

  blit::screen.text(
    l_buffer,
    assets.message_font,
    l_name_box,
    true,
    blit::TextAlign::top_left
  );

  /* The rest is all in a nicely tweened pen. */
  blit::screen.pen = font_pen;

  /* Draw a box around the current letter being edited. */
  blit::Point l_char_box = l_name_box;
  l_char_box.x += cursor * 32 - 5;
  l_char_box.y -= 9;
  blit::screen.h_span( l_char_box, 24 );
  blit::screen.h_span( l_char_box + blit::Point( 0, 32 ), 24 );
  blit::screen.v_span( l_char_box, 32 );
  blit::screen.v_span( l_char_box + blit::Point( 23, 0 ), 32 );

  /* The static messaging next - congrats, and how to enter your name... */
  blit::screen.text(
    assets.get_text( STR_NEW_HIGH_SCORE ),
    assets.message_font,
    blit::Point( blit::screen.bounds.w / 2, 10 ),
    true,
    blit::TextAlign::top_center
  );
  blit::screen.text(
    assets.get_text( STR_LEFT_RIGHT_SELECT ),
    assets.number_font,
    blit::Point( blit::screen.bounds.w / 2, blit::screen.bounds.h - 55 ),
    true,
    blit::TextAlign::bottom_center
  );
  blit::screen.text(
    assets.get_text( STR_UP_DOWN_CHANGE ),
    assets.number_font,
    blit::Point( blit::screen.bounds.w / 2, blit::screen.bounds.h - 40 ),
    true,
    blit::TextAlign::bottom_center
  );

  /* Lastly, prompt the user to press a button. */
  blit::screen.text(
    assets.get_text( STR_B_TO_SAVE ),
    assets.message_font,
    blit::Point( blit::screen.bounds.w / 2, blit::screen.bounds.h - 10 ),
    true,
    blit::TextAlign::bottom_center
  );


  /* All done. */
  return;
}


/* End of DeathState.cpp */
