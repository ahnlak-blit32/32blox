/*
 * MenuState.cpp - part of 32Blox (revised edition!)
 *
 * Copyright (C) 2020 Pete Favelle <32blit@ahnlak.com>
 *
 * This file is released under the MIT License; see LICENSE for details
 *
 * The Menu state is an out-of-machine state used to handle the in-game menu;
 * it overrides all other states, and as such is handled by the top level 
 * update/render functions as a whole different thing.
 *
 * The practical upshot of this is that it pauses the general game whenever
 * it's active, which is what you want from an in-game menu!
 */

/* System headers. */

#include <string.h>

/* Local headers. */

#include "32blit.hpp"
#include "32blox.hpp"
#include "assets_images.hpp"

#include "MenuState.hpp"


/* Functions. */

/*
 * constructor - create the contents of this State container.
 */

MenuState::MenuState( void )
{
  /* And compute the gradient colours for the background. */
  for ( uint16_t i = 0; i < MENUSTATE_GRADIENT_HEIGHT / 2; i++ )
  {
    gradient_pen[i] = gradient_pen[MENUSTATE_GRADIENT_HEIGHT - i - 1]
                    = blit::Pen( 40 - i / 2, 10 + i, 30 + i / 2 );
  }

  /* The font pen will be simpler. */
  plain_pen = blit::Pen( 255, 255, 0 );
  font_pen = blit::Pen( 255, 255, 0 );
  font_tween.init( blit::tween_sine, 255.0f, 100.0f, 500 );
}


/*
 * init - called whenever the game engine is switching to this state
 *
 * GameStateInterface * - the state we were most recently in
 */

void MenuState::init( GameStateInterface *p_previous )
{
  /* Set the tweens running. */
  font_tween.start();

  /* And work out the size of menu entries. */
  menu_size = blit::screen.measure_text( "Haptic <OFF>", *assets.message_font );

  /* And set the cursor to the first option. */
  cursor = 0;

  /* All done. */
  return;
}


/*
 * fini - called whenever the game engine turns off this state.
 * 
 * GameStateInterface * - the state we were switching to
 */

void MenuState::fini( GameStateInterface *p_next )
{
  /* Stop the tweens. */
  font_tween.stop();

  /* And we're done. */
  return;
}


/*
 * update - called every tick (~10ms) to update the state of the game.
 * 
 * uint32_t - the elapsed time (in ms) since the game launched.
 */

gamestate_t MenuState::update( uint32_t p_time )
{
  /* In this state, we'll update the background gradient, to make it look */
  /* pretty (or at least, moving so it's obvious we haven't crashed)      */
  if ( MENUSTATE_GRADIENT_HEIGHT < ++gradient_offset )
  {
    gradient_offset = 0;
  }

  /* The font pen we use will pulse more subtlely. */
  font_pen.g = font_tween.value;

  /* Fade any active vibrations. */
  if ( blit::vibration > 0.0f )
  {
    blit::vibration -= 0.05f;
  }
  if ( blit::vibration < 0.05f )
  {
    blit::vibration = 0.0f;
  }

  /* Check for the user pressing up or down, to move the cursor. */
  if ( ( blit::buttons.pressed & blit::Button::DPAD_UP ) && ( cursor > 0 ) )
  {
    blit::vibration = 0.25f;
    cursor--;
  }
  if ( ( blit::buttons.pressed & blit::Button::DPAD_DOWN ) && ( cursor < 2 ) )
  {
    blit::vibration = 0.25f;
    cursor++;
  }

  /* Left or right just toggles. */
  if ( ( blit::buttons.pressed & blit::Button::DPAD_LEFT ) || 
       ( blit::buttons.pressed & blit::Button::DPAD_RIGHT ) )
  {
    blit::vibration = 0.2f;
    switch( cursor )
    {
      case 0:       /* Sound. */
        output.enable_sound( !output.sound_enabled() );
        break;
      case 1:       /* Music. */
        output.enable_music( !output.music_enabled() );
        break;
      case 2:       /* Haptic. */
        output.enable_haptic( !output.haptic_enabled() );
        break;
      default:      /* Should never be reached. */
        break;
    }
  }

  /* For this state, the return value is meaningless. */
  return STATE_NONE;
}


