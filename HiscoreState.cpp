/*
 * HiscoreState.cpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The Hiscore state displays the high score table to the user, at the end
 * of the current game.
 */


/* System headers. */

/* Local headers. */

#include "32blit.hpp"
#include "32blox.hpp"
#include "assets_images.hpp"

#include "HiscoreState.hpp"
#include "HighScore.hpp"


/* Functions. */

/*
 * constructor - create the contents of this State container.
 */

HiscoreState::HiscoreState( void )
{
  /* Load up the game spritesheet. */
  //sprites = blit::SpriteSheet::load( a_game_sprites_img );

  /* And compute the gradient colours for the background. */
  for ( uint16_t i = 0; i < HISCORESTATE_GRADIENT_HEIGHT / 2; i++ )
  {
    gradient_pen[i] = gradient_pen[HISCORESTATE_GRADIENT_HEIGHT - i - 1]
                    = blit::Pen( 10 + i, 40 - i / 2, 30 + i / 2 );
  }

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

void HiscoreState::init( GameStateInterface *p_previous )
{
  /* Ask the high score table to reload itself. */
  high_score->load();

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

gamestate_t HiscoreState::update( uint32_t p_time )
{
  /* Only real inputs here, is asking for the A button to restart. */
  if ( blit::buttons.pressed & blit::Button::A )
  {
    font_tween.stop();
    return STATE_GAME;
  }

  /* In this state, we'll update the background gradient, to make it look */
  /* pretty (or at least, moving so it's obvious we haven't crashed)      */
  if ( HISCORESTATE_GRADIENT_HEIGHT < ++gradient_offset )
  {
    gradient_offset = 0;
  }

  /* The font pen we use will pulse more subtlely. */
  font_pen.g = font_tween.value;

  /* All done, remain in our current state */
  return STATE_HISCORE;
}


/*
 * render - called every frame (~20ms) to render the screen.
 * 
 * uint32_t - the elapsed time (in ms) since the game launched.
 */

void HiscoreState::render( uint32_t p_time )
{
  char l_buffer[32];
  uint8_t l_row_offset = 9;
  const hiscore_t *l_entry;

  /* Clear the screen down. */
  blit::screen.clear();

  /* Draw an animated background gradient, to look pretty. */
  for ( uint16_t i = 0; i < blit::screen.bounds.h; i++ )
  {
    blit::screen.pen = gradient_pen[( i + gradient_offset ) % HISCORESTATE_GRADIENT_HEIGHT];
    blit::screen.h_span( blit::Point( 0, i ), blit::screen.bounds.w );
  }

  /* Display the top 'n' high scores, that will fit onto the screen. */
  blit::screen.pen = blit::Pen( 255, 255, 255 );
  for( uint8_t i = 0; i < 10; i++ )
  {
    /* Fetch the high score entry for this position. */
    l_entry = high_score->get_entry( i );

    /* If there isn't one, we're done. */
    if ( ( l_entry == nullptr ) || ( l_entry->score == 0 ) ) 
    {
      break;
    }

    /* Format it up nicely, using fixed width for safety. */
    snprintf( 
      l_buffer, 32, "%-6s - %05d", 
      l_entry->name,
      l_entry->score
    );
    blit::screen.text (
      l_buffer,
      blit::minimal_font,
      blit::Point( blit::screen.bounds.w / 2, 20 + i * l_row_offset ),
      false,
      blit::TextAlign::center_center
    );
  }

  /* The static messaging next - hi score table heading. */
  blit::screen.pen = font_pen;
  blit::screen.text(
    "HIGH SCORES",
    blit::fat_font,
    blit::Point( blit::screen.bounds.w / 2, 10 ),
    true,
    blit::TextAlign::center_center
  );

  /* Lastly, prompt the user to press a button. */
  blit::screen.text(
    "PRESS 'A' TO START",
    blit::fat_font,
    blit::Point( blit::screen.bounds.w / 2, blit::screen.bounds.h - 10 ),
    true,
    blit::TextAlign::center_center
  );

  /* All done. */
  return;
}

/* End of HiscoreState.cpp */
