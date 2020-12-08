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
  /* The bat height will always be the same, but is bound by the screen size */
  /* so can't quite be hard coded.                                           */
  bat_height = blit::screen.bounds.h - 10;

  /* All done. */
  return;
}


/*
 * init - called whenever the game engine is switching to this state
 *
 * GameStateInterface * - the state we were most recently in
 */

void GameState::init( GameStateInterface *p_previous )
{
  /* Select the game spritesheet into the screen. */
  blit::screen.sprites = assets.spritesheet_game;

  /* Load the first level. */
  level = new Level( 1 );

  /* Reset the lives count and score. */
  lives = 3;
  score = 0;

  /* Centre the bat, and set it to a default type. */
  bat_position = blit::screen.bounds.w / 2;
  bat_speed = 0.5f;
  bat_type = BAT_NORMAL;

  /* Clear out the list of balls, and spawn one on the bat. */
  balls.clear();
  Ball *l_ball = new Ball( blit::Vec2( bat_position, bat_height - 4 ) );
  l_ball->sticky = true;
  balls.push_front( l_ball );

  /* All done. */
  return;
}


/*
 * move_bat - updates the bat position, taking into account the bat size and
 *            the edges of the screen.
 * float - the movement amount wanted; this may get clipped if the edge is hit
 */
void GameState::move_bat( float p_movement )
{
  /* First up, let's apply the full movement. */
  float l_last_pos = bat_position;
  bat_position += p_movement;

  /* And then clamp it, top and bottom. */
  if ( bat_position < ( bat_width[bat_type] / 2 ) )
  {
    bat_position = bat_width[bat_type] / 2;
  }
  if ( bat_position > blit::screen.bounds.w - ( bat_width[bat_type] / 2 ) )
  {
    bat_position = blit::screen.bounds.w - ( bat_width[bat_type] / 2 );
  }

  /* Now, check to see if any of our balls are sticky. */
  for ( auto l_ball : balls )
  {
    /* No point in even checking if they aren't. */
    if ( !l_ball->sticky )
    {
      continue;
    }

    /* So, consider if the ball is (a) at the height of the bat. */
    blit::Rect l_bounds = l_ball->get_bounds();
    if ( ( l_bounds.bl().y ) == bat_height )
    {
      /* Obviously we're only stuck to the bat if we're on it! */
      if ( ( l_bounds.bl().x >= bat_position - ( bat_width[bat_type] / 2 ) ) &&
           ( l_bounds.br().x <= bat_position + ( bat_width[bat_type] / 2 ) ) )
      {
        l_ball->offset( blit::Vec2( bat_position - l_last_pos, 0.0f ) );
      }
    }
  }

  /* All done! */
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
  /* Calculate any bat movement that's required. */
  float l_movement = 0.0f;

  /* Handle the joystick, which is slightly more gradiated. */
  if ( blit::joystick.x < -0.66f )
  {
    l_movement = bat_speed - 1;
  }
  else if ( blit::joystick.x > 0.66f )
  {
    l_movement = bat_speed;
  }
  else
  {
    l_movement = bat_speed * 1.5 * blit::joystick.x;
  }

  /* But if the player has moved the dpad, then use that instead. */
  if ( blit::buttons.state & blit::Button::DPAD_LEFT )
  {
    l_movement = bat_speed * -1;
  }
  if ( blit::buttons.state & blit::Button::DPAD_RIGHT )
  {
    l_movement = bat_speed;
  }

  /* And lastly, apply that movement. */
  if ( l_movement != 0.0f )
  {
    move_bat( l_movement );
  }

  /* Next up, we work our way through all the balls we have, and update their */
  /* positions. We'll deal with any collisions in a little while...           */
  for ( auto l_ball : balls )
  {
    l_ball->update();
  }

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
        blit::Rect( ( l_brick - 1 ) * 4, SPRITE_ROW_BRICK, 4, 2 ),
        blit::Point( l_column * 32, l_row * 16 )
      );
    }
  }

  /* Add in the bat; the position is the centre location. */
  switch( bat_type )
  {
    case BAT_NORMAL:  /* Simple bat, two sprites wide. */
      blit::screen.sprite(
        blit::Rect( 0, SPRITE_ROW_BAT, 3, 1 ),
        blit::Point( bat_position - ( bat_width[bat_type] / 2 ), bat_height )
      );
      break;
  }

  /* Balls next; we could have a number of them, in a handy container. */
  for ( auto l_ball : balls )
  {
    /* Render the ball. */
    blit::screen.sprite(
      blit::Rect( l_ball->get_type(), SPRITE_ROW_BALL, 1, 1 ),
      l_ball->get_render_location()
    );
  }

  /* All done. */
  return;
}

/* End of GameState.cpp */