/*
 * render - called every frame (~20ms) to render the screen.
 * 
 * uint32_t - the elapsed time (in ms) since the game launched.
 */

void MenuState::render( uint32_t p_time )
{
  char l_buffer[16];

  /* Clear the screen down. */
  blit::screen.clear();

  /* Draw an animated background gradient, to look pretty. */
  for ( uint16_t i = 0; i < blit::screen.bounds.h; i++ )
  {
    blit::screen.pen = gradient_pen[( i + gradient_offset ) % MENUSTATE_GRADIENT_HEIGHT];
    blit::screen.h_span( blit::Point( 0, i ), blit::screen.bounds.w );
  }

  /* Place the logo in the middle of the screen. */
  blit::Point l_pos;
  l_pos.x = (blit::screen.bounds.w - assets.surface_long_logo->bounds.w) / 2;
  l_pos.y = 10;
  blit::screen.blit( assets.surface_long_logo, assets.surface_long_logo->clip, l_pos );

  /* Now render all our menu options, highlighting what we're currently on. */
  blit::screen.pen = plain_pen;
  blit::screen.text(
    "Sounds",
    *assets.message_font,
    blit::Point( ( blit::screen.bounds.w - menu_size.w ) / 2, 100 ),
    true,
    blit::TextAlign::center_left
  );
  if ( output.sound_enabled() )
  {
    strcpy( l_buffer, "  <ON>" );
  }
  else
  {
    strcpy( l_buffer, " <OFF>" );
  }
  blit::screen.pen = ( cursor == 0 ) ? font_pen : plain_pen;
  blit::screen.text(
    l_buffer,
    *assets.message_font,
    blit::Point( blit::screen.bounds.w / 2, 100 ),
    true,
    blit::TextAlign::center_left
  );

  blit::screen.pen = plain_pen;
  blit::screen.text(
    "Music ",
    *assets.message_font,
    blit::Point( ( blit::screen.bounds.w - menu_size.w ) / 2, 130 ),
    true,
    blit::TextAlign::center_left
  );
  if ( output.music_enabled() )
  {
    strcpy( l_buffer, "  <ON>" );
  }
  else
  {
    strcpy( l_buffer, " <OFF>" );
  }
  blit::screen.pen = ( cursor == 1 ) ? font_pen : plain_pen;
  blit::screen.text(
    l_buffer,
    *assets.message_font,
    blit::Point( blit::screen.bounds.w / 2, 130 ),
    true,
    blit::TextAlign::center_left
  );

  blit::screen.pen = plain_pen;
  blit::screen.text(
    "Haptic",
    *assets.message_font,
    blit::Point( ( blit::screen.bounds.w - menu_size.w ) / 2, 160 ),
    true,
    blit::TextAlign::center_left
  );
  if ( output.haptic_enabled() )
  {
    strcpy( l_buffer, "  <ON>" );
  }
  else
  {
    strcpy( l_buffer, " <OFF>" );
  }
  blit::screen.pen = ( cursor == 2 ) ? font_pen : plain_pen;
  blit::screen.text(
    l_buffer,
    *assets.message_font,
    blit::Point( blit::screen.bounds.w / 2, 160 ),
    true,
    blit::TextAlign::center_left
  );

  blit::screen.pen = plain_pen;
  blit::screen.text(
    "PRESS <MENU> TO RETURN",
    *assets.number_font,
    blit::Point( blit::screen.bounds.w / 2, 200 ),
    true,
    blit::TextAlign::bottom_center
  );

  /* Lastly some gratuitous self-promotion. */
  blit::screen.text(
    "VISIT US AT https://32blit.ahnlak.com",
    *assets.number_font,
    blit::Point( blit::screen.bounds.w / 2, blit::screen.bounds.h - 10 ),
    true,
    blit::TextAlign::bottom_center
  );

  /* All done. */
  return;
}


/* End of MenuState.cpp */
